#
# Makefile for the netfilter modules on top of IPv4.
#
ifeq ($(CONFIG_MIPS),y)
ifeq ($(CONFIG_CPU_MIPS32_R1),y)
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel_r1/include -I$(srctree)/brcm/mipsel_r1/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel_r1/include -I$(srctree)/brcm/mipsel_r1/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel_r1/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel_r1/common/include
else
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel/include -I$(srctree)/brcm/mipsel/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel/include -I$(srctree)/brcm/mipsel/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel/common/include
endif
else
EXTRA_CFLAGS+=-I$(srctree)/brcm/arm/include -I$(srctree)/brcm/arm/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/arm/include -I$(srctree)/brcm/arm/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/arm/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/arm/common/include
endif

# objects for l3 independent conntrack
nf_conntrack_ipv4-y	:=  nf_conntrack_l3proto_ipv4.o nf_conntrack_proto_icmp.o
ifeq ($(CONFIG_NF_CONNTRACK_PROC_COMPAT),y)
ifeq ($(CONFIG_PROC_FS),y)
nf_conntrack_ipv4-objs	+= nf_conntrack_l3proto_ipv4_compat.o
endif
endif

# connection tracking
obj-$(CONFIG_NF_CONNTRACK_IPV4) += nf_conntrack_ipv4.o

nf_nat_ipv4-y		:= nf_nat_l3proto_ipv4.o nf_nat_proto_icmp.o
obj-$(CONFIG_NF_NAT_IPV4) += nf_nat_ipv4.o

# defrag
obj-$(CONFIG_NF_DEFRAG_IPV4) += nf_defrag_ipv4.o

# logging
obj-$(CONFIG_NF_LOG_ARP) += nf_log_arp.o
obj-$(CONFIG_NF_LOG_IPV4) += nf_log_ipv4.o

# reject
obj-$(CONFIG_NF_REJECT_IPV4) += nf_reject_ipv4.o


# NAT helpers (nf_conntrack)
obj-$(CONFIG_NF_NAT_H323) += nf_nat_h323.o
obj-$(CONFIG_NF_NAT_PPTP) += nf_nat_pptp.o
obj-$(CONFIG_NF_NAT_RTSP) += nf_nat_rtsp.o
obj-$(CONFIG_NF_NAT_SNMP_BASIC) += nf_nat_snmp_basic.o
obj-$(CONFIG_NF_NAT_MASQUERADE_IPV4) += nf_nat_masquerade_ipv4.o

# NAT protocols (nf_nat)
obj-$(CONFIG_NF_NAT_PROTO_GRE) += nf_nat_proto_gre.o

obj-$(CONFIG_NF_TABLES_IPV4) += nf_tables_ipv4.o
obj-$(CONFIG_NFT_CHAIN_ROUTE_IPV4) += nft_chain_route_ipv4.o
obj-$(CONFIG_NFT_CHAIN_NAT_IPV4) += nft_chain_nat_ipv4.o
obj-$(CONFIG_NFT_REJECT_IPV4) += nft_reject_ipv4.o
obj-$(CONFIG_NFT_MASQ_IPV4) += nft_masq_ipv4.o
obj-$(CONFIG_NFT_REDIR_IPV4) += nft_redir_ipv4.o
obj-$(CONFIG_NFT_DUP_IPV4) += nft_dup_ipv4.o
obj-$(CONFIG_NF_TABLES_ARP) += nf_tables_arp.o

# generic IP tables 
obj-$(CONFIG_IP_NF_IPTABLES) += ip_tables.o

# the three instances of ip_tables
obj-$(CONFIG_IP_NF_FILTER) += iptable_filter.o
obj-$(CONFIG_IP_NF_MANGLE) += iptable_mangle.o
obj-$(CONFIG_IP_NF_NAT) += iptable_nat.o
obj-$(CONFIG_IP_NF_RAW) += iptable_raw.o
obj-$(CONFIG_IP_NF_SECURITY) += iptable_security.o

# matches
obj-$(CONFIG_IP_NF_MATCH_AH) += ipt_ah.o
obj-$(CONFIG_IP_NF_MATCH_RPFILTER) += ipt_rpfilter.o
obj-$(CONFIG_IP_NF_MATCH_WEBSTR) += ipt_webstr.o
obj-$(CONFIG_IP_NF_MATCH_IPP2P) += xt_ipp2p.o compat_xtables.o

# targets
obj-$(CONFIG_IP_NF_TARGET_CLUSTERIP) += ipt_CLUSTERIP.o
obj-$(CONFIG_IP_NF_TARGET_ECN) += ipt_ECN.o
obj-$(CONFIG_IP_NF_TARGET_MASQUERADE) += ipt_MASQUERADE.o
obj-$(CONFIG_IP_NF_TARGET_REJECT) += ipt_REJECT.o
obj-$(CONFIG_IP_NF_TARGET_SYNPROXY) += ipt_SYNPROXY.o
obj-$(CONFIG_IP_NF_TARGET_TRIGGER) += ipt_TRIGGER.o

# generic ARP tables
obj-$(CONFIG_IP_NF_ARPTABLES) += arp_tables.o
obj-$(CONFIG_IP_NF_ARP_MANGLE) += arpt_mangle.o

# just filtering instance of ARP tables for now
obj-$(CONFIG_IP_NF_ARPFILTER) += arptable_filter.o

obj-$(CONFIG_NF_DUP_IPV4) += nf_dup_ipv4.o
