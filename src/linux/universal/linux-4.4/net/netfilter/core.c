/* netfilter.c: look after the filters for various protocols.
 * Heavily influenced by the old firewall.c by David Bonn and Alan Cox.
 *
 * Thanks to Rob `CmdrTaco' Malda for not influencing this code in any
 * way.
 *
 * Rusty Russell (C)2000 -- This code is GPL.
 * Patrick McHardy (c) 2006-2012
 */
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <net/protocol.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/if.h>
#include <linux/netdevice.h>
#include <linux/netfilter_ipv6.h>
#include <linux/inetdevice.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/locallock.h>
#include <net/net_namespace.h>
#include <net/sock.h>

#ifdef CONFIG_BCM47XX
#include <typedefs.h>
#include <bcmdefs.h>
#else
#define BCMFASTPATH_HOST
#define BCMFASTPATH
#endif

#include <net/netfilter/nf_conntrack_rtcache.h>

#include "nf_internals.h"

#ifdef CONFIG_PREEMPT_RT_BASE
DEFINE_LOCAL_IRQ_LOCK(xt_write_lock);
EXPORT_PER_CPU_SYMBOL(xt_write_lock);
#endif

static DEFINE_MUTEX(afinfo_mutex);

const struct nf_afinfo __rcu *nf_afinfo[NFPROTO_NUMPROTO] __read_mostly;
EXPORT_SYMBOL(nf_afinfo);
const struct nf_ipv6_ops __rcu *nf_ipv6_ops __read_mostly;
EXPORT_SYMBOL_GPL(nf_ipv6_ops);

DEFINE_PER_CPU(bool, nf_skb_duplicated);
EXPORT_SYMBOL_GPL(nf_skb_duplicated);

int nf_register_afinfo(const struct nf_afinfo *afinfo)
{
	mutex_lock(&afinfo_mutex);
	RCU_INIT_POINTER(nf_afinfo[afinfo->family], afinfo);
	mutex_unlock(&afinfo_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(nf_register_afinfo);

void nf_unregister_afinfo(const struct nf_afinfo *afinfo)
{
	mutex_lock(&afinfo_mutex);
	RCU_INIT_POINTER(nf_afinfo[afinfo->family], NULL);
	mutex_unlock(&afinfo_mutex);
	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(nf_unregister_afinfo);

#ifdef HAVE_JUMP_LABEL
struct static_key nf_hooks_needed[NFPROTO_NUMPROTO][NF_MAX_HOOKS];
EXPORT_SYMBOL(nf_hooks_needed);
#endif

static DEFINE_MUTEX(nf_hook_mutex);

static struct list_head *nf_find_hook_list(struct net *net,
					   const struct nf_hook_ops *reg)
{
	struct list_head *hook_list = NULL;

	if (reg->pf != NFPROTO_NETDEV)
		hook_list = &net->nf.hooks[reg->pf][reg->hooknum];
	else if (reg->hooknum == NF_NETDEV_INGRESS) {
#ifdef CONFIG_NETFILTER_INGRESS
		if (reg->dev && dev_net(reg->dev) == net)
			hook_list = &reg->dev->nf_hooks_ingress;
#endif
	}
	return hook_list;
}

struct nf_hook_entry {
	const struct nf_hook_ops	*orig_ops;
	struct nf_hook_ops		ops;
};

int nf_register_net_hook(struct net *net, const struct nf_hook_ops *reg)
{
	struct list_head *hook_list;
	struct nf_hook_entry *entry;
	struct nf_hook_ops *elem;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->orig_ops	= reg;
	entry->ops	= *reg;

	hook_list = nf_find_hook_list(net, reg);
	if (!hook_list) {
		kfree(entry);
		return -ENOENT;
	}

	mutex_lock(&nf_hook_mutex);
	list_for_each_entry(elem, hook_list, list) {
		if (reg->priority < elem->priority)
			break;
	}
	list_add_rcu(&entry->ops.list, elem->list.prev);
	mutex_unlock(&nf_hook_mutex);
#ifdef CONFIG_NETFILTER_INGRESS
	if (reg->pf == NFPROTO_NETDEV && reg->hooknum == NF_NETDEV_INGRESS)
		net_inc_ingress_queue();
#endif
#ifdef HAVE_JUMP_LABEL
	static_key_slow_inc(&nf_hooks_needed[reg->pf][reg->hooknum]);
#endif
	return 0;
}
EXPORT_SYMBOL(nf_register_net_hook);

void nf_unregister_net_hook(struct net *net, const struct nf_hook_ops *reg)
{
	struct list_head *hook_list;
	struct nf_hook_entry *entry;
	struct nf_hook_ops *elem;

	hook_list = nf_find_hook_list(net, reg);
	if (!hook_list)
		return;

	mutex_lock(&nf_hook_mutex);
	list_for_each_entry(elem, hook_list, list) {
		entry = container_of(elem, struct nf_hook_entry, ops);
		if (entry->orig_ops == reg) {
			list_del_rcu(&entry->ops.list);
			break;
		}
	}
	mutex_unlock(&nf_hook_mutex);
	if (&elem->list == hook_list) {
		WARN(1, "nf_unregister_net_hook: hook not found!\n");
		return;
	}
#ifdef CONFIG_NETFILTER_INGRESS
	if (reg->pf == NFPROTO_NETDEV && reg->hooknum == NF_NETDEV_INGRESS)
		net_dec_ingress_queue();
#endif
#ifdef HAVE_JUMP_LABEL
	static_key_slow_dec(&nf_hooks_needed[reg->pf][reg->hooknum]);
#endif
	synchronize_net();
	nf_queue_nf_hook_drop(net, &entry->ops);
	/* other cpu might still process nfqueue verdict that used reg */
	synchronize_net();
	kfree(entry);
}
EXPORT_SYMBOL(nf_unregister_net_hook);

int nf_register_net_hooks(struct net *net, const struct nf_hook_ops *reg,
			  unsigned int n)
{
	unsigned int i;
	int err = 0;

	for (i = 0; i < n; i++) {
		err = nf_register_net_hook(net, &reg[i]);
		if (err)
			goto err;
	}
	return err;

err:
	if (i > 0)
		nf_unregister_net_hooks(net, reg, i);
	return err;
}
EXPORT_SYMBOL(nf_register_net_hooks);

void nf_unregister_net_hooks(struct net *net, const struct nf_hook_ops *reg,
			     unsigned int n)
{
	while (n-- > 0)
		nf_unregister_net_hook(net, &reg[n]);
}
EXPORT_SYMBOL(nf_unregister_net_hooks);

static LIST_HEAD(nf_hook_list);

int nf_register_hook(struct nf_hook_ops *reg)
{
	struct net *net, *last;
	int ret;

	rtnl_lock();
	for_each_net(net) {
		ret = nf_register_net_hook(net, reg);
		if (ret && ret != -ENOENT)
			goto rollback;
	}
	list_add_tail(&reg->list, &nf_hook_list);
	rtnl_unlock();

	return 0;
rollback:
	last = net;
	for_each_net(net) {
		if (net == last)
			break;
		nf_unregister_net_hook(net, reg);
	}
	rtnl_unlock();
	return ret;
}
EXPORT_SYMBOL(nf_register_hook);

void nf_unregister_hook(struct nf_hook_ops *reg)
{
	struct net *net;

	rtnl_lock();
	list_del(&reg->list);
	for_each_net(net)
		nf_unregister_net_hook(net, reg);
	rtnl_unlock();
}
EXPORT_SYMBOL(nf_unregister_hook);

int nf_register_hooks(struct nf_hook_ops *reg, unsigned int n)
{
	unsigned int i;
	int err = 0;

	for (i = 0; i < n; i++) {
		err = nf_register_hook(&reg[i]);
		if (err)
			goto err;
	}
	return err;

err:
	if (i > 0)
		nf_unregister_hooks(reg, i);
	return err;
}
EXPORT_SYMBOL(nf_register_hooks);

void nf_unregister_hooks(struct nf_hook_ops *reg, unsigned int n)
{
	while (n-- > 0)
		nf_unregister_hook(&reg[n]);
}
EXPORT_SYMBOL(nf_unregister_hooks);

unsigned int BCMFASTPATH_HOST nf_iterate(struct list_head *head,
			struct sk_buff *skb,
			struct nf_hook_state *state,
			struct nf_hook_ops **elemp)
{
	unsigned int verdict;

	/*
	 * The caller must not block between calls to this
	 * function because of risk of continuing from deleted element.
	 */
	list_for_each_entry_continue_rcu((*elemp), head, list) {
		if (state->thresh > (*elemp)->priority)
			continue;

		/* Optimization: we don't need to hold module
		   reference here, since function can't sleep. --RR */
repeat:
		verdict = (*elemp)->hook((*elemp)->priv, skb, state);
		if (verdict != NF_ACCEPT) {
#ifdef CONFIG_NETFILTER_DEBUG
			if (unlikely((verdict & NF_VERDICT_MASK)
							> NF_MAX_VERDICT)) {
				NFDEBUG("Evil return from %p(%u).\n",
					(*elemp)->hook, state->hook);
				continue;
			}
#endif
			if (verdict != NF_REPEAT)
				return verdict;
			goto repeat;
		}
	}
	return NF_ACCEPT;
}


/* Returns 1 if okfn() needs to be executed by the caller,
 * -EPERM for NF_DROP, 0 otherwise. */
int nf_hook_slow(struct sk_buff *skb, struct nf_hook_state *state)
{
	struct nf_hook_ops *elem;
	unsigned int verdict;
	int ret = 0;

	/* We may already have this, but read-locks nest anyway */
	rcu_read_lock();

	elem = list_entry_rcu(state->hook_list, struct nf_hook_ops, list);
next_hook:
	verdict = nf_iterate(state->hook_list, skb, state, &elem);
	if (verdict == NF_ACCEPT || verdict == NF_STOP) {
		ret = 1;
	} else if ((verdict & NF_VERDICT_MASK) == NF_DROP) {
		kfree_skb(skb);
		ret = NF_DROP_GETERR(verdict);
		if (ret == 0)
			ret = -EPERM;
	} else if ((verdict & NF_VERDICT_MASK) == NF_QUEUE ||
		   (verdict & NF_VERDICT_MASK) == NF_IMQ_QUEUE) {
		int err = nf_queue(skb, elem, state,
			       verdict >> NF_VERDICT_QBITS,
			       verdict & NF_VERDICT_MASK);
		if (err < 0) {
			if (err == -ESRCH &&
			   (verdict & NF_VERDICT_FLAG_QUEUE_BYPASS))
				goto next_hook;
			kfree_skb(skb);
		}
	}
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(nf_hook_slow);


int skb_make_writable(struct sk_buff *skb, unsigned int writable_len)
{
	if (writable_len > skb->len)
		return 0;

	/* Not exclusive use of packet?  Must copy. */
	if (!skb_cloned(skb)) {
		if (writable_len <= skb_headlen(skb))
			return 1;
	} else if (skb_clone_writable(skb, writable_len))
		return 1;

	if (writable_len <= skb_headlen(skb))
		writable_len = 0;
	else
		writable_len -= skb_headlen(skb);

	return !!__pskb_pull_tail(skb, writable_len);
}
EXPORT_SYMBOL(skb_make_writable);

/* This needs to be compiled in any case to avoid dependencies between the
 * nfnetlink_queue code and nf_conntrack.
 */
struct nfnl_ct_hook __rcu *nfnl_ct_hook __read_mostly;
EXPORT_SYMBOL_GPL(nfnl_ct_hook);

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
/* This does not belong here, but locally generated errors need it if connection
   tracking in use: without this, connection may not be in hash table, and hence
   manufactured ICMP or RST packets will not be associated with it. */
void (*ip_ct_attach)(struct sk_buff *, const struct sk_buff *)
		__rcu __read_mostly;
EXPORT_SYMBOL(ip_ct_attach);

void nf_ct_attach(struct sk_buff *new, const struct sk_buff *skb)
{
	void (*attach)(struct sk_buff *, const struct sk_buff *);

	if (skb->nfct) {
		rcu_read_lock();
		attach = rcu_dereference(ip_ct_attach);
		if (attach)
			attach(new, skb);
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL(nf_ct_attach);

void (*nf_ct_destroy)(struct nf_conntrack *) __rcu __read_mostly;
EXPORT_SYMBOL(nf_ct_destroy);

void nf_conntrack_destroy(struct nf_conntrack *nfct)
{
	void (*destroy)(struct nf_conntrack *);

	rcu_read_lock();
	destroy = rcu_dereference(nf_ct_destroy);
	BUG_ON(destroy == NULL);
	destroy(nfct);
	rcu_read_unlock();
}
EXPORT_SYMBOL(nf_conntrack_destroy);

/* Built-in default zone used e.g. by modules. */
const struct nf_conntrack_zone nf_ct_zone_dflt = {
	.id	= NF_CT_DEFAULT_ZONE_ID,
	.dir	= NF_CT_DEFAULT_ZONE_DIR,
};
EXPORT_SYMBOL_GPL(nf_ct_zone_dflt);

#if IS_ENABLED(CONFIG_NF_CONNTRACK_RTCACHE)
/* returns true if dev matches the last recorded
 * input interface of the conntrack attached to skb.
 *
 * This is not in conntrack to avoid module dependency.
 */
bool nf_conn_rtcache_match_dev(const struct sk_buff *skb,
			       const struct net_device *dev)
{
	struct nf_conn_rtcache *rtc;
	enum ip_conntrack_info ctinfo;
	enum ip_conntrack_dir dir;
	struct nf_conn *ct;
	int iif;

	ct = nf_ct_get(skb, &ctinfo);
	rtc = nf_ct_rtcache_find(ct);
	if (!rtc)
		return false;

	dir = CTINFO2DIR(ctinfo);
	iif = nf_conn_rtcache_iif_get(rtc, dir);

	return iif == dev->ifindex;
}
EXPORT_SYMBOL_GPL(nf_conn_rtcache_match_dev);
#endif

#endif /* CONFIG_NF_CONNTRACK */

#ifdef CONFIG_NF_NAT_NEEDED
void (*nf_nat_decode_session_hook)(struct sk_buff *, struct flowi *);
EXPORT_SYMBOL(nf_nat_decode_session_hook);
#endif

static int nf_register_hook_list(struct net *net)
{
	struct nf_hook_ops *elem;
	int ret;

	rtnl_lock();
	list_for_each_entry(elem, &nf_hook_list, list) {
		ret = nf_register_net_hook(net, elem);
		if (ret && ret != -ENOENT)
			goto out_undo;
	}
	rtnl_unlock();
	return 0;

out_undo:
	list_for_each_entry_continue_reverse(elem, &nf_hook_list, list)
		nf_unregister_net_hook(net, elem);
	rtnl_unlock();
	return ret;
}

static void nf_unregister_hook_list(struct net *net)
{
	struct nf_hook_ops *elem;

	rtnl_lock();
	list_for_each_entry(elem, &nf_hook_list, list)
		nf_unregister_net_hook(net, elem);
	rtnl_unlock();
}

static int __net_init netfilter_net_init(struct net *net)
{
	int i, h, ret;

	for (i = 0; i < ARRAY_SIZE(net->nf.hooks); i++) {
		for (h = 0; h < NF_MAX_HOOKS; h++)
			INIT_LIST_HEAD(&net->nf.hooks[i][h]);
	}

#ifdef CONFIG_PROC_FS
	net->nf.proc_netfilter = proc_net_mkdir(net, "netfilter",
						net->proc_net);
	if (!net->nf.proc_netfilter) {
		if (!net_eq(net, &init_net))
			pr_err("cannot create netfilter proc entry");

		return -ENOMEM;
	}
#endif
	ret = nf_register_hook_list(net);
	if (ret)
		remove_proc_entry("netfilter", net->proc_net);

	return ret;
}

static void __net_exit netfilter_net_exit(struct net *net)
{
	nf_unregister_hook_list(net);
	remove_proc_entry("netfilter", net->proc_net);
}

static struct pernet_operations netfilter_net_ops = {
	.init = netfilter_net_init,
	.exit = netfilter_net_exit,
};

int __init netfilter_init(void)
{
	int ret;

	ret = register_pernet_subsys(&netfilter_net_ops);
	if (ret < 0)
		goto err;

	ret = netfilter_log_init();
	if (ret < 0)
		goto err_pernet;

	return 0;
err_pernet:
	unregister_pernet_subsys(&netfilter_net_ops);
err:
	return ret;
}
