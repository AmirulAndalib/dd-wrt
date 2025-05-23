/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _ISISC_API_H_
#define _ISISC_API_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#ifdef IN_PORTCONTROL
#define PORTCONTROL_API \
    SW_API_DEF(SW_API_PT_DUPLEX_GET, isisc_port_duplex_get), \
    SW_API_DEF(SW_API_PT_DUPLEX_SET, isisc_port_duplex_set), \
    SW_API_DEF(SW_API_PT_SPEED_GET, isisc_port_speed_get), \
    SW_API_DEF(SW_API_PT_SPEED_SET, isisc_port_speed_set), \
    SW_API_DEF(SW_API_PT_AN_GET, isisc_port_autoneg_status_get), \
    SW_API_DEF(SW_API_PT_AN_ENABLE, isisc_port_autoneg_enable), \
    SW_API_DEF(SW_API_PT_AN_RESTART, isisc_port_autoneg_restart), \
    SW_API_DEF(SW_API_PT_AN_ADV_GET, isisc_port_autoneg_adv_get), \
    SW_API_DEF(SW_API_PT_AN_ADV_SET, isisc_port_autoneg_adv_set), \
    SW_API_DEF(SW_API_PT_FLOWCTRL_SET, isisc_port_flowctrl_set), \
    SW_API_DEF(SW_API_PT_FLOWCTRL_GET, isisc_port_flowctrl_get), \
    SW_API_DEF(SW_API_PT_FLOWCTRL_MODE_SET, isisc_port_flowctrl_forcemode_set), \
    SW_API_DEF(SW_API_PT_FLOWCTRL_MODE_GET, isisc_port_flowctrl_forcemode_get), \
    SW_API_DEF(SW_API_PT_POWERSAVE_SET, isisc_port_powersave_set), \
    SW_API_DEF(SW_API_PT_POWERSAVE_GET, isisc_port_powersave_get), \
    SW_API_DEF(SW_API_PT_HIBERNATE_SET, isisc_port_hibernate_set), \
    SW_API_DEF(SW_API_PT_HIBERNATE_GET, isisc_port_hibernate_get), \
    SW_API_DEF(SW_API_PT_CDT, isisc_port_cdt), \
    SW_API_DEF(SW_API_PT_TXHDR_SET, isisc_port_txhdr_mode_set), \
    SW_API_DEF(SW_API_PT_TXHDR_GET, isisc_port_txhdr_mode_get), \
    SW_API_DEF(SW_API_PT_RXHDR_SET, isisc_port_rxhdr_mode_set), \
    SW_API_DEF(SW_API_PT_RXHDR_GET, isisc_port_rxhdr_mode_get), \
    SW_API_DEF(SW_API_HEADER_TYPE_SET, isisc_header_type_set),  \
    SW_API_DEF(SW_API_HEADER_TYPE_GET, isisc_header_type_get), \
    SW_API_DEF(SW_API_TXMAC_STATUS_SET, isisc_port_txmac_status_set), \
    SW_API_DEF(SW_API_TXMAC_STATUS_GET, isisc_port_txmac_status_get), \
    SW_API_DEF(SW_API_RXMAC_STATUS_SET, isisc_port_rxmac_status_set), \
    SW_API_DEF(SW_API_RXMAC_STATUS_GET, isisc_port_rxmac_status_get), \
    SW_API_DEF(SW_API_TXFC_STATUS_SET, isisc_port_txfc_status_set),   \
    SW_API_DEF(SW_API_TXFC_STATUS_GET, isisc_port_txfc_status_get),   \
    SW_API_DEF(SW_API_RXFC_STATUS_SET, isisc_port_rxfc_status_set),   \
    SW_API_DEF(SW_API_RXFC_STATUS_GET, isisc_port_rxfc_status_get),   \
    SW_API_DEF(SW_API_BP_STATUS_SET, isisc_port_bp_status_set),   \
    SW_API_DEF(SW_API_BP_STATUS_GET, isisc_port_bp_status_get),   \
    SW_API_DEF(SW_API_PT_LINK_MODE_SET, isisc_port_link_forcemode_set),   \
    SW_API_DEF(SW_API_PT_LINK_MODE_GET, isisc_port_link_forcemode_get), \
    SW_API_DEF(SW_API_PT_LINK_STATUS_GET, isisc_port_link_status_get), \
    SW_API_DEF(SW_API_PT_MAC_LOOPBACK_SET, isisc_port_mac_loopback_set), \
    SW_API_DEF(SW_API_PT_MAC_LOOPBACK_GET, isisc_port_mac_loopback_get), \
    SW_API_DEF(SW_API_PT_8023AZ_SET, isisc_port_8023az_set), \
    SW_API_DEF(SW_API_PT_8023AZ_GET, isisc_port_8023az_get),

#define PORTCONTROL_API_PARAM \
    SW_API_DESC(SW_API_PT_DUPLEX_GET) \
    SW_API_DESC(SW_API_PT_DUPLEX_SET) \
    SW_API_DESC(SW_API_PT_SPEED_GET)  \
    SW_API_DESC(SW_API_PT_SPEED_SET)  \
    SW_API_DESC(SW_API_PT_AN_GET)  \
    SW_API_DESC(SW_API_PT_AN_ENABLE)   \
    SW_API_DESC(SW_API_PT_AN_RESTART)  \
    SW_API_DESC(SW_API_PT_AN_ADV_GET)  \
    SW_API_DESC(SW_API_PT_AN_ADV_SET)  \
    SW_API_DESC(SW_API_PT_FLOWCTRL_SET)  \
    SW_API_DESC(SW_API_PT_FLOWCTRL_GET)  \
    SW_API_DESC(SW_API_PT_FLOWCTRL_MODE_SET)  \
    SW_API_DESC(SW_API_PT_FLOWCTRL_MODE_GET) \
    SW_API_DESC(SW_API_PT_POWERSAVE_SET) \
    SW_API_DESC(SW_API_PT_POWERSAVE_GET) \
    SW_API_DESC(SW_API_PT_HIBERNATE_SET) \
    SW_API_DESC(SW_API_PT_HIBERNATE_GET) \
    SW_API_DESC(SW_API_PT_CDT) \
    SW_API_DESC(SW_API_PT_TXHDR_SET) \
    SW_API_DESC(SW_API_PT_TXHDR_GET) \
    SW_API_DESC(SW_API_PT_RXHDR_SET) \
    SW_API_DESC(SW_API_PT_RXHDR_GET) \
    SW_API_DESC(SW_API_HEADER_TYPE_SET) \
    SW_API_DESC(SW_API_HEADER_TYPE_GET) \
    SW_API_DESC(SW_API_TXMAC_STATUS_SET) \
    SW_API_DESC(SW_API_TXMAC_STATUS_GET) \
    SW_API_DESC(SW_API_RXMAC_STATUS_SET) \
    SW_API_DESC(SW_API_RXMAC_STATUS_GET) \
    SW_API_DESC(SW_API_TXFC_STATUS_SET) \
    SW_API_DESC(SW_API_TXFC_STATUS_GET) \
    SW_API_DESC(SW_API_RXFC_STATUS_SET) \
    SW_API_DESC(SW_API_RXFC_STATUS_GET) \
    SW_API_DESC(SW_API_BP_STATUS_SET) \
    SW_API_DESC(SW_API_BP_STATUS_GET) \
    SW_API_DESC(SW_API_PT_LINK_MODE_SET) \
    SW_API_DESC(SW_API_PT_LINK_MODE_GET) \
    SW_API_DESC(SW_API_PT_LINK_STATUS_GET) \
    SW_API_DESC(SW_API_PT_MAC_LOOPBACK_SET) \
    SW_API_DESC(SW_API_PT_MAC_LOOPBACK_GET) \
    SW_API_DESC(SW_API_PT_8023AZ_SET) \
    SW_API_DESC(SW_API_PT_8023AZ_GET)
#else
#define PORTCONTROL_API
#define PORTCONTROL_API_PARAM
#endif

#ifdef IN_VLAN
#define VLAN_API \
    SW_API_DEF(SW_API_VLAN_ADD, isisc_vlan_create), \
    SW_API_DEF(SW_API_VLAN_DEL, isisc_vlan_delete), \
    SW_API_DEF(SW_API_VLAN_FIND, isisc_vlan_find), \
    SW_API_DEF(SW_API_VLAN_NEXT, isisc_vlan_next), \
    SW_API_DEF(SW_API_VLAN_APPEND, isisc_vlan_entry_append), \
    SW_API_DEF(SW_API_VLAN_FLUSH, isisc_vlan_flush), \
    SW_API_DEF(SW_API_VLAN_FID_SET, isisc_vlan_fid_set), \
    SW_API_DEF(SW_API_VLAN_FID_GET, isisc_vlan_fid_get), \
    SW_API_DEF(SW_API_VLAN_MEMBER_ADD, isisc_vlan_member_add), \
    SW_API_DEF(SW_API_VLAN_MEMBER_DEL, isisc_vlan_member_del), \
    SW_API_DEF(SW_API_VLAN_LEARN_STATE_SET, isisc_vlan_learning_state_set), \
    SW_API_DEF(SW_API_VLAN_LEARN_STATE_GET, isisc_vlan_learning_state_get),

#define VLAN_API_PARAM \
    SW_API_DESC(SW_API_VLAN_ADD)     \
    SW_API_DESC(SW_API_VLAN_DEL)     \
    SW_API_DESC(SW_API_VLAN_FIND)    \
    SW_API_DESC(SW_API_VLAN_NEXT)    \
    SW_API_DESC(SW_API_VLAN_APPEND)  \
    SW_API_DESC(SW_API_VLAN_FLUSH)   \
    SW_API_DESC(SW_API_VLAN_FID_SET) \
    SW_API_DESC(SW_API_VLAN_FID_GET) \
    SW_API_DESC(SW_API_VLAN_MEMBER_ADD) \
    SW_API_DESC(SW_API_VLAN_MEMBER_DEL) \
    SW_API_DESC(SW_API_VLAN_LEARN_STATE_SET) \
    SW_API_DESC(SW_API_VLAN_LEARN_STATE_GET)
#else
#define VLAN_API
#define VLAN_API_PARAM
#endif

#ifdef IN_PORTVLAN
#define PORTVLAN_API \
    SW_API_DEF(SW_API_PT_ING_MODE_GET, isisc_port_1qmode_get), \
    SW_API_DEF(SW_API_PT_ING_MODE_SET, isisc_port_1qmode_set), \
    SW_API_DEF(SW_API_PT_EG_MODE_GET, isisc_port_egvlanmode_get), \
    SW_API_DEF(SW_API_PT_EG_MODE_SET, isisc_port_egvlanmode_set), \
    SW_API_DEF(SW_API_PT_VLAN_MEM_ADD, isisc_portvlan_member_add), \
    SW_API_DEF(SW_API_PT_VLAN_MEM_DEL, isisc_portvlan_member_del), \
    SW_API_DEF(SW_API_PT_VLAN_MEM_UPDATE, isisc_portvlan_member_update), \
    SW_API_DEF(SW_API_PT_VLAN_MEM_GET, isisc_portvlan_member_get), \
    SW_API_DEF(SW_API_PT_FORCE_DEF_VID_SET, isisc_port_force_default_vid_set), \
    SW_API_DEF(SW_API_PT_FORCE_DEF_VID_GET, isisc_port_force_default_vid_get), \
    SW_API_DEF(SW_API_PT_FORCE_PORTVLAN_SET, isisc_port_force_portvlan_set), \
    SW_API_DEF(SW_API_PT_FORCE_PORTVLAN_GET, isisc_port_force_portvlan_get), \
    SW_API_DEF(SW_API_NESTVLAN_TPID_SET, isisc_nestvlan_tpid_set), \
    SW_API_DEF(SW_API_NESTVLAN_TPID_GET, isisc_nestvlan_tpid_get), \
    SW_API_DEF(SW_API_PT_IN_VLAN_MODE_SET, isisc_port_invlan_mode_set), \
    SW_API_DEF(SW_API_PT_IN_VLAN_MODE_GET, isisc_port_invlan_mode_get), \
    SW_API_DEF(SW_API_PT_TLS_SET, isisc_port_tls_set), \
    SW_API_DEF(SW_API_PT_TLS_GET, isisc_port_tls_get), \
    SW_API_DEF(SW_API_PT_PRI_PROPAGATION_SET, isisc_port_pri_propagation_set), \
    SW_API_DEF(SW_API_PT_PRI_PROPAGATION_GET, isisc_port_pri_propagation_get), \
    SW_API_DEF(SW_API_PT_DEF_SVID_SET, isisc_port_default_svid_set), \
    SW_API_DEF(SW_API_PT_DEF_SVID_GET, isisc_port_default_svid_get), \
    SW_API_DEF(SW_API_PT_DEF_CVID_SET, isisc_port_default_cvid_set), \
    SW_API_DEF(SW_API_PT_DEF_CVID_GET, isisc_port_default_cvid_get), \
    SW_API_DEF(SW_API_PT_VLAN_PROPAGATION_SET, isisc_port_vlan_propagation_set), \
    SW_API_DEF(SW_API_PT_VLAN_PROPAGATION_GET, isisc_port_vlan_propagation_get), \
    SW_API_DEF(SW_API_PT_VLAN_TRANS_ADD, isisc_port_vlan_trans_add), \
    SW_API_DEF(SW_API_PT_VLAN_TRANS_DEL, isisc_port_vlan_trans_del), \
    SW_API_DEF(SW_API_PT_VLAN_TRANS_GET, isisc_port_vlan_trans_get), \
    SW_API_DEF(SW_API_QINQ_MODE_SET, isisc_qinq_mode_set), \
    SW_API_DEF(SW_API_QINQ_MODE_GET, isisc_qinq_mode_get), \
    SW_API_DEF(SW_API_PT_QINQ_ROLE_SET, isisc_port_qinq_role_set), \
    SW_API_DEF(SW_API_PT_QINQ_ROLE_GET, isisc_port_qinq_role_get), \
    SW_API_DEF(SW_API_PT_VLAN_TRANS_ITERATE, isisc_port_vlan_trans_iterate), \
    SW_API_DEF(SW_API_PT_MAC_VLAN_XLT_SET, isisc_port_mac_vlan_xlt_set), \
    SW_API_DEF(SW_API_PT_MAC_VLAN_XLT_GET, isisc_port_mac_vlan_xlt_get), \
    SW_API_DEF(SW_API_NETISOLATE_SET, isisc_netisolate_set), \
    SW_API_DEF(SW_API_NETISOLATE_GET, isisc_netisolate_get),\
    SW_API_DEF(SW_API_EG_FLTR_BYPASS_EN_SET, isisc_eg_trans_filter_bypass_en_set), \
    SW_API_DEF(SW_API_EG_FLTR_BYPASS_EN_GET, isisc_eg_trans_filter_bypass_en_get),


#define PORTVLAN_API_PARAM \
    SW_API_DESC(SW_API_PT_ING_MODE_GET)  \
    SW_API_DESC(SW_API_PT_ING_MODE_SET)  \
    SW_API_DESC(SW_API_PT_EG_MODE_GET)  \
    SW_API_DESC(SW_API_PT_EG_MODE_SET)  \
    SW_API_DESC(SW_API_PT_VLAN_MEM_ADD) \
    SW_API_DESC(SW_API_PT_VLAN_MEM_DEL) \
    SW_API_DESC(SW_API_PT_VLAN_MEM_UPDATE)  \
    SW_API_DESC(SW_API_PT_VLAN_MEM_GET)     \
    SW_API_DESC(SW_API_PT_FORCE_DEF_VID_SET)    \
    SW_API_DESC(SW_API_PT_FORCE_DEF_VID_GET)    \
    SW_API_DESC(SW_API_PT_FORCE_PORTVLAN_SET)   \
    SW_API_DESC(SW_API_PT_FORCE_PORTVLAN_GET)   \
    SW_API_DESC(SW_API_NESTVLAN_TPID_SET)    \
    SW_API_DESC(SW_API_NESTVLAN_TPID_GET)    \
    SW_API_DESC(SW_API_PT_IN_VLAN_MODE_SET)    \
    SW_API_DESC(SW_API_PT_IN_VLAN_MODE_GET)    \
    SW_API_DESC(SW_API_PT_TLS_SET)    \
    SW_API_DESC(SW_API_PT_TLS_GET)    \
    SW_API_DESC(SW_API_PT_PRI_PROPAGATION_SET)    \
    SW_API_DESC(SW_API_PT_PRI_PROPAGATION_GET)    \
    SW_API_DESC(SW_API_PT_DEF_SVID_SET)    \
    SW_API_DESC(SW_API_PT_DEF_SVID_GET)    \
    SW_API_DESC(SW_API_PT_DEF_CVID_SET)    \
    SW_API_DESC(SW_API_PT_DEF_CVID_GET)    \
    SW_API_DESC(SW_API_PT_VLAN_PROPAGATION_SET)    \
    SW_API_DESC(SW_API_PT_VLAN_PROPAGATION_GET)    \
    SW_API_DESC(SW_API_PT_VLAN_TRANS_ADD)  \
    SW_API_DESC(SW_API_PT_VLAN_TRANS_DEL)  \
    SW_API_DESC(SW_API_PT_VLAN_TRANS_GET)  \
    SW_API_DESC(SW_API_QINQ_MODE_SET)      \
    SW_API_DESC(SW_API_QINQ_MODE_GET)      \
    SW_API_DESC(SW_API_PT_QINQ_ROLE_SET)   \
    SW_API_DESC(SW_API_PT_QINQ_ROLE_GET)   \
    SW_API_DESC(SW_API_PT_VLAN_TRANS_ITERATE) \
    SW_API_DESC(SW_API_PT_MAC_VLAN_XLT_SET)   \
    SW_API_DESC(SW_API_PT_MAC_VLAN_XLT_GET)  \
    SW_API_DESC(SW_API_NETISOLATE_SET)   \
    SW_API_DESC(SW_API_NETISOLATE_GET)   \
    SW_API_DESC(SW_API_EG_FLTR_BYPASS_EN_SET) \
    SW_API_DESC(SW_API_EG_FLTR_BYPASS_EN_GET)

#else
#define PORTVLAN_API
#define PORTVLAN_API_PARAM
#endif

#ifdef IN_FDB
#define FDB_API \
    SW_API_DEF(SW_API_FDB_ADD, isisc_fdb_add), \
    SW_API_DEF(SW_API_FDB_DELALL, isisc_fdb_del_all), \
    SW_API_DEF(SW_API_FDB_DELPORT,isisc_fdb_del_by_port), \
    SW_API_DEF(SW_API_FDB_DELMAC, isisc_fdb_del_by_mac), \
    SW_API_DEF(SW_API_FDB_FIND,   isisc_fdb_find), \
    SW_API_DEF(SW_API_FDB_EXTEND_NEXT,    isisc_fdb_extend_next),  \
    SW_API_DEF(SW_API_FDB_EXTEND_FIRST,   isisc_fdb_extend_first),  \
    SW_API_DEF(SW_API_FDB_TRANSFER,       isisc_fdb_transfer),  \
    SW_API_DEF(SW_API_FDB_PT_LEARN_SET,   isisc_fdb_port_learn_set), \
    SW_API_DEF(SW_API_FDB_PT_LEARN_GET,   isisc_fdb_port_learn_get), \
    SW_API_DEF(SW_API_FDB_AGE_CTRL_SET,   isisc_fdb_age_ctrl_set), \
    SW_API_DEF(SW_API_FDB_AGE_CTRL_GET,   isisc_fdb_age_ctrl_get), \
    SW_API_DEF(SW_API_FDB_VLAN_IVL_SVL_SET, isisc_fdb_vlan_ivl_svl_set),\
    SW_API_DEF(SW_API_FDB_VLAN_IVL_SVL_GET, isisc_fdb_vlan_ivl_svl_get),\
    SW_API_DEF(SW_API_FDB_AGE_TIME_SET,   isisc_fdb_age_time_set), \
    SW_API_DEF(SW_API_FDB_AGE_TIME_GET,   isisc_fdb_age_time_get), \
    SW_API_DEF(SW_API_PT_FDB_LEARN_LIMIT_SET,    isisc_port_fdb_learn_limit_set),  \
    SW_API_DEF(SW_API_PT_FDB_LEARN_LIMIT_GET,    isisc_port_fdb_learn_limit_get),  \
    SW_API_DEF(SW_API_PT_FDB_LEARN_EXCEED_CMD_SET,    isisc_port_fdb_learn_exceed_cmd_set),  \
    SW_API_DEF(SW_API_PT_FDB_LEARN_EXCEED_CMD_GET,    isisc_port_fdb_learn_exceed_cmd_get), \
    SW_API_DEF(SW_API_FDB_LEARN_LIMIT_SET,    isisc_fdb_learn_limit_set),  \
    SW_API_DEF(SW_API_FDB_LEARN_LIMIT_GET,    isisc_fdb_learn_limit_get),  \
    SW_API_DEF(SW_API_FDB_LEARN_EXCEED_CMD_SET,    isisc_fdb_learn_exceed_cmd_set),  \
    SW_API_DEF(SW_API_FDB_LEARN_EXCEED_CMD_GET,    isisc_fdb_learn_exceed_cmd_get),  \
    SW_API_DEF(SW_API_FDB_RESV_ADD, isisc_fdb_resv_add), \
    SW_API_DEF(SW_API_FDB_RESV_DEL, isisc_fdb_resv_del), \
    SW_API_DEF(SW_API_FDB_RESV_FIND, isisc_fdb_resv_find), \
    SW_API_DEF(SW_API_FDB_RESV_ITERATE, isisc_fdb_resv_iterate), \
    SW_API_DEF(SW_API_FDB_PT_LEARN_STATIC_SET,   isisc_fdb_port_learn_static_set), \
    SW_API_DEF(SW_API_FDB_PT_LEARN_STATIC_GET,   isisc_fdb_port_learn_static_get), \
    SW_API_DEF(SW_API_FDB_PORT_ADD,   isisc_fdb_port_add), \
    SW_API_DEF(SW_API_FDB_PORT_DEL,   isisc_fdb_port_del),

#define FDB_API_PARAM \
    SW_API_DESC(SW_API_FDB_ADD) \
    SW_API_DESC(SW_API_FDB_DELALL)  \
    SW_API_DESC(SW_API_FDB_DELPORT) \
    SW_API_DESC(SW_API_FDB_DELMAC) \
    SW_API_DESC(SW_API_FDB_FIND) \
    SW_API_DESC(SW_API_FDB_EXTEND_NEXT)  \
    SW_API_DESC(SW_API_FDB_EXTEND_FIRST) \
    SW_API_DESC(SW_API_FDB_TRANSFER) \
    SW_API_DESC(SW_API_FDB_PT_LEARN_SET) \
    SW_API_DESC(SW_API_FDB_PT_LEARN_GET) \
    SW_API_DESC(SW_API_FDB_AGE_CTRL_SET) \
    SW_API_DESC(SW_API_FDB_AGE_CTRL_GET) \
    SW_API_DESC(SW_API_FDB_VLAN_IVL_SVL_SET) \
    SW_API_DESC(SW_API_FDB_VLAN_IVL_SVL_GET) \
    SW_API_DESC(SW_API_FDB_AGE_TIME_SET) \
    SW_API_DESC(SW_API_FDB_AGE_TIME_GET) \
    SW_API_DESC(SW_API_PT_FDB_LEARN_LIMIT_SET)  \
    SW_API_DESC(SW_API_PT_FDB_LEARN_LIMIT_GET)  \
    SW_API_DESC(SW_API_PT_FDB_LEARN_EXCEED_CMD_SET)  \
    SW_API_DESC(SW_API_PT_FDB_LEARN_EXCEED_CMD_GET)  \
    SW_API_DESC(SW_API_FDB_LEARN_LIMIT_SET)  \
    SW_API_DESC(SW_API_FDB_LEARN_LIMIT_GET)  \
    SW_API_DESC(SW_API_FDB_LEARN_EXCEED_CMD_SET)  \
    SW_API_DESC(SW_API_FDB_LEARN_EXCEED_CMD_GET)  \
    SW_API_DESC(SW_API_FDB_RESV_ADD)  \
    SW_API_DESC(SW_API_FDB_RESV_DEL)  \
    SW_API_DESC(SW_API_FDB_RESV_FIND) \
    SW_API_DESC(SW_API_FDB_RESV_ITERATE) \
    SW_API_DESC(SW_API_FDB_PT_LEARN_STATIC_SET) \
    SW_API_DESC(SW_API_FDB_PT_LEARN_STATIC_GET) \
    SW_API_DESC(SW_API_FDB_PORT_ADD)  \
    SW_API_DESC(SW_API_FDB_PORT_DEL)

#else
#define FDB_API
#define FDB_API_PARAM
#endif


#ifdef IN_ACL
#define ACL_API \
    SW_API_DEF(SW_API_ACL_LIST_CREAT, isisc_acl_list_creat), \
    SW_API_DEF(SW_API_ACL_LIST_DESTROY, isisc_acl_list_destroy), \
    SW_API_DEF(SW_API_ACL_RULE_ADD, isisc_acl_rule_add), \
    SW_API_DEF(SW_API_ACL_RULE_DELETE, isisc_acl_rule_delete), \
    SW_API_DEF(SW_API_ACL_RULE_QUERY, isisc_acl_rule_query), \
    SW_API_DEF(SW_API_ACL_LIST_BIND, isisc_acl_list_bind), \
    SW_API_DEF(SW_API_ACL_LIST_UNBIND, isisc_acl_list_unbind), \
    SW_API_DEF(SW_API_ACL_STATUS_SET, isisc_acl_status_set), \
    SW_API_DEF(SW_API_ACL_STATUS_GET, isisc_acl_status_get), \
    SW_API_DEF(SW_API_ACL_LIST_DUMP, isisc_acl_list_dump), \
    SW_API_DEF(SW_API_ACL_RULE_DUMP, isisc_acl_rule_dump), \
    SW_API_DEF(SW_API_ACL_PT_UDF_PROFILE_SET, isisc_acl_port_udf_profile_set), \
    SW_API_DEF(SW_API_ACL_PT_UDF_PROFILE_GET, isisc_acl_port_udf_profile_get), \
    SW_API_DEF(SW_API_ACL_RULE_ACTIVE, isisc_acl_rule_active), \
    SW_API_DEF(SW_API_ACL_RULE_DEACTIVE, isisc_acl_rule_deactive),\
    SW_API_DEF(SW_API_ACL_RULE_SRC_FILTER_STS_SET, isisc_acl_rule_src_filter_sts_set),\
    SW_API_DEF(SW_API_ACL_RULE_SRC_FILTER_STS_GET, isisc_acl_rule_src_filter_sts_get), \
    SW_API_DEF(SW_API_ACL_RULE_GET_OFFSET, isisc_acl_rule_get_offset),

#define ACL_API_PARAM \
    SW_API_DESC(SW_API_ACL_LIST_CREAT)   \
    SW_API_DESC(SW_API_ACL_LIST_DESTROY) \
    SW_API_DESC(SW_API_ACL_RULE_ADD)     \
    SW_API_DESC(SW_API_ACL_RULE_DELETE)  \
    SW_API_DESC(SW_API_ACL_RULE_QUERY)   \
    SW_API_DESC(SW_API_ACL_LIST_BIND)    \
    SW_API_DESC(SW_API_ACL_LIST_UNBIND)  \
    SW_API_DESC(SW_API_ACL_STATUS_SET)   \
    SW_API_DESC(SW_API_ACL_STATUS_GET)   \
    SW_API_DESC(SW_API_ACL_LIST_DUMP)    \
    SW_API_DESC(SW_API_ACL_RULE_DUMP)    \
    SW_API_DESC(SW_API_ACL_PT_UDF_PROFILE_SET)    \
    SW_API_DESC(SW_API_ACL_PT_UDF_PROFILE_GET)    \
    SW_API_DESC(SW_API_ACL_RULE_ACTIVE)    \
    SW_API_DESC(SW_API_ACL_RULE_DEACTIVE) \
    SW_API_DESC(SW_API_ACL_RULE_SRC_FILTER_STS_SET) \
    SW_API_DESC(SW_API_ACL_RULE_SRC_FILTER_STS_GET)
#else
#define ACL_API
#define ACL_API_PARAM
#endif


#ifdef IN_QOS
#define QOS_API \
    SW_API_DEF(SW_API_QOS_QU_TX_BUF_ST_SET, isisc_qos_queue_tx_buf_status_set), \
    SW_API_DEF(SW_API_QOS_QU_TX_BUF_ST_GET, isisc_qos_queue_tx_buf_status_get), \
    SW_API_DEF(SW_API_QOS_QU_TX_BUF_NR_SET, isisc_qos_queue_tx_buf_nr_set), \
    SW_API_DEF(SW_API_QOS_QU_TX_BUF_NR_GET, isisc_qos_queue_tx_buf_nr_get), \
    SW_API_DEF(SW_API_QOS_PT_TX_BUF_ST_SET, isisc_qos_port_tx_buf_status_set), \
    SW_API_DEF(SW_API_QOS_PT_TX_BUF_ST_GET, isisc_qos_port_tx_buf_status_get), \
    SW_API_DEF(SW_API_QOS_PT_RED_EN_SET, isisc_qos_port_red_en_set),\
    SW_API_DEF(SW_API_QOS_PT_RED_EN_GET, isisc_qos_port_red_en_get),\
    SW_API_DEF(SW_API_QOS_PT_TX_BUF_NR_SET, isisc_qos_port_tx_buf_nr_set), \
    SW_API_DEF(SW_API_QOS_PT_TX_BUF_NR_GET, isisc_qos_port_tx_buf_nr_get), \
    SW_API_DEF(SW_API_QOS_PT_RX_BUF_NR_SET, isisc_qos_port_rx_buf_nr_set), \
    SW_API_DEF(SW_API_QOS_PT_RX_BUF_NR_GET, isisc_qos_port_rx_buf_nr_get), \
    SW_API_DEF(SW_API_QOS_PT_MODE_SET, isisc_qos_port_mode_set), \
    SW_API_DEF(SW_API_QOS_PT_MODE_GET, isisc_qos_port_mode_get), \
    SW_API_DEF(SW_API_QOS_PT_MODE_PRI_SET, isisc_qos_port_mode_pri_set), \
    SW_API_DEF(SW_API_QOS_PT_MODE_PRI_GET, isisc_qos_port_mode_pri_get), \
    SW_API_DEF(SW_API_QOS_PORT_SCH_MODE_SET, isisc_qos_port_sch_mode_set), \
    SW_API_DEF(SW_API_QOS_PORT_SCH_MODE_GET, isisc_qos_port_sch_mode_get), \
    SW_API_DEF(SW_API_QOS_PT_DEF_SPRI_SET, isisc_qos_port_default_spri_set), \
    SW_API_DEF(SW_API_QOS_PT_DEF_SPRI_GET, isisc_qos_port_default_spri_get), \
    SW_API_DEF(SW_API_QOS_PT_DEF_CPRI_SET, isisc_qos_port_default_cpri_set), \
    SW_API_DEF(SW_API_QOS_PT_DEF_CPRI_GET, isisc_qos_port_default_cpri_get), \
    SW_API_DEF(SW_API_QOS_PT_FORCE_SPRI_ST_SET, isisc_qos_port_force_spri_status_set), \
    SW_API_DEF(SW_API_QOS_PT_FORCE_SPRI_ST_GET, isisc_qos_port_force_spri_status_get), \
    SW_API_DEF(SW_API_QOS_PT_FORCE_CPRI_ST_SET, isisc_qos_port_force_cpri_status_set), \
    SW_API_DEF(SW_API_QOS_PT_FORCE_CPRI_ST_GET, isisc_qos_port_force_cpri_status_get), \
    SW_API_DEF(SW_API_QOS_QUEUE_REMARK_SET, isisc_qos_queue_remark_table_set), \
    SW_API_DEF(SW_API_QOS_QUEUE_REMARK_GET, isisc_qos_queue_remark_table_get),


#define QOS_API_PARAM \
    SW_API_DESC(SW_API_QOS_QU_TX_BUF_ST_SET) \
    SW_API_DESC(SW_API_QOS_QU_TX_BUF_ST_GET) \
    SW_API_DESC(SW_API_QOS_QU_TX_BUF_NR_SET) \
    SW_API_DESC(SW_API_QOS_QU_TX_BUF_NR_GET) \
    SW_API_DESC(SW_API_QOS_PT_TX_BUF_ST_SET) \
    SW_API_DESC(SW_API_QOS_PT_TX_BUF_ST_GET) \
    SW_API_DESC(SW_API_QOS_PT_RED_EN_SET) \
    SW_API_DESC(SW_API_QOS_PT_RED_EN_GET) \
    SW_API_DESC(SW_API_QOS_PT_TX_BUF_NR_SET) \
    SW_API_DESC(SW_API_QOS_PT_TX_BUF_NR_GET) \
    SW_API_DESC(SW_API_QOS_PT_RX_BUF_NR_SET) \
    SW_API_DESC(SW_API_QOS_PT_RX_BUF_NR_GET) \
    SW_API_DESC(SW_API_QOS_PT_MODE_SET) \
    SW_API_DESC(SW_API_QOS_PT_MODE_GET) \
    SW_API_DESC(SW_API_QOS_PT_MODE_PRI_SET) \
    SW_API_DESC(SW_API_QOS_PT_MODE_PRI_GET) \
    SW_API_DESC(SW_API_QOS_PORT_DEF_UP_SET) \
    SW_API_DESC(SW_API_QOS_PORT_DEF_UP_GET) \
    SW_API_DESC(SW_API_QOS_PORT_SCH_MODE_SET) \
    SW_API_DESC(SW_API_QOS_PORT_SCH_MODE_GET) \
    SW_API_DESC(SW_API_QOS_PT_DEF_SPRI_SET)  \
    SW_API_DESC(SW_API_QOS_PT_DEF_SPRI_GET)  \
    SW_API_DESC(SW_API_QOS_PT_DEF_CPRI_SET)  \
    SW_API_DESC(SW_API_QOS_PT_DEF_CPRI_GET)  \
    SW_API_DESC(SW_API_QOS_PT_FORCE_SPRI_ST_SET)  \
    SW_API_DESC(SW_API_QOS_PT_FORCE_SPRI_ST_GET)  \
    SW_API_DESC(SW_API_QOS_PT_FORCE_CPRI_ST_SET)  \
    SW_API_DESC(SW_API_QOS_PT_FORCE_CPRI_ST_GET)  \
    SW_API_DESC(SW_API_QOS_QUEUE_REMARK_SET) \
    SW_API_DESC(SW_API_QOS_QUEUE_REMARK_GET)
#else
#define QOS_API
#define QOS_API_PARAM
#endif


#ifdef IN_IGMP
#define IGMP_API \
    SW_API_DEF(SW_API_PT_IGMPS_MODE_SET, isisc_port_igmps_status_set), \
    SW_API_DEF(SW_API_PT_IGMPS_MODE_GET, isisc_port_igmps_status_get), \
    SW_API_DEF(SW_API_IGMP_MLD_CMD_SET, isisc_igmp_mld_cmd_set), \
    SW_API_DEF(SW_API_IGMP_MLD_CMD_GET, isisc_igmp_mld_cmd_get), \
    SW_API_DEF(SW_API_IGMP_PT_JOIN_SET, isisc_port_igmp_mld_join_set), \
    SW_API_DEF(SW_API_IGMP_PT_JOIN_GET, isisc_port_igmp_mld_join_get), \
    SW_API_DEF(SW_API_IGMP_PT_LEAVE_SET, isisc_port_igmp_mld_leave_set), \
    SW_API_DEF(SW_API_IGMP_PT_LEAVE_GET, isisc_port_igmp_mld_leave_get), \
    SW_API_DEF(SW_API_IGMP_RP_SET, isisc_igmp_mld_rp_set), \
    SW_API_DEF(SW_API_IGMP_RP_GET, isisc_igmp_mld_rp_get), \
    SW_API_DEF(SW_API_IGMP_ENTRY_CREAT_SET, isisc_igmp_mld_entry_creat_set), \
    SW_API_DEF(SW_API_IGMP_ENTRY_CREAT_GET, isisc_igmp_mld_entry_creat_get), \
    SW_API_DEF(SW_API_IGMP_ENTRY_STATIC_SET, isisc_igmp_mld_entry_static_set), \
    SW_API_DEF(SW_API_IGMP_ENTRY_STATIC_GET, isisc_igmp_mld_entry_static_get), \
    SW_API_DEF(SW_API_IGMP_ENTRY_LEAKY_SET, isisc_igmp_mld_entry_leaky_set), \
    SW_API_DEF(SW_API_IGMP_ENTRY_LEAKY_GET, isisc_igmp_mld_entry_leaky_get), \
    SW_API_DEF(SW_API_IGMP_ENTRY_V3_SET, isisc_igmp_mld_entry_v3_set), \
    SW_API_DEF(SW_API_IGMP_ENTRY_V3_GET, isisc_igmp_mld_entry_v3_get), \
    SW_API_DEF(SW_API_IGMP_ENTRY_QUEUE_SET, isisc_igmp_mld_entry_queue_set), \
    SW_API_DEF(SW_API_IGMP_ENTRY_QUEUE_GET, isisc_igmp_mld_entry_queue_get), \
    SW_API_DEF(SW_API_PT_IGMP_LEARN_LIMIT_SET,    isisc_port_igmp_mld_learn_limit_set),  \
    SW_API_DEF(SW_API_PT_IGMP_LEARN_LIMIT_GET,    isisc_port_igmp_mld_learn_limit_get),  \
    SW_API_DEF(SW_API_PT_IGMP_LEARN_EXCEED_CMD_SET,    isisc_port_igmp_mld_learn_exceed_cmd_set),  \
    SW_API_DEF(SW_API_PT_IGMP_LEARN_EXCEED_CMD_GET,    isisc_port_igmp_mld_learn_exceed_cmd_get), \
    SW_API_DEF(SW_API_IGMP_SG_ENTRY_SET,    isisc_igmp_sg_entry_set),  \
    SW_API_DEF(SW_API_IGMP_SG_ENTRY_CLEAR,    isisc_igmp_sg_entry_clear),  \
    SW_API_DEF(SW_API_IGMP_SG_ENTRY_SHOW,    isisc_igmp_sg_entry_show),

#define IGMP_API_PARAM \
    SW_API_DESC(SW_API_PT_IGMPS_MODE_SET) \
    SW_API_DESC(SW_API_PT_IGMPS_MODE_GET) \
    SW_API_DESC(SW_API_IGMP_MLD_CMD_SET)  \
    SW_API_DESC(SW_API_IGMP_MLD_CMD_GET)  \
    SW_API_DESC(SW_API_IGMP_PT_JOIN_SET)  \
    SW_API_DESC(SW_API_IGMP_PT_JOIN_GET)  \
    SW_API_DESC(SW_API_IGMP_PT_LEAVE_SET) \
    SW_API_DESC(SW_API_IGMP_PT_LEAVE_GET) \
    SW_API_DESC(SW_API_IGMP_RP_SET)  \
    SW_API_DESC(SW_API_IGMP_RP_GET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_CREAT_SET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_CREAT_GET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_STATIC_SET) \
    SW_API_DESC(SW_API_IGMP_ENTRY_STATIC_GET) \
    SW_API_DESC(SW_API_IGMP_ENTRY_LEAKY_SET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_LEAKY_GET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_V3_SET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_V3_GET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_QUEUE_SET)  \
    SW_API_DESC(SW_API_IGMP_ENTRY_QUEUE_GET) \
    SW_API_DESC(SW_API_PT_IGMP_LEARN_LIMIT_SET) \
    SW_API_DESC(SW_API_PT_IGMP_LEARN_LIMIT_GET) \
    SW_API_DESC(SW_API_PT_IGMP_LEARN_EXCEED_CMD_SET) \
    SW_API_DESC(SW_API_PT_IGMP_LEARN_EXCEED_CMD_GET) \
    SW_API_DESC(SW_API_IGMP_SG_ENTRY_SET) \
    SW_API_DESC(SW_API_IGMP_SG_ENTRY_CLEAR) \
    SW_API_DESC(SW_API_IGMP_SG_ENTRY_SHOW)
#else
#define IGMP_API
#define IGMP_API_PARAM
#endif


#ifdef IN_LEAKY
#define LEAKY_API \
    SW_API_DEF(SW_API_UC_LEAKY_MODE_SET, isisc_uc_leaky_mode_set), \
    SW_API_DEF(SW_API_UC_LEAKY_MODE_GET, isisc_uc_leaky_mode_get), \
    SW_API_DEF(SW_API_MC_LEAKY_MODE_SET, isisc_mc_leaky_mode_set), \
    SW_API_DEF(SW_API_MC_LEAKY_MODE_GET, isisc_mc_leaky_mode_get), \
    SW_API_DEF(SW_API_ARP_LEAKY_MODE_SET, isisc_port_arp_leaky_set), \
    SW_API_DEF(SW_API_ARP_LEAKY_MODE_GET, isisc_port_arp_leaky_get), \
    SW_API_DEF(SW_API_PT_UC_LEAKY_MODE_SET, isisc_port_uc_leaky_set), \
    SW_API_DEF(SW_API_PT_UC_LEAKY_MODE_GET, isisc_port_uc_leaky_get), \
    SW_API_DEF(SW_API_PT_MC_LEAKY_MODE_SET, isisc_port_mc_leaky_set), \
    SW_API_DEF(SW_API_PT_MC_LEAKY_MODE_GET, isisc_port_mc_leaky_get),

#define LEAKY_API_PARAM \
    SW_API_DESC(SW_API_UC_LEAKY_MODE_SET)  \
    SW_API_DESC(SW_API_UC_LEAKY_MODE_GET)  \
    SW_API_DESC(SW_API_MC_LEAKY_MODE_SET)  \
    SW_API_DESC(SW_API_MC_LEAKY_MODE_GET)  \
    SW_API_DESC(SW_API_ARP_LEAKY_MODE_SET) \
    SW_API_DESC(SW_API_ARP_LEAKY_MODE_GET) \
    SW_API_DESC(SW_API_PT_UC_LEAKY_MODE_SET)  \
    SW_API_DESC(SW_API_PT_UC_LEAKY_MODE_GET)  \
    SW_API_DESC(SW_API_PT_MC_LEAKY_MODE_SET)  \
    SW_API_DESC(SW_API_PT_MC_LEAKY_MODE_GET)
#else
#define LEAKY_API
#define LEAKY_API_PARAM
#endif


#ifdef IN_MIRROR
#define MIRROR_API \
    SW_API_DEF(SW_API_MIRROR_ANALY_PT_SET, isisc_mirr_analysis_port_set), \
    SW_API_DEF(SW_API_MIRROR_ANALY_PT_GET, isisc_mirr_analysis_port_get), \
    SW_API_DEF(SW_API_MIRROR_IN_PT_SET, isisc_mirr_port_in_set), \
    SW_API_DEF(SW_API_MIRROR_IN_PT_GET, isisc_mirr_port_in_get), \
    SW_API_DEF(SW_API_MIRROR_EG_PT_SET, isisc_mirr_port_eg_set), \
    SW_API_DEF(SW_API_MIRROR_EG_PT_GET, isisc_mirr_port_eg_get),

#define MIRROR_API_PARAM \
    SW_API_DESC(SW_API_MIRROR_ANALY_PT_SET)  \
    SW_API_DESC(SW_API_MIRROR_ANALY_PT_GET)  \
    SW_API_DESC(SW_API_MIRROR_IN_PT_SET)  \
    SW_API_DESC(SW_API_MIRROR_IN_PT_GET)  \
    SW_API_DESC(SW_API_MIRROR_EG_PT_SET)  \
    SW_API_DESC(SW_API_MIRROR_EG_PT_GET)
#else
#define MIRROR_API
#define MIRROR_API_PARAM
#endif


#ifdef IN_RATE
#define RATE_API \
    SW_API_DEF(SW_API_RATE_PORT_POLICER_SET, isisc_rate_port_policer_set), \
    SW_API_DEF(SW_API_RATE_PORT_POLICER_GET, isisc_rate_port_policer_get), \
    SW_API_DEF(SW_API_RATE_PORT_SHAPER_SET, isisc_rate_port_shaper_set), \
    SW_API_DEF(SW_API_RATE_PORT_SHAPER_GET, isisc_rate_port_shaper_get), \
    SW_API_DEF(SW_API_RATE_QUEUE_SHAPER_SET, isisc_rate_queue_shaper_set), \
    SW_API_DEF(SW_API_RATE_QUEUE_SHAPER_GET, isisc_rate_queue_shaper_get), \
    SW_API_DEF(SW_API_RATE_ACL_POLICER_SET, isisc_rate_acl_policer_set), \
    SW_API_DEF(SW_API_RATE_ACL_POLICER_GET, isisc_rate_acl_policer_get), \
    SW_API_DEF(SW_API_RATE_PT_ADDRATEBYTE_SET, isisc_rate_port_add_rate_byte_set), \
    SW_API_DEF(SW_API_RATE_PT_ADDRATEBYTE_GET, isisc_rate_port_add_rate_byte_get), \
    SW_API_DEF(SW_API_RATE_PT_GOL_FLOW_EN_SET, isisc_rate_port_gol_flow_en_set), \
    SW_API_DEF(SW_API_RATE_PT_GOL_FLOW_EN_GET, isisc_rate_port_gol_flow_en_get),

#define RATE_API_PARAM \
    SW_API_DESC(SW_API_RATE_PORT_POLICER_SET) \
    SW_API_DESC(SW_API_RATE_PORT_POLICER_GET) \
    SW_API_DESC(SW_API_RATE_PORT_SHAPER_SET) \
    SW_API_DESC(SW_API_RATE_PORT_SHAPER_GET) \
    SW_API_DESC(SW_API_RATE_QUEUE_SHAPER_SET) \
    SW_API_DESC(SW_API_RATE_QUEUE_SHAPER_GET) \
    SW_API_DESC(SW_API_RATE_ACL_POLICER_SET) \
    SW_API_DESC(SW_API_RATE_ACL_POLICER_GET) \
    SW_API_DESC(SW_API_RATE_PT_ADDRATEBYTE_SET) \
    SW_API_DESC(SW_API_RATE_PT_ADDRATEBYTE_GET) \
    SW_API_DESC(SW_API_RATE_PT_GOL_FLOW_EN_SET) \
    SW_API_DESC(SW_API_RATE_PT_GOL_FLOW_EN_GET)
#else
#define RATE_API
#define RATE_API_PARAM
#endif


#ifdef IN_STP
#define STP_API \
    SW_API_DEF(SW_API_STP_PT_STATE_SET, isisc_stp_port_state_set), \
    SW_API_DEF(SW_API_STP_PT_STATE_GET, isisc_stp_port_state_get),

#define STP_API_PARAM \
    SW_API_DESC(SW_API_STP_PT_STATE_SET) \
    SW_API_DESC(SW_API_STP_PT_STATE_GET)
#else
#define STP_API
#define STP_API_PARAM
#endif


#ifdef IN_MIB
#define MIB_API \
    SW_API_DEF(SW_API_PT_MIB_GET, isisc_get_mib_info), \
    SW_API_DEF(SW_API_MIB_STATUS_SET, isisc_mib_status_set), \
    SW_API_DEF(SW_API_MIB_STATUS_GET, isisc_mib_status_get), \
    SW_API_DEF(SW_API_PT_MIB_FLUSH_COUNTERS, isisc_mib_port_flush_counters), \
    SW_API_DEF(SW_API_MIB_CPU_KEEP_SET, isisc_mib_cpukeep_set), \
    SW_API_DEF(SW_API_MIB_CPU_KEEP_GET, isisc_mib_cpukeep_get),


#define MIB_API_PARAM \
    SW_API_DESC(SW_API_PT_MIB_GET)  \
    SW_API_DESC(SW_API_MIB_STATUS_SET) \
    SW_API_DESC(SW_API_MIB_STATUS_GET)\
    SW_API_DESC(SW_API_PT_MIB_FLUSH_COUNTERS) \
    SW_API_DESC(SW_API_MIB_CPU_KEEP_SET) \
    SW_API_DESC(SW_API_MIB_CPU_KEEP_GET)
#else
#define MIB_API
#define MIB_API_PARAM
#endif


#ifdef IN_MISC
#define MISC_API \
    SW_API_DEF(SW_API_FRAME_MAX_SIZE_SET, isisc_frame_max_size_set), \
    SW_API_DEF(SW_API_FRAME_MAX_SIZE_GET, isisc_frame_max_size_get), \
    SW_API_DEF(SW_API_PT_UNK_UC_FILTER_SET, isisc_port_unk_uc_filter_set), \
    SW_API_DEF(SW_API_PT_UNK_UC_FILTER_GET, isisc_port_unk_uc_filter_get), \
    SW_API_DEF(SW_API_PT_UNK_MC_FILTER_SET, isisc_port_unk_mc_filter_set), \
    SW_API_DEF(SW_API_PT_UNK_MC_FILTER_GET, isisc_port_unk_mc_filter_get), \
    SW_API_DEF(SW_API_PT_BC_FILTER_SET, isisc_port_bc_filter_set), \
    SW_API_DEF(SW_API_PT_BC_FILTER_GET, isisc_port_bc_filter_get), \
    SW_API_DEF(SW_API_CPU_PORT_STATUS_SET, isisc_cpu_port_status_set), \
    SW_API_DEF(SW_API_CPU_PORT_STATUS_GET, isisc_cpu_port_status_get), \
    SW_API_DEF(SW_API_PPPOE_CMD_SET, isisc_pppoe_cmd_set), \
    SW_API_DEF(SW_API_PPPOE_CMD_GET, isisc_pppoe_cmd_get), \
    SW_API_DEF(SW_API_PPPOE_STATUS_SET, isisc_pppoe_status_set), \
    SW_API_DEF(SW_API_PPPOE_STATUS_GET, isisc_pppoe_status_get), \
    SW_API_DEF(SW_API_PT_DHCP_SET, isisc_port_dhcp_set), \
    SW_API_DEF(SW_API_PT_DHCP_GET, isisc_port_dhcp_get), \
    SW_API_DEF(SW_API_ARP_CMD_SET, isisc_arp_cmd_set), \
    SW_API_DEF(SW_API_ARP_CMD_GET, isisc_arp_cmd_get), \
    SW_API_DEF(SW_API_EAPOL_CMD_SET, isisc_eapol_cmd_set), \
    SW_API_DEF(SW_API_EAPOL_CMD_GET, isisc_eapol_cmd_get), \
    SW_API_DEF(SW_API_EAPOL_STATUS_SET, isisc_eapol_status_set), \
    SW_API_DEF(SW_API_EAPOL_STATUS_GET, isisc_eapol_status_get), \
    SW_API_DEF(SW_API_RIPV1_STATUS_SET, isisc_ripv1_status_set), \
    SW_API_DEF(SW_API_RIPV1_STATUS_GET, isisc_ripv1_status_get), \
    SW_API_DEF(SW_API_PT_ARP_REQ_STATUS_SET, isisc_port_arp_req_status_set), \
    SW_API_DEF(SW_API_PT_ARP_REQ_STATUS_GET, isisc_port_arp_req_status_get), \
    SW_API_DEF(SW_API_PT_ARP_ACK_STATUS_SET, isisc_port_arp_ack_status_set), \
    SW_API_DEF(SW_API_PT_ARP_ACK_STATUS_GET, isisc_port_arp_ack_status_get), \
    SW_API_DEF(SW_API_PPPOE_SESSION_TABLE_ADD, isisc_pppoe_session_table_add), \
    SW_API_DEF(SW_API_PPPOE_SESSION_TABLE_DEL, isisc_pppoe_session_table_del), \
    SW_API_DEF(SW_API_PPPOE_SESSION_TABLE_GET, isisc_pppoe_session_table_get), \
    SW_API_DEF(SW_API_PPPOE_SESSION_ID_SET, isisc_pppoe_session_id_set), \
    SW_API_DEF(SW_API_PPPOE_SESSION_ID_GET, isisc_pppoe_session_id_get), \
    SW_API_DEF(SW_API_INTR_MASK_SET, isisc_intr_mask_set), \
    SW_API_DEF(SW_API_INTR_MASK_GET, isisc_intr_mask_get), \
    SW_API_DEF(SW_API_INTR_STATUS_GET, isisc_intr_status_get),   \
    SW_API_DEF(SW_API_INTR_STATUS_CLEAR, isisc_intr_status_clear), \
    SW_API_DEF(SW_API_INTR_PORT_LINK_MASK_SET, isisc_intr_port_link_mask_set), \
    SW_API_DEF(SW_API_INTR_PORT_LINK_MASK_GET, isisc_intr_port_link_mask_get), \
    SW_API_DEF(SW_API_INTR_PORT_LINK_STATUS_GET, isisc_intr_port_link_status_get),\
    SW_API_DEF(SW_API_INTR_MASK_MAC_LINKCHG_SET, isisc_intr_mask_mac_linkchg_set), \
    SW_API_DEF(SW_API_INTR_MASK_MAC_LINKCHG_GET, isisc_intr_mask_mac_linkchg_get), \
    SW_API_DEF(SW_API_INTR_STATUS_MAC_LINKCHG_GET, isisc_intr_status_mac_linkchg_get), \
        SW_API_DEF(SW_API_INTR_STATUS_MAC_LINKCHG_CLEAR, isisc_intr_status_mac_linkchg_clear), \
    SW_API_DEF(SW_API_CPU_VID_EN_SET, isisc_cpu_vid_en_set), \
    SW_API_DEF(SW_API_CPU_VID_EN_GET, isisc_cpu_vid_en_get), \
    SW_API_DEF(SW_API_RTD_PPPOE_EN_SET, isisc_rtd_pppoe_en_set), \
    SW_API_DEF(SW_API_RTD_PPPOE_EN_GET, isisc_rtd_pppoe_en_get),

#define MISC_API_PARAM \
    SW_API_DESC(SW_API_FRAME_MAX_SIZE_SET)  \
    SW_API_DESC(SW_API_FRAME_MAX_SIZE_GET)  \
    SW_API_DESC(SW_API_PT_UNK_UC_FILTER_SET)  \
    SW_API_DESC(SW_API_PT_UNK_UC_FILTER_GET)  \
    SW_API_DESC(SW_API_PT_UNK_MC_FILTER_SET)  \
    SW_API_DESC(SW_API_PT_UNK_MC_FILTER_GET)  \
    SW_API_DESC(SW_API_PT_BC_FILTER_SET)  \
    SW_API_DESC(SW_API_PT_BC_FILTER_GET)  \
    SW_API_DESC(SW_API_CPU_PORT_STATUS_SET)  \
    SW_API_DESC(SW_API_CPU_PORT_STATUS_GET)  \
    SW_API_DESC(SW_API_PPPOE_CMD_SET)  \
    SW_API_DESC(SW_API_PPPOE_CMD_GET)  \
    SW_API_DESC(SW_API_PPPOE_STATUS_SET)  \
    SW_API_DESC(SW_API_PPPOE_STATUS_GET)  \
    SW_API_DESC(SW_API_PT_DHCP_SET)  \
    SW_API_DESC(SW_API_PT_DHCP_GET)  \
    SW_API_DESC(SW_API_ARP_CMD_SET)  \
    SW_API_DESC(SW_API_ARP_CMD_GET)  \
    SW_API_DESC(SW_API_EAPOL_CMD_SET) \
    SW_API_DESC(SW_API_EAPOL_CMD_GET) \
    SW_API_DESC(SW_API_PPPOE_SESSION_ADD)  \
    SW_API_DESC(SW_API_PPPOE_SESSION_DEL)  \
    SW_API_DESC(SW_API_PPPOE_SESSION_GET)  \
    SW_API_DESC(SW_API_EAPOL_STATUS_SET)  \
    SW_API_DESC(SW_API_EAPOL_STATUS_GET)  \
    SW_API_DESC(SW_API_RIPV1_STATUS_SET)  \
    SW_API_DESC(SW_API_RIPV1_STATUS_GET) \
    SW_API_DESC(SW_API_PT_ARP_REQ_STATUS_SET) \
    SW_API_DESC(SW_API_PT_ARP_REQ_STATUS_GET) \
    SW_API_DESC(SW_API_PT_ARP_ACK_STATUS_SET) \
    SW_API_DESC(SW_API_PT_ARP_ACK_STATUS_GET) \
    SW_API_DESC(SW_API_PPPOE_SESSION_TABLE_ADD) \
    SW_API_DESC(SW_API_PPPOE_SESSION_TABLE_DEL) \
    SW_API_DESC(SW_API_PPPOE_SESSION_TABLE_GET) \
    SW_API_DESC(SW_API_PPPOE_SESSION_ID_SET) \
    SW_API_DESC(SW_API_PPPOE_SESSION_ID_GET) \
    SW_API_DESC(SW_API_INTR_MASK_SET) \
    SW_API_DESC(SW_API_INTR_MASK_GET) \
    SW_API_DESC(SW_API_INTR_STATUS_GET)   \
    SW_API_DESC(SW_API_INTR_STATUS_CLEAR) \
    SW_API_DESC(SW_API_INTR_PORT_LINK_MASK_SET) \
    SW_API_DESC(SW_API_INTR_PORT_LINK_MASK_GET) \
    SW_API_DESC(SW_API_INTR_PORT_LINK_STATUS_GET) \
     SW_API_DESC(SW_API_INTR_MASK_MAC_LINKCHG_SET) \
    SW_API_DESC(SW_API_INTR_MASK_MAC_LINKCHG_GET) \
    SW_API_DESC(SW_API_INTR_STATUS_MAC_LINKCHG_GET) \
    SW_API_DESC(SW_API_INTR_STATUS_MAC_LINKCHG_CLEAR) \
    SW_API_DESC(SW_API_CPU_VID_EN_SET)  \
    SW_API_DESC(SW_API_CPU_VID_EN_GET)  \
    SW_API_DESC(SW_API_RTD_PPPOE_EN_SET)  \
    SW_API_DESC(SW_API_RTD_PPPOE_EN_GET)

#else
#define MISC_API
#define MISC_API_PARAM
#endif


#ifdef IN_LED
#define LED_API \
    SW_API_DEF(SW_API_LED_PATTERN_SET, isisc_led_ctrl_pattern_set), \
    SW_API_DEF(SW_API_LED_PATTERN_GET, isisc_led_ctrl_pattern_get),

#define LED_API_PARAM \
    SW_API_DESC(SW_API_LED_PATTERN_SET) \
    SW_API_DESC(SW_API_LED_PATTERN_GET)
#else
#define LED_API
#define LED_API_PARAM
#endif

#ifdef IN_COSMAP
#define COSMAP_API \
    SW_API_DEF(SW_API_COSMAP_DSCP_TO_PRI_SET, isisc_cosmap_dscp_to_pri_set), \
    SW_API_DEF(SW_API_COSMAP_DSCP_TO_PRI_GET, isisc_cosmap_dscp_to_pri_get), \
    SW_API_DEF(SW_API_COSMAP_DSCP_TO_DP_SET, isisc_cosmap_dscp_to_dp_set), \
    SW_API_DEF(SW_API_COSMAP_DSCP_TO_DP_GET, isisc_cosmap_dscp_to_dp_get), \
    SW_API_DEF(SW_API_COSMAP_UP_TO_PRI_SET, isisc_cosmap_up_to_pri_set), \
    SW_API_DEF(SW_API_COSMAP_UP_TO_PRI_GET, isisc_cosmap_up_to_pri_get), \
    SW_API_DEF(SW_API_COSMAP_UP_TO_DP_SET, isisc_cosmap_up_to_dp_set), \
    SW_API_DEF(SW_API_COSMAP_UP_TO_DP_GET, isisc_cosmap_up_to_dp_get), \
    SW_API_DEF(SW_API_COSMAP_PRI_TO_QU_SET, isisc_cosmap_pri_to_queue_set), \
    SW_API_DEF(SW_API_COSMAP_PRI_TO_QU_GET, isisc_cosmap_pri_to_queue_get), \
    SW_API_DEF(SW_API_COSMAP_PRI_TO_EHQU_SET, isisc_cosmap_pri_to_ehqueue_set), \
    SW_API_DEF(SW_API_COSMAP_PRI_TO_EHQU_GET, isisc_cosmap_pri_to_ehqueue_get), \
    SW_API_DEF(SW_API_COSMAP_EG_REMARK_SET, isisc_cosmap_egress_remark_set), \
    SW_API_DEF(SW_API_COSMAP_EG_REMARK_GET, isisc_cosmap_egress_remark_get),

#define COSMAP_API_PARAM \
    SW_API_DESC(SW_API_COSMAP_DSCP_TO_PRI_SET)  \
    SW_API_DESC(SW_API_COSMAP_DSCP_TO_PRI_GET)  \
    SW_API_DESC(SW_API_COSMAP_DSCP_TO_DP_SET)  \
    SW_API_DESC(SW_API_COSMAP_DSCP_TO_DP_GET)  \
    SW_API_DESC(SW_API_COSMAP_UP_TO_PRI_SET)  \
    SW_API_DESC(SW_API_COSMAP_UP_TO_PRI_GET)  \
    SW_API_DESC(SW_API_COSMAP_UP_TO_DP_SET)  \
    SW_API_DESC(SW_API_COSMAP_UP_TO_DP_GET)  \
    SW_API_DESC(SW_API_COSMAP_PRI_TO_QU_SET)  \
    SW_API_DESC(SW_API_COSMAP_PRI_TO_QU_GET)  \
    SW_API_DESC(SW_API_COSMAP_PRI_TO_EHQU_SET) \
    SW_API_DESC(SW_API_COSMAP_PRI_TO_EHQU_GET) \
    SW_API_DESC(SW_API_COSMAP_EG_REMARK_SET) \
    SW_API_DESC(SW_API_COSMAP_EG_REMARK_GET)
#else
#define COSMAP_API
#define COSMAP_API_PARAM
#endif

#ifdef IN_SEC
#define SEC_API \
    SW_API_DEF(SW_API_SEC_NORM_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_NORM_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_MAC_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_MAC_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_IP_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_IP_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_IP4_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_IP4_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_IP6_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_IP6_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_TCP_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_TCP_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_UDP_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_UDP_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_ICMP4_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_ICMP4_GET, isisc_sec_norm_item_get), \
    SW_API_DEF(SW_API_SEC_ICMP6_SET, isisc_sec_norm_item_set), \
    SW_API_DEF(SW_API_SEC_ICMP6_GET, isisc_sec_norm_item_get),

#define SEC_API_PARAM \
    SW_API_DESC(SW_API_SEC_NORM_SET) \
    SW_API_DESC(SW_API_SEC_NORM_GET) \
    SW_API_DESC(SW_API_SEC_MAC_SET) \
    SW_API_DESC(SW_API_SEC_MAC_GET) \
    SW_API_DESC(SW_API_SEC_IP_SET) \
    SW_API_DESC(SW_API_SEC_IP_GET) \
    SW_API_DESC(SW_API_SEC_IP4_SET) \
    SW_API_DESC(SW_API_SEC_IP4_GET) \
    SW_API_DESC(SW_API_SEC_IP6_SET) \
    SW_API_DESC(SW_API_SEC_IP6_GET) \
    SW_API_DESC(SW_API_SEC_TCP_SET) \
    SW_API_DESC(SW_API_SEC_TCP_GET) \
    SW_API_DESC(SW_API_SEC_UDP_SET) \
    SW_API_DESC(SW_API_SEC_UDP_GET) \
    SW_API_DESC(SW_API_SEC_ICMP4_SET) \
    SW_API_DESC(SW_API_SEC_ICMP4_GET) \
    SW_API_DESC(SW_API_SEC_ICMP6_SET) \
    SW_API_DESC(SW_API_SEC_ICMP6_GET)
#else
#define SEC_API
#define SEC_API_PARAM
#endif

#ifdef IN_IP
#define IP_API \
    SW_API_DEF(SW_API_IP_HOST_ADD, isisc_ip_host_add), \
    SW_API_DEF(SW_API_IP_HOST_DEL, isisc_ip_host_del), \
    SW_API_DEF(SW_API_IP_HOST_GET, isisc_ip_host_get), \
    SW_API_DEF(SW_API_IP_HOST_NEXT, isisc_ip_host_next), \
    SW_API_DEF(SW_API_IP_HOST_COUNTER_BIND, isisc_ip_host_counter_bind), \
    SW_API_DEF(SW_API_IP_HOST_PPPOE_BIND, isisc_ip_host_pppoe_bind), \
    SW_API_DEF(SW_API_IP_PT_ARP_LEARN_SET, isisc_ip_pt_arp_learn_set), \
    SW_API_DEF(SW_API_IP_PT_ARP_LEARN_GET, isisc_ip_pt_arp_learn_get), \
    SW_API_DEF(SW_API_IP_ARP_LEARN_SET, isisc_ip_arp_learn_set), \
    SW_API_DEF(SW_API_IP_ARP_LEARN_GET, isisc_ip_arp_learn_get), \
    SW_API_DEF(SW_API_IP_SOURCE_GUARD_SET, isisc_ip_source_guard_set), \
    SW_API_DEF(SW_API_IP_SOURCE_GUARD_GET, isisc_ip_source_guard_get), \
    SW_API_DEF(SW_API_IP_ARP_GUARD_SET, isisc_ip_arp_guard_set), \
    SW_API_DEF(SW_API_IP_ARP_GUARD_GET, isisc_ip_arp_guard_get), \
    SW_API_DEF(SW_API_IP_ROUTE_STATUS_SET, isisc_ip_route_status_set), \
    SW_API_DEF(SW_API_IP_ROUTE_STATUS_GET, isisc_ip_route_status_get), \
    SW_API_DEF(SW_API_IP_INTF_ENTRY_ADD, isisc_ip_intf_entry_add), \
    SW_API_DEF(SW_API_IP_INTF_ENTRY_DEL, isisc_ip_intf_entry_del), \
    SW_API_DEF(SW_API_IP_INTF_ENTRY_NEXT, isisc_ip_intf_entry_next), \
    SW_API_DEF(SW_API_IP_UNK_SOURCE_CMD_SET, isisc_ip_unk_source_cmd_set),  \
    SW_API_DEF(SW_API_IP_UNK_SOURCE_CMD_GET, isisc_ip_unk_source_cmd_get), \
    SW_API_DEF(SW_API_ARP_UNK_SOURCE_CMD_SET, isisc_arp_unk_source_cmd_set), \
    SW_API_DEF(SW_API_ARP_UNK_SOURCE_CMD_GET, isisc_arp_unk_source_cmd_get), \
    SW_API_DEF(SW_API_IP_AGE_TIME_SET, isisc_ip_age_time_set), \
    SW_API_DEF(SW_API_IP_AGE_TIME_GET, isisc_ip_age_time_get), \
    SW_API_DEF(SW_API_WCMP_HASH_MODE_SET, isisc_ip_wcmp_hash_mode_set), \
    SW_API_DEF(SW_API_WCMP_HASH_MODE_GET, isisc_ip_wcmp_hash_mode_get),

#define IP_API_PARAM \
    SW_API_DESC(SW_API_IP_HOST_ADD) \
    SW_API_DESC(SW_API_IP_HOST_DEL) \
    SW_API_DESC(SW_API_IP_HOST_GET) \
    SW_API_DESC(SW_API_IP_HOST_NEXT) \
    SW_API_DESC(SW_API_IP_HOST_COUNTER_BIND) \
    SW_API_DESC(SW_API_IP_HOST_PPPOE_BIND) \
    SW_API_DESC(SW_API_IP_PT_ARP_LEARN_SET) \
    SW_API_DESC(SW_API_IP_PT_ARP_LEARN_GET) \
    SW_API_DESC(SW_API_IP_ARP_LEARN_SET) \
    SW_API_DESC(SW_API_IP_ARP_LEARN_GET) \
    SW_API_DESC(SW_API_IP_SOURCE_GUARD_SET) \
    SW_API_DESC(SW_API_IP_SOURCE_GUARD_GET) \
    SW_API_DESC(SW_API_IP_ARP_GUARD_SET) \
    SW_API_DESC(SW_API_IP_ARP_GUARD_GET) \
    SW_API_DESC(SW_API_IP_ROUTE_STATUS_SET) \
    SW_API_DESC(SW_API_IP_ROUTE_STATUS_GET) \
    SW_API_DESC(SW_API_IP_INTF_ENTRY_ADD)  \
    SW_API_DESC(SW_API_IP_INTF_ENTRY_DEL)  \
    SW_API_DESC(SW_API_IP_INTF_ENTRY_NEXT) \
    SW_API_DESC(SW_API_IP_UNK_SOURCE_CMD_SET)  \
    SW_API_DESC(SW_API_IP_UNK_SOURCE_CMD_GET)  \
    SW_API_DESC(SW_API_ARP_UNK_SOURCE_CMD_SET) \
    SW_API_DESC(SW_API_ARP_UNK_SOURCE_CMD_GET) \
    SW_API_DESC(SW_API_IP_AGE_TIME_SET) \
    SW_API_DESC(SW_API_IP_AGE_TIME_GET) \
    SW_API_DESC(SW_API_WCMP_HASH_MODE_SET) \
    SW_API_DESC(SW_API_WCMP_HASH_MODE_GET)

#else
#define IP_API
#define IP_API_PARAM
#endif

#ifdef IN_NAT
#define NAT_API \
    SW_API_DEF(SW_API_NAT_ADD, isisc_nat_add), \
    SW_API_DEF(SW_API_NAT_DEL, isisc_nat_del), \
    SW_API_DEF(SW_API_NAT_GET, isisc_nat_get), \
    SW_API_DEF(SW_API_NAT_NEXT, isisc_nat_next), \
    SW_API_DEF(SW_API_NAT_COUNTER_BIND, isisc_nat_counter_bind), \
    SW_API_DEF(SW_API_NAPT_ADD, isisc_napt_add), \
    SW_API_DEF(SW_API_NAPT_DEL, isisc_napt_del), \
    SW_API_DEF(SW_API_NAPT_GET, isisc_napt_get), \
    SW_API_DEF(SW_API_NAPT_NEXT, isisc_napt_next), \
    SW_API_DEF(SW_API_NAPT_COUNTER_BIND, isisc_napt_counter_bind), \
    SW_API_DEF(SW_API_NAT_STATUS_SET, isisc_nat_status_set), \
    SW_API_DEF(SW_API_NAT_STATUS_GET, isisc_nat_status_get), \
    SW_API_DEF(SW_API_NAT_HASH_MODE_SET, isisc_nat_hash_mode_set), \
    SW_API_DEF(SW_API_NAT_HASH_MODE_GET, isisc_nat_hash_mode_get), \
    SW_API_DEF(SW_API_NAPT_STATUS_SET, isisc_napt_status_set), \
    SW_API_DEF(SW_API_NAPT_STATUS_GET, isisc_napt_status_get), \
    SW_API_DEF(SW_API_NAPT_MODE_SET, isisc_napt_mode_set), \
    SW_API_DEF(SW_API_NAPT_MODE_GET, isisc_napt_mode_get), \
    SW_API_DEF(SW_API_PRV_BASE_ADDR_SET, isisc_nat_prv_base_addr_set), \
    SW_API_DEF(SW_API_PRV_BASE_ADDR_GET, isisc_nat_prv_base_addr_get), \
    SW_API_DEF(SW_API_PUB_ADDR_ENTRY_ADD, isisc_nat_pub_addr_add), \
    SW_API_DEF(SW_API_PUB_ADDR_ENTRY_DEL, isisc_nat_pub_addr_del), \
    SW_API_DEF(SW_API_PUB_ADDR_ENTRY_NEXT, isisc_nat_pub_addr_next), \
    SW_API_DEF(SW_API_NAT_UNK_SESSION_CMD_SET, isisc_nat_unk_session_cmd_set), \
    SW_API_DEF(SW_API_NAT_UNK_SESSION_CMD_GET, isisc_nat_unk_session_cmd_get), \
    SW_API_DEF(SW_API_PRV_BASE_MASK_SET, isisc_nat_prv_base_mask_set), \
    SW_API_DEF(SW_API_PRV_BASE_MASK_GET, isisc_nat_prv_base_mask_get), \
    SW_API_DEF(SW_API_NAT_GLOBAL_SET, isisc_nat_global_set),

#define NAT_API_PARAM \
    SW_API_DESC(SW_API_NAT_ADD) \
    SW_API_DESC(SW_API_NAT_DEL) \
    SW_API_DESC(SW_API_NAT_GET) \
    SW_API_DESC(SW_API_NAT_NEXT) \
    SW_API_DESC(SW_API_NAT_COUNTER_BIND) \
    SW_API_DESC(SW_API_NAPT_ADD) \
    SW_API_DESC(SW_API_NAPT_DEL) \
    SW_API_DESC(SW_API_NAPT_GET) \
    SW_API_DESC(SW_API_NAPT_NEXT) \
    SW_API_DESC(SW_API_NAPT_COUNTER_BIND) \
    SW_API_DESC(SW_API_NAT_STATUS_SET) \
    SW_API_DESC(SW_API_NAT_STATUS_GET) \
    SW_API_DESC(SW_API_NAT_HASH_MODE_SET) \
    SW_API_DESC(SW_API_NAT_HASH_MODE_GET) \
    SW_API_DESC(SW_API_NAPT_STATUS_SET) \
    SW_API_DESC(SW_API_NAPT_STATUS_GET) \
    SW_API_DESC(SW_API_NAPT_MODE_SET) \
    SW_API_DESC(SW_API_NAPT_MODE_GET) \
    SW_API_DESC(SW_API_PRV_BASE_ADDR_SET) \
    SW_API_DESC(SW_API_PRV_BASE_ADDR_GET) \
    SW_API_DESC(SW_API_PUB_ADDR_ENTRY_ADD) \
    SW_API_DESC(SW_API_PUB_ADDR_ENTRY_DEL) \
    SW_API_DESC(SW_API_PUB_ADDR_ENTRY_NEXT) \
    SW_API_DESC(SW_API_NAT_UNK_SESSION_CMD_SET) \
    SW_API_DESC(SW_API_NAT_UNK_SESSION_CMD_GET) \
    SW_API_DESC(SW_API_PRV_BASE_MASK_SET) \
    SW_API_DESC(SW_API_PRV_BASE_MASK_GET) \
    SW_API_DESC(SW_API_NAT_GLOBAL_SET)
#else
#define NAT_API
#define NAT_API_PARAM
#endif

#ifdef IN_TRUNK
#define TRUNK_API \
    SW_API_DEF(SW_API_TRUNK_GROUP_SET, isisc_trunk_group_set), \
    SW_API_DEF(SW_API_TRUNK_GROUP_GET, isisc_trunk_group_get), \
    SW_API_DEF(SW_API_TRUNK_HASH_SET, isisc_trunk_hash_mode_set), \
    SW_API_DEF(SW_API_TRUNK_HASH_GET, isisc_trunk_hash_mode_get), \
    SW_API_DEF(SW_API_TRUNK_MAN_SA_SET, isisc_trunk_manipulate_sa_set), \
    SW_API_DEF(SW_API_TRUNK_MAN_SA_GET, isisc_trunk_manipulate_sa_get),

#define TRUNK_API_PARAM \
    SW_API_DESC(SW_API_TRUNK_GROUP_SET) \
    SW_API_DESC(SW_API_TRUNK_GROUP_GET) \
    SW_API_DESC(SW_API_TRUNK_HASH_SET)  \
    SW_API_DESC(SW_API_TRUNK_HASH_GET)  \
    SW_API_DESC(SW_API_TRUNK_MAN_SA_SET)\
    SW_API_DESC(SW_API_TRUNK_MAN_SA_GET)
#else
#define TRUNK_API
#define TRUNK_API_PARAM
#endif

#ifdef IN_INTERFACECONTROL
#define INTERFACECTRL_API \
    SW_API_DEF(SW_API_MAC_MODE_SET, isisc_interface_mac_mode_set), \
    SW_API_DEF(SW_API_MAC_MODE_GET, isisc_interface_mac_mode_get), \
    SW_API_DEF(SW_API_PHY_MODE_SET, isisc_interface_phy_mode_set), \
    SW_API_DEF(SW_API_PHY_MODE_GET, isisc_interface_phy_mode_get), \
    SW_API_DEF(SW_API_FX100_CTRL_SET, isisc_interface_fx100_ctrl_set), \
    SW_API_DEF(SW_API_FX100_CTRL_GET, isisc_interface_fx100_ctrl_get), \
    SW_API_DEF(SW_API_FX100_STATUS_GET, isisc_interface_fx100_status_get), \
    SW_API_DEF(SW_API_MAC06_EXCH_SET, isisc_interface_mac06_exch_set), \
    SW_API_DEF(SW_API_MAC06_EXCH_GET, isisc_interface_mac06_exch_get),

#define INTERFACECTRL_API_PARAM \
    SW_API_DESC(SW_API_MAC_MODE_SET)  \
    SW_API_DESC(SW_API_MAC_MODE_GET)  \
    SW_API_DESC(SW_API_PHY_MODE_SET)  \
    SW_API_DESC(SW_API_PHY_MODE_GET)  \
    SW_API_DESC(SW_API_FX100_CTRL_SET)  \
    SW_API_DESC(SW_API_FX100_CTRL_GET)  \
    SW_API_DESC(SW_API_FX100_STATUS_GET) \
    SW_API_DESC(SW_API_MAC06_EXCH_SET) \
    SW_API_DESC(SW_API_MAC06_EXCH_GET)

#else
#define INTERFACECTRL_API
#define INTERFACECTRL_API_PARAM
#endif

#define REG_API \
    SW_API_DEF(SW_API_PHY_GET, isisc_phy_get), \
    SW_API_DEF(SW_API_PHY_SET, isisc_phy_set), \
    SW_API_DEF(SW_API_REG_GET, isisc_reg_get), \
    SW_API_DEF(SW_API_REG_SET, isisc_reg_set), \
    SW_API_DEF(SW_API_REG_FIELD_GET, isisc_reg_field_get), \
    SW_API_DEF(SW_API_REG_FIELD_SET, isisc_reg_field_set), \
    SW_API_DEF(SW_API_REG_DUMP, isisc_regsiter_dump), \
    SW_API_DEF(SW_API_DBG_REG_DUMP, isisc_debug_regsiter_dump),

#define REG_API_PARAM \
    SW_API_DESC(SW_API_PHY_GET) \
    SW_API_DESC(SW_API_PHY_SET) \
    SW_API_DESC(SW_API_REG_GET) \
    SW_API_DESC(SW_API_REG_SET) \
    SW_API_DESC(SW_API_REG_FIELD_GET) \
    SW_API_DESC(SW_API_REG_FIELD_SET) \
    SW_API_DESC(SW_API_REG_DUMP)  \
    SW_API_DESC(SW_API_DBG_REG_DUMP)

#define SSDK_API \
    SW_API_DEF(SW_API_SWITCH_RESET, isisc_reset), \
    SW_API_DEF(SW_API_SSDK_CFG, hsl_ssdk_cfg), \
    PORTCONTROL_API \
    VLAN_API \
    PORTVLAN_API \
    FDB_API    \
    ACL_API    \
    QOS_API    \
    IGMP_API   \
    LEAKY_API  \
    MIRROR_API \
    RATE_API \
    STP_API  \
    MIB_API  \
    MISC_API \
    LED_API  \
    COSMAP_API \
    SEC_API  \
    IP_API \
    NAT_API \
    TRUNK_API \
    INTERFACECTRL_API \
    REG_API  \
    SW_API_DEF(SW_API_MAX, NULL),


#define SSDK_PARAM  \
    SW_PARAM_DEF(SW_API_SWITCH_RESET, SW_UINT32, 4, SW_PARAM_IN, "Dev ID"), \
    SW_PARAM_DEF(SW_API_SSDK_CFG, SW_UINT32, 4, SW_PARAM_IN, "Dev ID"), \
    SW_PARAM_DEF(SW_API_SSDK_CFG, SW_SSDK_CFG, sizeof(ssdk_cfg_t), SW_PARAM_PTR|SW_PARAM_OUT, "ssdk configuration"), \
    MIB_API_PARAM    \
    LEAKY_API_PARAM  \
    MISC_API_PARAM   \
    IGMP_API_PARAM   \
    MIRROR_API_PARAM \
    PORTCONTROL_API_PARAM \
    PORTVLAN_API_PARAM    \
    VLAN_API_PARAM \
    FDB_API_PARAM  \
    QOS_API_PARAM  \
    RATE_API_PARAM \
    STP_API_PARAM  \
    ACL_API_PARAM  \
    LED_API_PARAM  \
    COSMAP_API_PARAM \
    SEC_API_PARAM  \
    IP_API_PARAM \
    NAT_API_PARAM \
    TRUNK_API_PARAM \
    INTERFACECTRL_API_PARAM \
    REG_API_PARAM  \
    SW_PARAM_DEF(SW_API_MAX, SW_UINT32, 4, SW_PARAM_IN, "Dev ID"),

#if (defined(USER_MODE) && defined(KERNEL_MODULE))
#undef SSDK_API
#undef SSDK_PARAM

#define SSDK_API \
    REG_API \
    SW_API_DEF(SW_API_MAX, NULL),

#define SSDK_PARAM  \
    REG_API_PARAM \
    SW_PARAM_DEF(SW_API_MAX, SW_UINT32, 4, SW_PARAM_IN, "Dev ID"),
#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* _ISISC_API_H_ */
