/*
 * ndpi_typedefs.h
 *
 * Copyright (C) 2011-25 - ntop.org
 *
 * This file is part of nDPI, an open source deep packet inspection
 * library based on the OpenDPI and PACE technology by ipoque GmbH
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __NDPI_TYPEDEFS_H__
#define __NDPI_TYPEDEFS_H__

#define BT_ANNOUNCE
#define SNAP_EXT

#ifdef __KERNEL__
#undef HAVE_HYPERSCAN
#endif

#ifndef NDPI_CFFI_PREPROCESSING
#ifdef USE_GLOBAL_CONTEXT
#include "ndpi_config.h"
#define HAVE_STRUCT_TIMESPEC
#ifndef __KERNEL__
#include <pthread.h>
#endif
#endif
#endif

#include "ndpi_define.h"
#ifndef NDPI_CFFI_PREPROCESSING
#include "ndpi_includes.h"
#endif
#include "ndpi_protocol_ids.h"

/* Used by both nDPI core and patricia code under third-party */
#ifdef __KERNEL__
#include <linux/in6.h>
#endif
#include "ndpi_patricia_typedefs.h"

#ifndef __KERNEL__
#ifndef NDPI_CFFI_PREPROCESSING
#ifndef u_char
typedef unsigned char u_char;
#endif

#ifndef u_short
typedef unsigned short u_short;
#endif

#ifndef u_int
typedef unsigned int u_int;
#endif
#endif
#endif /* __KERNEL__ */

#ifdef __APPLE__
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
#endif

/* NDPI_LOG_LEVEL */
typedef enum {
	      NDPI_LOG_ERROR,
	      NDPI_LOG_TRACE,
	      NDPI_LOG_DEBUG,
	      NDPI_LOG_DEBUG_EXTRA
} ndpi_log_level_t;

typedef enum {
  ndpi_multimedia_unknown_flow          = 0x00,
  ndpi_multimedia_audio_flow            = 0x01,
  ndpi_multimedia_video_flow            = 0x02,
  ndpi_multimedia_screen_sharing_flow   = 0x04,
} ndpi_multimedia_flow_type;

typedef enum {
  ndpi_l4_proto_unknown = 0,
  ndpi_l4_proto_tcp_only,
  ndpi_l4_proto_udp_only,
  ndpi_l4_proto_tcp_and_udp,
} ndpi_l4_proto_info;

typedef enum {
  ndpi_no_tunnel = 0,
  ndpi_gtp_tunnel,
  ndpi_capwap_tunnel,
  ndpi_tzsp_tunnel,
  ndpi_l2tp_tunnel,
  ndpi_vxlan_tunnel,
  ndpi_gre_tunnel,
} ndpi_packet_tunnel;

/*
  NOTE
  When the typedef below is modified don't forget to update
  - nDPI/wireshark/ndpi.lua
  - ndpi_risk2str, ndpi_risk2code, ndpi_code2risk, ndpi_risk_shortnames (in ndpi_utils.c)
  - doc/flow_risks.rst
  - ndpi_known_risks (ndpi_main.c)

  To make sure the risk is also seen by ntopng:
  1. Add a new flow alert key to the enum FlowAlertTypeEnum in include/ntop_typedefs.h
  2. Add the very same flow alert key to the table flow_alert_keys in scripts/lua/modules/alert_keys/flow_alert_keys.lua
  3. Add the risk to the array risk_enum_to_alert_type in src/FlowRiskAlerts.cpp

  Example: https://github.com/ntop/ntopng/commit/aecc1e3e6505a0522439dbb2b295a3703d3d0f9a
 */
typedef enum {
  NDPI_NO_RISK = 0,
  NDPI_URL_POSSIBLE_XSS,
  NDPI_URL_POSSIBLE_SQL_INJECTION,
  NDPI_URL_POSSIBLE_RCE_INJECTION,
  NDPI_BINARY_APPLICATION_TRANSFER,
  NDPI_KNOWN_PROTOCOL_ON_NON_STANDARD_PORT,
  NDPI_TLS_SELFSIGNED_CERTIFICATE,
  NDPI_TLS_OBSOLETE_VERSION,
  NDPI_TLS_WEAK_CIPHER,
  NDPI_TLS_CERTIFICATE_EXPIRED,
  NDPI_TLS_CERTIFICATE_MISMATCH, /* 10 */
  NDPI_HTTP_SUSPICIOUS_USER_AGENT,
  NDPI_NUMERIC_IP_HOST,
  NDPI_HTTP_SUSPICIOUS_URL,
  NDPI_HTTP_SUSPICIOUS_HEADER,
  NDPI_TLS_NOT_CARRYING_HTTPS,
  NDPI_SUSPICIOUS_DGA_DOMAIN,
  NDPI_MALFORMED_PACKET,
  NDPI_SSH_OBSOLETE_CLIENT_VERSION_OR_CIPHER,
  NDPI_SSH_OBSOLETE_SERVER_VERSION_OR_CIPHER,
  NDPI_SMB_INSECURE_VERSION, /* 20 */
  NDPI_FREE_21,                                          /* FREE */
  NDPI_UNSAFE_PROTOCOL,
  NDPI_DNS_SUSPICIOUS_TRAFFIC,
  NDPI_TLS_MISSING_SNI,
  NDPI_HTTP_SUSPICIOUS_CONTENT,
  NDPI_RISKY_ASN,
  NDPI_RISKY_DOMAIN,
  NDPI_MALICIOUS_FINGERPRINT,
  NDPI_MALICIOUS_SHA1_CERTIFICATE,
  NDPI_DESKTOP_OR_FILE_SHARING_SESSION, /* 30 */
  NDPI_TLS_UNCOMMON_ALPN,
  NDPI_TLS_CERT_VALIDITY_TOO_LONG,
  NDPI_TLS_SUSPICIOUS_EXTENSION,
  NDPI_TLS_FATAL_ALERT,
  NDPI_SUSPICIOUS_ENTROPY,
  NDPI_CLEAR_TEXT_CREDENTIALS,
  NDPI_DNS_LARGE_PACKET,
  NDPI_DNS_FRAGMENTED,
  NDPI_INVALID_CHARACTERS,
  NDPI_POSSIBLE_EXPLOIT, /* 40 */ /* Log4J, Wordpress and other exploits */
  NDPI_TLS_CERTIFICATE_ABOUT_TO_EXPIRE,
  NDPI_PUNYCODE_IDN, /* https://en.wikipedia.org/wiki/Punycode */
  NDPI_ERROR_CODE_DETECTED,
  NDPI_HTTP_CRAWLER_BOT,
  NDPI_ANONYMOUS_SUBSCRIBER,
  NDPI_UNIDIRECTIONAL_TRAFFIC, /* NOTE: as nDPI can detect a protocol with one packet, make sure
				  your app will clear this risk if future packets (not sent to nDPI)
				  are received in the opposite direction */
  NDPI_HTTP_OBSOLETE_SERVER,
  NDPI_PERIODIC_FLOW,          /* Set in case a flow repeats at a specific pace [used by apps on top of nDPI] */
  NDPI_MINOR_ISSUES,           /* Generic packet issues (e.g. DNS with 0 TTL) */
  NDPI_TCP_ISSUES,    /* 50 */ /* TCP issues such as connection failed or scan */
  NDPI_FREE_51,                                         /* FREE */
  NDPI_TLS_ALPN_SNI_MISMATCH,  /* Invalid ALPN/SNI combination */
  NDPI_MALWARE_HOST_CONTACTED, /* Flow client contacted a malware host */
  NDPI_BINARY_DATA_TRANSFER,   /* Attempt to transfer something in binary format */
  NDPI_PROBING_ATTEMPT,        /* Probing attempt (e.g. TCP connection with no data exchanged or unidirection traffic for bidirectional flows such as SSH) */
  NDPI_OBFUSCATED_TRAFFIC,
  /* Before allocating a new risk here, check if there are FREE entries above */

  /* Leave this as last member */
  NDPI_MAX_RISK /* must be <= 63 due to (**) */
} ndpi_risk_enum;

typedef u_int64_t ndpi_risk; /* (**) */

/*Used mainly by configuration */
//extern const char *ndpi_risk_shortnames[NDPI_MAX_RISK];

typedef enum {
  NDPI_PARAM_HOSTNAME  /* char* */,
  NDPI_PARAM_ISSUER_DN /* char* */,
  NDPI_PARAM_HOST_IPV4 /* u_int32_t* */, /* Network byte order */

  /*
    IMPORTANT
    please update ndpi_check_flow_risk_exceptions()
    (in ndpi_utils.c) whenever you add a new parameter
  */

  /* Leave this as last member */
  NDPI_MAX_RISK_PARAM_ID
} ndpi_risk_param_id;

typedef struct {
  ndpi_risk_param_id id;
  void *value; /* char* for strings, u_int32_t* for IPv4 addresses */
} ndpi_risk_params;

typedef enum {
  NDPI_RISK_LOW,
  NDPI_RISK_MEDIUM,
  NDPI_RISK_HIGH,
  NDPI_RISK_SEVERE,
  NDPI_RISK_CRITICAL,
  NDPI_RISK_EMERGENCY
} ndpi_risk_severity;

typedef enum {
  NDPI_SCORE_RISK_LOW       =  10,
  NDPI_SCORE_RISK_MEDIUM    =  50,
  NDPI_SCORE_RISK_HIGH      = 100,
  NDPI_SCORE_RISK_SEVERE    = 150,
  NDPI_SCORE_RISK_CRITICAL  = 200,
  NDPI_SCORE_RISK_EMERGENCY = 250,
} ndpi_risk_score;

typedef enum {
  CLIENT_NO_RISK_PERCENTAGE   =   0, /* 100% server risk */
  CLIENT_LOW_RISK_PERCENTAGE  =  10, /* 90%  server risk */
  CLIENT_FAIR_RISK_PERCENTAGE =  50, /* 50%  server risk */
  CLIENT_HIGH_RISK_PERCENTAGE =  90, /* 10%  server risk */
  CLIENT_FULL_RISK_PERCENTAGE = 100  /* 0%   server risk */
} risk_percentage;

typedef enum {
  NDPI_NO_ACCOUNTABILITY = 0,
  NDPI_CLIENT_ACCOUNTABLE, /* flow client triggered the risk */
  NDPI_SERVER_ACCOUNTABLE, /* flow server triggered the risk */
  NDPI_BOTH_ACCOUNTABLE    /* Both actors are responsible */
} ndpi_risk_accountability;

typedef struct {
  ndpi_risk_enum risk;
  ndpi_risk_severity severity;
  risk_percentage default_client_risk_pctg; /* 0-100 */
  ndpi_risk_accountability accountability;
} ndpi_risk_info;

/* NDPI_VISIT */
typedef enum {
   ndpi_preorder,
   ndpi_postorder,
   ndpi_endorder,
   ndpi_leaf
} ndpi_VISIT;

typedef enum {
  NDPI_CFG_INVALID_CONTEXT = -1,
  NDPI_CFG_NOT_FOUND = -2,
  NDPI_CFG_INVALID_PARAM = -3,
  NDPI_CFG_CONTEXT_ALREADY_INITIALIZED = -4,
  NDPI_CFG_CALLBACK_ERROR = -5,

  NDPI_CFG_OK = 0,
} ndpi_cfg_error;

/* NDPI_MASK_SIZE */
typedef u_int32_t ndpi_ndpi_mask;

#define MAX_NUM_RISK_INFOS    8

/* NDPI_PROTO_BITMASK_STRUCT */
#ifdef NDPI_CFFI_PREPROCESSING
#undef NDPI_NUM_FDS_BITS
#define NDPI_NUM_FDS_BITS     16
#endif

typedef struct ndpi_protocol_bitmask_struct {
  ndpi_ndpi_mask fds_bits[NDPI_NUM_FDS_BITS];
} ndpi_protocol_bitmask_struct_t;


#ifdef NDPI_CFFI_PREPROCESSING
#undef NDPI_NUM_FDS_BITS_DISSECTORS
#define NDPI_NUM_FDS_BITS_DISSECTORS    9
#endif

typedef struct ndpi_dissector_bitmask_struct {
  ndpi_ndpi_mask fds_bits[NDPI_NUM_FDS_BITS_DISSECTORS];
} ndpi_dissector_bitmask_struct_t;

struct ndpi_detection_module_struct;

/* NDPI_DEBUG_FUNCTION_PTR (cast) */
typedef void (*ndpi_debug_function_ptr) (u_int16_t protocol, struct ndpi_detection_module_struct *module_struct,
					 ndpi_log_level_t log_level, const char *file,
					 const char *func, unsigned line,
					 const char *format, ...);

#ifndef NDPI_CFFI_PREPROCESSING_EXCLUDE_PACKED
/* ************************************************************ */
/* ******************* NDPI NETWORKS HEADERS ****************** */
/* ************************************************************ */

/* ++++++++++++++++++++++++ Cisco headers +++++++++++++++++++++ */

/* Cisco HDLC */
#ifdef _MSC_VER
/* Windows */
#define PACK_ON   __pragma(pack(push, 1))
#define PACK_OFF  __pragma(pack(pop))
#elif defined(__GNUC__)
/* GNU C */
#define PACK_ON
#define PACK_OFF  __attribute__((packed))
#endif

/* PLEASE DO NOT REMOVE OR CHANGE THE ORDER OF WHAT IS DELIMITED BY CFFI.NDPI_PACKED_STRUCTURES FLAG AS IT IS USED FOR
   PYTHON BINDINGS AUTO GENERATION */
#ifdef NDPI_CFFI_PREPROCESSING
#undef PACK_ON
#undef PACK_OFF
#define PACK_ON
#define PACK_OFF
#endif
//CFFI.NDPI_PACKED_STRUCTURES
PACK_ON
struct ndpi_chdlc
{
  u_int8_t addr;          /* 0x0F (Unicast) - 0x8F (Broadcast) */
  u_int8_t ctrl;          /* always 0x00                       */
  u_int16_t proto_code;   /* protocol type (e.g. 0x0800 IP)    */
} PACK_OFF;

/* SLARP - Serial Line ARP http://tinyurl.com/qa54e95 */
PACK_ON
struct ndpi_slarp
{
  /* address requests (0x00)
     address replies  (0x01)
     keep-alive       (0x02)
  */
  u_int32_t slarp_type;
  u_int32_t addr_1;
  u_int32_t addr_2;
} PACK_OFF;

/* Cisco Discovery Protocol http://tinyurl.com/qa6yw9l */
PACK_ON
struct ndpi_cdp
{
  u_int8_t version;
  u_int8_t ttl;
  u_int16_t checksum;
  u_int16_t type;
  u_int16_t length;
} PACK_OFF;

/* +++++++++++++++ Ethernet header (IEEE 802.3) +++++++++++++++ */

PACK_ON
struct ndpi_ethhdr
{
  u_char h_dest[6];       /* destination eth addr */
  u_char h_source[6];     /* source ether addr    */
  u_int16_t h_proto;      /* data length (<= 1500) or type ID proto (>=1536) */
} PACK_OFF;

/* +++++++++++++++ ARP header +++++++++++++++ */
PACK_ON
struct ndpi_arphdr {
  u_int16_t ar_hrd;/* Format of hardware address.  */
  u_int16_t ar_pro;/* Format of protocol address.  */
  u_int8_t  ar_hln;/* Length of hardware address.  */
  u_int8_t  ar_pln;/* Length of protocol address.  */
  u_int16_t ar_op;/* ARP opcode (command).  */
  u_char arp_sha[6];/* sender hardware address */
  u_int32_t arp_spa;/* sender protocol address */
  u_char arp_tha[6];/* target hardware address */
  u_int32_t arp_tpa;/* target protocol address */
} PACK_OFF;

/* +++++++++++++++ DHCP header +++++++++++++++ */
PACK_ON
struct ndpi_dhcphdr {
  u_int8_t      msgType;
  u_int8_t      htype;
  u_int8_t      hlen;
  u_int8_t      hops;
  u_int32_t     xid;/* 4 */
  u_int16_t     secs;/* 8 */
  u_int16_t     flags;
  u_int32_t     ciaddr;/* 12 */
  u_int32_t     yiaddr;/* 16 */
  u_int32_t     siaddr;/* 20 */
  u_int32_t     giaddr;/* 24 */
  u_int8_t      chaddr[16]; /* 28 */
  u_int8_t      sname[64]; /* 44 */
  u_int8_t      file[128]; /* 108 */
  u_int32_t     magic; /* 236 */
  u_int8_t      options[308];
} PACK_OFF;

/* +++++++++++++++ MDNS rsp header +++++++++++++++ */
PACK_ON
struct ndpi_mdns_rsp_entry {
  u_int16_t rsp_type, rsp_class;
  u_int32_t ttl;
  u_int16_t data_len;
} PACK_OFF;

/* +++++++++++++++++++ LLC header (IEEE 802.2) ++++++++++++++++ */

PACK_ON
struct ndpi_snap_extension
{
  u_int16_t   oui;
  u_int8_t    oui2;
  u_int16_t   proto_ID;
} PACK_OFF;

PACK_ON
struct ndpi_llc_header_snap
{
  u_int8_t    dsap;
  u_int8_t    ssap;
  u_int8_t    ctrl;
  struct ndpi_snap_extension snap;
} PACK_OFF;

/* ++++++++++ RADIO TAP header (for IEEE 802.11) +++++++++++++ */
PACK_ON
struct ndpi_radiotap_header
{
  u_int8_t  version;         /* set to 0 */
  u_int8_t  pad;
  u_int16_t len;
  u_int32_t present;
  u_int64_t MAC_timestamp;
  u_int8_t flags;
} PACK_OFF;

/* ++++++++++++ Wireless header (IEEE 802.11) ++++++++++++++++ */
PACK_ON
struct ndpi_wifi_header
{
  u_int16_t fc;
  u_int16_t duration;
  u_char rcvr[6];
  u_char trsm[6];
  u_char dest[6];
  u_int16_t seq_ctrl;
  /* u_int64_t ccmp - for data encryption only - check fc.flag */
} PACK_OFF;

/* +++++++++++++++++++++++ MPLS header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_mpls_header
{
  /* Before using this strcut to parse an MPLS header, you will need to convert
   * the 4-byte data to the correct endianness with ntohl(). */
#if defined(__LITTLE_ENDIAN__)
  u_int32_t ttl:8, s:1, exp:3, label:20;
#elif defined(__BIG_ENDIAN__)
  u_int32_t label:20, exp:3, s:1, ttl:8;
#else
# error "Byte order must be defined"
#endif
} PACK_OFF;

/* ++++++++++++++++++++++++ IP header ++++++++++++++++++++++++ */

PACK_ON
struct ndpi_iphdr {
#if defined(__LITTLE_ENDIAN__)
  u_int8_t ihl:4, version:4;
#elif defined(__BIG_ENDIAN__)
  u_int8_t version:4, ihl:4;
#else
# error "Byte order must be defined"
#endif
  u_int8_t tos;
  u_int16_t tot_len;
  u_int16_t id;
  u_int16_t frag_off;
  u_int8_t ttl;
  u_int8_t protocol;
  u_int16_t check;
  u_int32_t saddr;
  u_int32_t daddr;
} PACK_OFF;

/* +++++++++++++++++++++++ IPv6 header +++++++++++++++++++++++ */
/* rfc3542 */

PACK_ON
struct ndpi_in6_addr {
  union {
    u_int8_t   u6_addr8[16];
    u_int16_t  u6_addr16[8];
    u_int32_t  u6_addr32[4];
    u_int64_t  u6_addr64[2];
  } u6_addr;  /* 128-bit IP6 address */
} PACK_OFF;

PACK_ON
struct ndpi_ip6_hdrctl {
  u_int32_t ip6_un1_flow;
  u_int16_t ip6_un1_plen;
  u_int8_t ip6_un1_nxt;
  u_int8_t ip6_un1_hlim;
} PACK_OFF;

PACK_ON
struct ndpi_ipv6hdr {
  struct ndpi_ip6_hdrctl ip6_hdr;
  struct ndpi_in6_addr ip6_src;
  struct ndpi_in6_addr ip6_dst;
} PACK_OFF;

/* +++++++++++++++++++++++ TCP header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_tcphdr
{
  u_int16_t source;
  u_int16_t dest;
  u_int32_t seq;
  u_int32_t ack_seq;
#if defined(__LITTLE_ENDIAN__)
  u_int16_t res1:4, doff:4, fin:1, syn:1, rst:1, psh:1, ack:1, urg:1, ece:1, cwr:1;
#elif defined(__BIG_ENDIAN__)
  u_int16_t doff:4, res1:4, cwr:1, ece:1, urg:1, ack:1, psh:1, rst:1, syn:1, fin:1;
#else
# error "Byte order must be defined"
#endif
  u_int16_t window;
  u_int16_t check;
  u_int16_t urg_ptr;
} PACK_OFF;

/* +++++++++++++++++++++++ UDP header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_udphdr
{
  u_int16_t source;
  u_int16_t dest;
  u_int16_t len;
  u_int16_t check;
} PACK_OFF;

PACK_ON
struct ndpi_dns_packet_header {
  u_int16_t tr_id;
  u_int16_t flags;
  u_int16_t num_queries;
  u_int16_t num_answers;
  u_int16_t authority_rrs;
  u_int16_t additional_rrs;
} PACK_OFF;


/* +++++++++++++++++++++++ ICMP header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_icmphdr {
  u_int8_t type;/* message type */
  u_int8_t code;/* type sub-code */
  u_int16_t checksum;
  union {
    struct {
      u_int16_t id;
      u_int16_t sequence;
    } echo; /* echo datagram */

    u_int32_t gateway; /* gateway address */
    struct {
      u_int16_t _unused;
      u_int16_t mtu;
    } frag;/* path mtu discovery */
  } un;
} PACK_OFF;

/* +++++++++++++++++++++++ ICMP6 header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_icmp6hdr {
  u_int8_t     icmp6_type;   /* type field */
  u_int8_t     icmp6_code;   /* code field */
  u_int16_t    icmp6_cksum;  /* checksum field */
  union {
    u_int32_t  icmp6_un_data32[1]; /* type-specific field */
    u_int16_t  icmp6_un_data16[2]; /* type-specific field */
    u_int8_t   icmp6_un_data8[4];  /* type-specific field */
  } icmp6_dataun;
} PACK_OFF;

/* +++++++++++++++++++++++ VXLAN header +++++++++++++++++++++++ */

PACK_ON
struct ndpi_vxlanhdr {
  u_int16_t flags;
  u_int16_t groupPolicy;
  u_int8_t vni[3];
  u_int8_t reserved;
} PACK_OFF;

#ifndef IPPROTO_GRE
#define IPPROTO_GRE 47
#endif

#define NDPI_GRE_CSUM        ntohs(0x8000)
#define NDPI_GRE_ROUTING     ntohs(0x4000)
#define NDPI_GRE_KEY         ntohs(0x2000)
#define NDPI_GRE_SEQ         ntohs(0x1000)
#define NDPI_GRE_STRICT      ntohs(0x0800)
#define NDPI_GRE_REC         ntohs(0x0700)
#define NDPI_GRE_ACK         ntohs(0x0080)
#define NDPI_GRE_FLAGS       ntohs(0x0078)
#define NDPI_GRE_VERSION     ntohs(0x0007)

#define NDPI_GRE_IS_CSUM(f)		((f) & NDPI_GRE_CSUM)
#define NDPI_GRE_IS_ROUTING(f)	((f) & NDPI_GRE_ROUTING)
#define NDPI_GRE_IS_KEY(f)		((f) & NDPI_GRE_KEY)
#define NDPI_GRE_IS_SEQ(f)		((f) & NDPI_GRE_SEQ)
#define NDPI_GRE_IS_STRICT(f)	((f) & NDPI_GRE_STRICT)
#define NDPI_GRE_IS_REC(f)		((f) & NDPI_GRE_REC)
#define NDPI_GRE_IS_FLAGS(f)		((f) & NDPI_GRE_FLAGS)
#define NDPI_GRE_IS_ACK(f)		((f) & NDPI_GRE_ACK)
#define NDPI_GRE_IS_VERSION_0(f) (((f) & NDPI_GRE_VERSION) == ntohs(0x0000))
#define NDPI_GRE_IS_VERSION_1(f) (((f) & NDPI_GRE_VERSION) == ntohs(0x0001))

#define NDPI_GRE_PROTO_PPP ntohs(0x880b)
#define NDPI_PPP_HDRLEN	4	/* octets for standard ppp header */

/* +++++++++++++++++++++++ GRE basic header +++++++++++++++++++++++ */
PACK_ON
struct ndpi_gre_basehdr {
	uint16_t flags;
	uint16_t protocol;
} PACK_OFF;

/* ************************************************************ */

/**
 * The application might inform the library about client/server direction
 */
#define NDPI_IN_PKT_DIR_UNKNOWN		0	/**< The application doesn't provide this kind of information */
#define NDPI_IN_PKT_DIR_C_TO_S		1	/**< Current packet is from client to server */
#define NDPI_IN_PKT_DIR_S_TO_C		2	/**< Current packet is from server to client */

/**
 * The application might choose to not pass TCP handshake packets to the library
 * (for performance reasons), but it might want to inform the library itlsef that these
 * packets have been captured/seen anyway (to avoid losing classifiation capabilities).
 */
#define NDPI_FLOW_BEGINNING_UNKNOWN	0	/**< The application doesn't provide this kind of information */
#define NDPI_FLOW_BEGINNING_SEEN	1	/**< The application informs the library that the TCP handshake has been seen (even if its packets might not have been passed to the library) */
#define NDPI_FLOW_BEGINNING_NOT_SEEN	2	/**< The application informs the library that the TCP handshake has not been seen */

/**
 * Optional information about flow management (per packet)
 */
struct ndpi_flow_input_info {
  unsigned char in_pkt_dir; /* If unknown, the library might *returns* to the application the direction calculated internally */
  unsigned char seen_flow_beginning;
};

PACK_ON
struct ndpi_binary_bitmap_entry {
#ifdef SAVE_BINARY_BITMAP_MEMORY
  u_int64_t value:56, category:8;
#else
  u_int64_t value;
  u_int8_t category;
#endif
} PACK_OFF;

/* ******************* ********************* ****************** */
/* ************************************************************ */

//CFFI.NDPI_PACKED_STRUCTURES
#endif // NDPI_CFFI_PREPROCESSING_EXCLUDE_PACKED


typedef union
{
  u_int32_t ipv4;
  struct ndpi_in6_addr ipv6;
} ndpi_ip_addr_t;


typedef struct message {
  u_int8_t *buffer;
  u_int buffer_len, buffer_used;
  u_int32_t next_seq;
} message_t;

/* NDPI_PROTOCOL_BITTORRENT */

#ifndef __KERNEL__

typedef struct spinlock {
  volatile int    val;
} spinlock_t;

typedef struct atomic {
  volatile int counter;
} atomic_t;

#define atomic_inc(a) ((atomic_t *)(a))->counter++
#define atomic_dec(a) ((atomic_t *)(a))->counter--
#define spin_lock_init(a) (a)->val = 0

static inline void spin_lock(spinlock_t *a) { a->val++; };
static inline void spin_unlock(spinlock_t *a) { a->val--; };

#define spin_lock_bh(a) spin_lock(a)
#define spin_unlock_bh(a) spin_unlock(a)
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
typedef ktime_t time_t;
#endif
#endif

struct hash_ip4p_node {
  struct hash_ip4p_node   *next,*prev;
  time_t                  lchg;
  u_int16_t               port,count:12,flag:4;
  u_int32_t               ip;
};

/* copy from hash_ip4p_node */
struct hash_ip6p_node {
  struct hash_ip6p_node   *next,*prev;
  time_t                  lchg;
  u_int16_t               port,count:12,flag:4;
  u_int32_t               ip[4];
};

struct hash_ip4p {
  struct hash_ip4p_node   *top;
  spinlock_t              lock;
  size_t                  len;
};

struct hash_ip4p_table {
  size_t                  size;
  int			  ipv6;
//  spinlock_t              lock;
  atomic_t                count;
  struct hash_ip4p        tbl[0];
};

struct bt_announce {              // 192 bytes
  u_int32_t		hash[5];
  u_int32_t		ip[4];
  u_int32_t		time;
  u_int16_t		port;
  u_int8_t		name_len,
    name[192 - 4*10 - 2 - 1];     // 149 bytes
};

/* NDPI_PROTOCOL_TINC */
#define TINC_CACHE_MAX_SIZE 10

/*
   In case the typedef below is modified, please update
   ndpi_http_method2str (ndpi_utils.c)
*/
typedef enum {
	      NDPI_HTTP_METHOD_UNKNOWN = 0,
	      NDPI_HTTP_METHOD_OPTIONS,
	      NDPI_HTTP_METHOD_GET,
	      NDPI_HTTP_METHOD_HEAD,
	      NDPI_HTTP_METHOD_PATCH,
	      NDPI_HTTP_METHOD_POST,
	      NDPI_HTTP_METHOD_PUT,
	      NDPI_HTTP_METHOD_DELETE,
	      NDPI_HTTP_METHOD_TRACE,
	      NDPI_HTTP_METHOD_CONNECT,
	      NDPI_HTTP_METHOD_RPC_CONNECT,
	      NDPI_HTTP_METHOD_RPC_IN_DATA,
	      NDPI_HTTP_METHOD_RPC_OUT_DATA,
	      NDPI_HTTP_METHOD_MKCOL,
	      NDPI_HTTP_METHOD_MOVE,
	      NDPI_HTTP_METHOD_COPY,
	      NDPI_HTTP_METHOD_LOCK,
	      NDPI_HTTP_METHOD_UNLOCK,
	      NDPI_HTTP_METHOD_PROPFIND,
	      NDPI_HTTP_METHOD_PROPPATCH,
} ndpi_http_method;

typedef enum {
  NDPI_PTREE_RISK_MASK = 0,
  NDPI_PTREE_RISK_MASK6,
  NDPI_PTREE_RISK,
  NDPI_PTREE_RISK6,
  NDPI_PTREE_PROTOCOLS,
  NDPI_PTREE_PROTOCOLS6,

  NDPI_PTREE_MAX	/* Last one! */
} ptree_type;

enum {
  NO_RTP_RTCP = 0,
  IS_RTP = 1,
  IS_RTCP = 2,
};

typedef enum {
  NDPI_AUTOMA_HOST = 0,
  NDPI_AUTOMA_DOMAIN,
  NDPI_AUTOMA_TLS_CERT,
  NDPI_AUTOMA_RISK_MASK,
  NDPI_AUTOMA_COMMON_ALPNS,

  NDPI_AUTOMA_MAX	/* Last one! */
} automa_type;

struct ndpi_automa_stats {
  u_int64_t n_search;
  u_int64_t n_found;
};

typedef enum {
  NDPI_LRUCACHE_OOKLA = 0,
  NDPI_LRUCACHE_BITTORRENT,
  NDPI_LRUCACHE_STUN,
  NDPI_LRUCACHE_TLS_CERT,
  NDPI_LRUCACHE_MINING,
  NDPI_LRUCACHE_MSTEAMS,
  NDPI_LRUCACHE_FPC_DNS, /* FPC DNS cache */
  NDPI_LRUCACHE_SIGNAL,
  NDPI_LRUCACHE_MAX	/* Last one! */
} lru_cache_type;

typedef enum {
  NDPI_LRUCACHE_SCOPE_LOCAL = 0,
  NDPI_LRUCACHE_SCOPE_GLOBAL,
} lru_cache_scope;

struct ndpi_lru_cache_entry {
  u_int64_t key; /* Store the whole key to avoid ambiguities */
  u_int32_t is_full:1, value:16, pad:15;
  u_int32_t timestamp; /* sec */
};

struct ndpi_lru_cache_stats {
  u_int64_t n_insert;
  u_int64_t n_search;
  u_int64_t n_found;
};

struct ndpi_lru_cache {
  u_int32_t num_entries;
  u_int32_t ttl : 31, shared : 1;
#ifndef NDPI_CFFI_PREPROCESSING
#ifndef __KERNEL__
#ifdef USE_GLOBAL_CONTEXT
  pthread_mutex_t mutex;
#endif
#endif
#endif
  struct ndpi_lru_cache_stats stats;
  struct ndpi_lru_cache_entry *entries;
};

#define NDPI_GIVEUP_GUESS_BY_PORT	0x01
#define NDPI_GIVEUP_GUESS_BY_IP		0x02

/* Aggressiveness values */

#define NDPI_AGGRESSIVENESS_DISABLED			0x00 /* For all protocols */

/* Ookla */
#define NDPI_AGGRESSIVENESS_OOKLA_TLS			0x01 /* Enable detection over TLS (using ookla cache) */

/* OpenVPN */
#define NDPI_HEURISTICS_OPENVPN_OPCODE			0x01 /* Enable heuristic based on opcode frequency */

/* TLS */
#define NDPI_HEURISTICS_TLS_OBFUSCATED_PLAIN		0x01 /* Enable heuristic to detect proxied/obfuscated TLS flows over generic/unknown flows */
#define NDPI_HEURISTICS_TLS_OBFUSCATED_TLS		0x02 /* Enable heuristic to detect proxied/obfuscated TLS flows over TLS tunnels, i.e. TLS over TLS */
#define NDPI_HEURISTICS_TLS_OBFUSCATED_HTTP		0x04 /* Enable heuristic to detect proxied/obfuscated TLS flows over HTTP/WebSocket */


/* ************************************************** */

struct ndpi_flow_tcp_struct {
  /* TCP sequence number */
  u_int32_t next_tcp_seq_nr[2];
  u_int16_t last_tcp_pkt_payload_len;

  /* Part of the TCP header */
  u_int8_t cli2srv_tcp_flags, srv2cli_tcp_flags;

  /* NDPI_PROTOCOL_MAIL_SMTP */
  /* NDPI_PROTOCOL_MAIL_POP */
  /* NDPI_PROTOCOL_MAIL_IMAP */
  /* NDPI_PROTOCOL_FTP_CONTROL */
  /* TODO: something clever to save memory */
  struct {
    u_int8_t auth_found:1, auth_failed:1, auth_tls:1, auth_done:1, _pad:4;
    char username[32], password[16];
  } ftp_imap_pop_smtp;

  struct {
    /* NDPI_PROTOCOL_TLS */
    u_int8_t app_data_seen[2];
    u_int8_t num_tls_blocks;
    int16_t tls_application_blocks_len[NDPI_MAX_NUM_TLS_APPL_BLOCKS]; /* + = src->dst, - = dst->src */
  } tls;



  /* NDPI_PROTOCOL_MAIL_SMTP */
  u_int16_t smtp_command_bitmask;

  /* NDPI_PROTOCOL_MAIL_POP */
  u_int16_t pop_command_bitmask;

  /* NDPI_PROTOCOL_RTMP */
  u_int16_t rtmp_client_buffer_len;

  /* NDPI_PROTOCOL_HCL_NOTES */
  u_int8_t hcl_notes_packet_id;

  /* NDPI_PROTOCOL_WHATSAPP */
  u_int8_t wa_matched_so_far;

  /* NDPI_PROTOCOL_IRC */
  u_int8_t irc_stage;

  /* NDPI_PROTOCOL_NEST_LOG_SINK */
  u_int8_t nest_log_sink_matches;

  /* NDPI_PROTOCOL_MEMCACHED */
  u_int8_t memcached_matches;

  /* Part of the TCP header */
  u_int64_t seen_syn:1, seen_syn_ack:1, seen_ack:1;

  /* NDPI_PROTOCOL_IRC */
  u_int64_t irc_3a_counter:3;

  /* NDPI_PROTOCOL_USENET */
  u_int64_t usenet_stage:2;

  /* NDPI_PROTOCOL_HTTP */
  u_int64_t http_stage:3;
  u_int64_t http_asymmetric_stage:2;

  /* NDPI_PROTOCOL_GNUTELLA */
  u_int64_t gnutella_stage:2;

  /* NDPI_PROTOCOL_SSH */
  u_int64_t ssh_stage:3;

  /* NDPI_PROTOCOL_VNC */
  u_int64_t vnc_stage:2;

  /* NDPI_PROTOCOL_TELNET */
  u_int64_t telnet_stage:2;

  /* NDPI_PROTOCOL_RADMIN */
  u_int64_t radmin_stage:1;

  /* NDPI_PROTOCOL_FTP_CONTROL */
  u_int64_t ftp_control_stage:2;

  /* NDPI_PROTOCOL_SOAP */
  u_int64_t soap_stage:1;

  /* NDPI_PROTOCOL_SOCKS */
  u_int64_t socks5_stage:2;
  u_int64_t socks4_stage:2;

  /* NDPI_PROTOCOL_Z3950 */
  u_int64_t z3950_stage:2;

  /* NDPI_PROTOCOL_RTMP */
  u_int64_t rtmp_stage:2;

  /* NDPI_PROTOCOL_POSTGRES */
  u_int64_t postgres_stage:3;

  /* NDPI_PROTOCOL_ICECAST */
  u_int64_t icecast_stage:1;

  /* NDPI_PROTOCOL_MAIL_POP */
  u_int64_t mail_pop_stage:2;

  /* NDPI_PROTOCOL_MAIL_IMAP */
  u_int64_t mail_imap_stage:3;
  u_int64_t mail_imap_starttls:1;

  /* Reserved for future use */
  u_int64_t reserved:20;
};

/* ************************************************** */

struct ndpi_flow_udp_struct {
  /* NDPI_PROTOCOL_TFTP */
  u_int32_t tftp_stage:2;

  /* NDPI_PROTOCOL_XBOX */
  u_int32_t xbox_stage:1;

  /* NDPI_PROTOCOL_QUIC */
  u_int32_t quic_server_cid_stage:2;
  u_int32_t quic_0rtt_found:1;
  u_int32_t quic_vn_pair:1;

  /* NDPI_PROTOCOL_LOLWILDRIFT */
  u_int32_t lolwildrift_stage:1;

  /* NDPI_PROTOCOL_ZOOM */
  u_int32_t zoom_p2p:1;

  /* NDPI_PROTOCOL_RAKNET */
  u_int32_t raknet_custom:1;

  /* NDPI_PROTOCOL_MUMBLE */
  u_int32_t mumble_stage:1;

  /* NDPI_PROTOCOL_HAMACHI */
  u_int32_t hamachi_stage:2;

  /* NDPI_PROTOCOL_EPICGAMES */
  u_int32_t epicgames_stage:1;
  u_int32_t epicgames_word;

  /* NDPI_PROTOCOL_RX */
  u_int32_t rx_conn_epoch;
  u_int32_t rx_conn_id;

  /* NDPI_PROTOCOL_WIREGUARD */
  u_int32_t wireguard_peer_index[2];
  u_int8_t wireguard_stage;

  /* NDPI_PROTOCOL_MEMCACHED */
  u_int8_t memcached_matches;

  /* NDPI_PROTOCOL_EAQ */
  u_int8_t eaq_pkt_id;
  u_int32_t eaq_sequence;

  /* NDPI_PROTOCOL_MUMBLE */
  u_int64_t mumble_ident;

  /* NDPI_PROTOCOL_HAMACHI */
  u_int32_t hamachi_long[2];
  u_int16_t hamachi_short[2];

  /* NDPI_PROTOCOL_QUIC */
  u_int8_t *quic_reasm_buf;
  u_int8_t *quic_reasm_buf_bitmap;
  u_int32_t quic_reasm_buf_last_pos;
#define QUIC_SERVER_CID_HEURISTIC_LENGTH	8
  u_int8_t quic_server_cid[QUIC_SERVER_CID_HEURISTIC_LENGTH];
  u_int8_t quic_client_last_byte;
  /* DCID of the first Initial sent by the client */
  u_int8_t quic_orig_dest_conn_id[20]; /* Max length is 20 on all QUIC versions */
  u_int8_t quic_orig_dest_conn_id_len;

  /* NDPI_PROTOCOL_RDP */
  u_int8_t rdp_to_srv[3], rdp_from_srv[3], rdp_to_srv_pkts, rdp_from_srv_pkts;

  /* NDPI_PROTOCOL_IMO */
  u_int8_t imo_last_one_byte_pkt, imo_last_byte;

  /* NDPI_PROTOCOL_LINE_CALL */
  u_int8_t line_pkts[2];
  u_int8_t line_base_cnt[2];

  /* NDPI_PROTOCOL_TFTP */
  u_int16_t tftp_data_num;
  u_int16_t tftp_ack_num;

};

/* ************************************************** */


struct ndpi_global_context;
struct ndpi_detection_module_struct;
struct ndpi_flow_struct;

typedef struct {
  u_int16_t port_low, port_high;
} ndpi_port_range;

typedef enum {
  NDPI_CONFIDENCE_UNKNOWN           = 0,    /* Unknown classification */
  NDPI_CONFIDENCE_MATCH_BY_PORT,            /* Classification obtained looking only at the L4 ports */
  NDPI_CONFIDENCE_MATCH_BY_IP,              /* Classification obtained looking only at the IP */
  NDPI_CONFIDENCE_NBPF,                     /* PF_RING nBPF (custom protocol) */
  NDPI_CONFIDENCE_DPI_PARTIAL,              /* Classification results based on partial/incomplete DPI information */
  NDPI_CONFIDENCE_DPI_PARTIAL_CACHE,        /* Classification results based on some LRU cache with partial/incomplete DPI information */
  NDPI_CONFIDENCE_DPI_CACHE,                /* Classification results based on some LRU cache (i.e. correlation among sessions) */
  NDPI_CONFIDENCE_DPI,                      /* Deep packet inspection */
  NDPI_CONFIDENCE_DPI_AGGRESSIVE,           /* Aggressive DPI: it might be a false positive */
  NDPI_CONFIDENCE_CUSTOM_RULE,              /* Matching a custom rules */

  /*
    IMPORTANT

    Please keep in sync with
    ndpi_confidence_get_name()
    in ndpi_main.c
  */

  /* Last one */
  NDPI_CONFIDENCE_MAX,
} ndpi_confidence_t;

typedef enum {
  NDPI_FPC_CONFIDENCE_UNKNOWN           = 0,    /* Unknown First Packet Classification */
  NDPI_FPC_CONFIDENCE_IP,                       /* FPC based on IP address */
  NDPI_FPC_CONFIDENCE_DNS,                      /* FPC based on DNS information */
  NDPI_FPC_CONFIDENCE_DPI,                      /* FPC based on DPI information (i.e. flow classified via DPI with only one packet)*/

  /*
    IMPORTANT

    Please keep in sync with
    ndpi_fpc_confidence_get_name()
    in ndpi_main.c
  */

  /* Last one */
  NDPI_FPC_CONFIDENCE_MAX,
} ndpi_fpc_confidence_t;

typedef enum {
  NDPI_PROTOCOL_SAFE = 0,              /* Surely doesn't provide risks for the network. (e.g., a news site) */
  NDPI_PROTOCOL_ACCEPTABLE,            /* Probably doesn't provide risks, but could be malicious (e.g., Dropbox) */
  NDPI_PROTOCOL_FUN,                   /* Pure fun protocol, which may be prohibited by the user policy (e.g., Netflix) */
  NDPI_PROTOCOL_UNSAFE,                /* Probably provides risks, but could be a normal traffic. Unencrypted protocols with clear pass should be here (e.g., telnet) */
  NDPI_PROTOCOL_POTENTIALLY_DANGEROUS, /* Possibly dangerous (ex. Tor). */
  NDPI_PROTOCOL_DANGEROUS,             /* Surely is dangerous (ex. smbv1). Be prepared to troubles */
  NDPI_PROTOCOL_TRACKER_ADS,           /* Trackers, Advertisements... */
  NDPI_PROTOCOL_UNRATED                /* No idea, not implemented or impossible to classify */
} ndpi_protocol_breed_t;

#define NUM_BREEDS (NDPI_PROTOCOL_UNRATED+1)

/*
  Abstract categories to group the protocols.

  IMPORTANT
  Keep in sync with categories[] on ndpi_main.c
*/
typedef enum {
  NDPI_PROTOCOL_CATEGORY_UNSPECIFIED = 0,   /* For general services and unknown protocols */
  NDPI_PROTOCOL_CATEGORY_MEDIA,             /* Multimedia and streaming */
  NDPI_PROTOCOL_CATEGORY_VPN,               /* Virtual Private Networks */
  NDPI_PROTOCOL_CATEGORY_MAIL,              /* Protocols to send/receive/sync emails */
  NDPI_PROTOCOL_CATEGORY_DATA_TRANSFER,     /* AFS/NFS and similar protocols */
  NDPI_PROTOCOL_CATEGORY_WEB,               /* Web/mobile protocols and services */
  NDPI_PROTOCOL_CATEGORY_SOCIAL_NETWORK,    /* Social networks */
  NDPI_PROTOCOL_CATEGORY_DOWNLOAD_FT,       /* Download, FTP, file transfer/sharing */
  NDPI_PROTOCOL_CATEGORY_GAME,              /* Online games */
  NDPI_PROTOCOL_CATEGORY_CHAT,              /* Instant messaging */
  NDPI_PROTOCOL_CATEGORY_VOIP,              /* Real-time communications and conferencing */
  NDPI_PROTOCOL_CATEGORY_DATABASE,          /* Protocols for database communication */
  NDPI_PROTOCOL_CATEGORY_REMOTE_ACCESS,     /* Remote access and control */
  NDPI_PROTOCOL_CATEGORY_CLOUD,             /* Online cloud services */
  NDPI_PROTOCOL_CATEGORY_NETWORK,           /* Network infrastructure protocols */
  NDPI_PROTOCOL_CATEGORY_COLLABORATIVE,     /* Software for collaborative development, including Webmail */
  NDPI_PROTOCOL_CATEGORY_RPC,               /* High level network communication protocols */
  NDPI_PROTOCOL_CATEGORY_STREAMING,         /* Streaming protocols */
  NDPI_PROTOCOL_CATEGORY_SYSTEM_OS,         /* System/Operating System level applications */
  NDPI_PROTOCOL_CATEGORY_SW_UPDATE,         /* Software update */

  /* See #define NUM_CUSTOM_CATEGORIES */
  NDPI_PROTOCOL_CATEGORY_CUSTOM_1,          /* User custom category 1 */
  NDPI_PROTOCOL_CATEGORY_CUSTOM_2,          /* User custom category 2 */
  NDPI_PROTOCOL_CATEGORY_CUSTOM_3,          /* User custom category 3 */
  NDPI_PROTOCOL_CATEGORY_CUSTOM_4,          /* User custom category 4 */
  NDPI_PROTOCOL_CATEGORY_CUSTOM_5,          /* User custom category 5 */

  /* Further categories... */
  NDPI_PROTOCOL_CATEGORY_MUSIC,
  NDPI_PROTOCOL_CATEGORY_VIDEO,
  NDPI_PROTOCOL_CATEGORY_SHOPPING,
  NDPI_PROTOCOL_CATEGORY_PRODUCTIVITY,
  NDPI_PROTOCOL_CATEGORY_FILE_SHARING,

  /*
    The category below is used by sites who are used
    to test connectivity
  */
  NDPI_PROTOCOL_CATEGORY_CONNECTIVITY_CHECK,
  NDPI_PROTOCOL_CATEGORY_IOT_SCADA,
  /*
    The category below is used for vocal assistance services.
  */
  NDPI_PROTOCOL_CATEGORY_VIRTUAL_ASSISTANT,
  NDPI_PROTOCOL_CATEGORY_CYBERSECURITY,
  NDPI_PROTOCOL_CATEGORY_ADULT_CONTENT,

  /* Some custom categories */
  CUSTOM_CATEGORY_MINING           = 99,
  CUSTOM_CATEGORY_MALWARE          = 100,
  CUSTOM_CATEGORY_ADVERTISEMENT    = 101,
  CUSTOM_CATEGORY_BANNED_SITE      = 102,
  CUSTOM_CATEGORY_SITE_UNAVAILABLE = 103,
  CUSTOM_CATEGORY_ALLOWED_SITE     = 104,
  /*
    The category below is used to track communications made by
    security applications (e.g. sophosxl.net, spamhaus.org)
    to track malware, spam etc.
  */
  CUSTOM_CATEGORY_ANTIMALWARE      = 105,

  /* Crypto Currency e.g Bitcoin, Litecoin, Etherum ..et. */
  NDPI_PROTOCOL_CATEGORY_CRYPTO_BLOCKCHAIN = 106,

  /* Gambling websites */
  NDPI_PROTOCOL_CATEGORY_GAMBLING = 107,
  NDPI_PROTOCOL_CATEGORY_HEALTH,
  NDPI_PROTOCOL_CATEGORY_ARTIFICIAL_INTELLIGENCE,
  NDPI_PROTOCOL_CATEGORY_FINANCE,
  NDPI_PROTOCOL_CATEGORY_NEWS,
  NDPI_PROTOCOL_CATEGORY_SPORT,
  NDPI_PROTOCOL_CATEGORY_BUSINESS,
  NDPI_PROTOCOL_CATEGORY_INTERNET_HOSTING,          /* Internet sites including hosting */
  NDPI_PROTOCOL_CATEGORY_BLOCKCHAIN_CRYPTO,
  NDPI_PROTOCOL_CATEGORY_BLOG_FORUM,
  NDPI_PROTOCOL_CATEGORY_GOVERNMENT,
  NDPI_PROTOCOL_CATEGORY_EDUCATION,
  NDPI_PROTOCOL_CATEGORY_CND_PROXY,
  NDPI_PROTOCOL_CATEGORY_HARDWARE_SOFTWARE,
  NDPI_PROTOCOL_CATEGORY_DATING,
  NDPI_PROTOCOL_CATEGORY_TRAVEL,
  NDPI_PROTOCOL_CATEGORY_FOOD,
  
  NDPI_PROTOCOL_CATEGORY_BOTS, /* Crawlers, bots */
  NDPI_PROTOCOL_CATEGORY_SCANNERS, /* e.g. shodan.io, censys.io */


  /*
    IMPORTANT

    Please keep in sync with

    static const char* categories[] = { ..}

    in ndpi_main.c
  */

  NDPI_PROTOCOL_NUM_CATEGORIES, /*
				  NOTE: Keep this as last member
				  Unused as value but useful to getting the number of elements
				  in this datastructure
				*/
  NDPI_PROTOCOL_ANY_CATEGORY /* Used to handle wildcards */
} ndpi_protocol_category_t;

typedef enum {
  NDPI_PROTOCOL_QOE_CATEGORY_UNSPECIFIED = 0,    /* Unspecified/default QoE constraints          */
  NDPI_PROTOCOL_QOE_CATEGORY_VOIP_CALLS,         /* VoIP Calls (G.711, RTP, SIP)                 */
  NDPI_PROTOCOL_QOE_CATEGORY_VIDEO_CONFERENCING, /* Video Conferencing (Zoom, WebRTC, Skype)     */
  NDPI_PROTOCOL_QOE_CATEGORY_ONLINE_GAMING,      /* Online Gaming (FPS, MOBA, MMO)               */
  NDPI_PROTOCOL_QOE_CATEGORY_CLOUD_GAMING,       /* Cloud Gaming (GeForce NOW, Stadia, xCloud)   */
  NDPI_PROTOCOL_QOE_CATEGORY_WEB_BROWSING,       /* Web Browsing (HTTP, HTTPS)                   */
  NDPI_PROTOCOL_QOE_CATEGORY_DNS_QUERIES,        /* DNS Queries (UDP/53)                         */
  NDPI_PROTOCOL_QOE_CATEGORY_LIVE_STREAMING,     /* Live Streaming (Twitch, YouTube Live, etc.)  */
  NDPI_PROTOCOL_QOE_CATEGORY_BUFFERED_STREAMING, /* Buffered Streaming (Netflix, YouTube, etc.)  */
  NDPI_PROTOCOL_QOE_CATEGORY_REMOTE_ACCESS,      /* Remote Desktop (RDP, VNC, AnyDesk)           */
  NDPI_PROTOCOL_QOE_CATEGORY_HFT,                /* High-Frequency Trading (HFT, Stock Trading)  */
  NDPI_PROTOCOL_QOE_CATEGORY_IOT_OT,             /* IoT Smart Home (Zigbee, Z-Wave, MQTT)        */
  NDPI_PROTOCOL_QOE_CATEGORY_AV,                 /* Autonomous Vehicles (V2X, 5G C-V2X)          */

  NDPI_PROTOCOL_QOE_CATEGORY_MAX                 /* Leave it as last entry !!!                   */
} ndpi_protocol_qoe_category_t;

/* ntop extensions */
typedef struct ndpi_proto_defaults {
  char *protoName;
  ndpi_protocol_category_t protoCategory;
  u_int8_t isClearTextProto:1, isAppProtocol:1, _notused:6;
  u_int16_t *subprotocols;
  u_int32_t subprotocol_count;
  u_int16_t protoId, dissector_idx;
  ndpi_port_range tcp_default_ports[MAX_DEFAULT_PORTS], udp_default_ports[MAX_DEFAULT_PORTS];
  ndpi_protocol_breed_t protoBreed;
  ndpi_protocol_qoe_category_t qoeCategory;
} ndpi_proto_defaults_t;


typedef struct _ndpi_automa {
  void *ac_automa; /* Real type is AC_AUTOMATA_t */
  struct ndpi_automa_stats stats;
} ndpi_automa;

typedef void ndpi_str_hash;

typedef struct {
  /*
    Note
    below we do not use ndpi_protocol_id_t as users can define their own
    custom protocols and thus the typedef could be too short in size.
  */
  u_int16_t master_protocol /* e.g. HTTP */, app_protocol /* e.g. FaceBook */;
} ndpi_master_app_protocol;

struct ndpi_fpc_info {
  ndpi_master_app_protocol proto;
  ndpi_fpc_confidence_t confidence;
};

typedef struct ndpi_proto {
  ndpi_master_app_protocol proto;
  u_int16_t protocol_by_ip;
#ifndef __KERNEL__
  ndpi_protocol_category_t category;
  void *custom_category_userdata;
#endif
} ndpi_protocol;

#ifndef __KERNEL__
  #define NDPI_PROTOCOL_NULL { { NDPI_PROTOCOL_UNKNOWN , NDPI_PROTOCOL_UNKNOWN }, NDPI_PROTOCOL_UNKNOWN, NDPI_PROTOCOL_CATEGORY_UNSPECIFIED, NULL }
#else
  #define NDPI_PROTOCOL_NULL { { NDPI_PROTOCOL_UNKNOWN , NDPI_PROTOCOL_UNKNOWN }, NDPI_PROTOCOL_UNKNOWN }
#endif

#define NUM_CUSTOM_CATEGORIES      5
#define CUSTOM_CATEGORY_LABEL_LEN 32

typedef void ndpi_bitmap;
typedef void ndpi_bitmap64;
typedef void ndpi_bitmap64_fuse; /* probabilistic */
typedef void ndpi_bitmap_iterator;
typedef void ndpi_filter;

typedef struct {
  u_int32_t num_allocated_entries, num_used_entries;
  struct ndpi_binary_bitmap_entry *entries;
  bool is_compressed;
} ndpi_binary_bitmap;

#define MAX_NUM_NDPI_DOMAIN_CLASSIFICATIONS          16

typedef struct {
  ndpi_str_hash *domains;
} ndpi_domain_classify;

typedef enum {
   ndpi_cipher_safe = NDPI_CIPHER_SAFE,
   ndpi_cipher_weak = NDPI_CIPHER_WEAK,
   ndpi_cipher_insecure = NDPI_CIPHER_INSECURE
} ndpi_cipher_weakness;

#define MAX_NUM_TLS_SIGNATURE_ALGORITHMS 16
#define MAX_NUM_DNS_RSP_ADDRESSES         4

typedef struct {
  union {
    u_int32_t v4;
    u_int8_t v6[16];
  } address; /* Network-order */

  u_int16_t port;
  u_int16_t is_ipv6: 1, _pad: 15;
} ndpi_address_port;

struct tls_heuristics {
  /*
    TLS heuristics for detecting browsers usage
    NOTE: expect false positives
  */
  u_int8_t is_safari_tls:1, is_firefox_tls:1, is_chrome_tls:1, notused:5;
};

struct ndpi_risk_information {
  ndpi_risk_enum id;
  char *info;
};

struct ndpi_metadata_monitoring {
  union {
    struct {
      ndpi_address_port mapped_address;
      ndpi_address_port peer_address;
      ndpi_address_port relayed_address;
      ndpi_address_port response_origin;
      ndpi_address_port other_address;
    } dtls_stun_rtp;
  } protos;
};

typedef enum {
  ndpi_os_unknown     = 0,
  ndpi_os_windows     = 1,
  ndpi_os_macos       = 2,
  ndpi_os_ios_ipad_os = 3,
  ndpi_os_android     = 4,
  ndpi_os_linux       = 5,
  ndpi_os_freebsd     = 6,
  ndpi_os_MAX_OS      = 7 /* Keep it as last */
} ndpi_os;

struct os_fingerprint {
  const char *fingerprint;
  ndpi_os os;
};

struct rtp_info {
  u_int8_t payload_type;
  bool payload_detected;
  u_int32_t evs_subtype;
};

struct ndpi_flow_struct {
  u_int16_t detected_protocol_stack[NDPI_PROTOCOL_SIZE];

  u_int16_t guessed_protocol_id;       /* Classification by-port. Set with the first pkt and never updated */
  u_int16_t guessed_protocol_id_by_ip; /* Classification by-ip. Set with the first pkt and never updated */
  u_int16_t fast_callback_protocol_id; /* Partial/incomplete classification. Used internally as first callback when iterating all the protocols */
  u_int16_t guessed_header_category;
  u_int8_t l4_proto, protocol_id_already_guessed:1, fail_with_unknown:1, ip_port_finished:1,
    init_finished:1, client_packet_direction:1, packet_direction:1, is_ipv6:1, first_pkt_fully_encrypted:1;
  u_int8_t skip_entropy_check: 1, monitoring:1, already_gaveup:1, _pad:5;

  u_int16_t num_dissector_calls;
  ndpi_confidence_t confidence; /* ndpi_confidence_t */

  uint16_t ipdef_proto; /* protocol by ip/port + ip_port_finished */
  ndpi_confidence_t ipdef_proto_level;

  /* First Packet Classification info */
  struct ndpi_fpc_info fpc;

  /* Flow addresses (useful for LRU lookups in ndpi_detection_giveup())
     and ports. All in *network* byte order.
     Client and server.
   */
  union {
    u_int32_t v4;
    u_int8_t v6[16];
  } c_address, s_address;	/* For some unknown reasons, x86_64-w64-mingw32-gcc doesn't like the name "s_addr" */
  u_int16_t c_port, s_port;

  // -----------------------------------------

  u_int8_t max_extra_packets_to_check;
  u_int8_t num_extra_packets_checked;
  u_int8_t num_processed_packets[2]; /* packet with payload. direct and replay. 255 max */
  u_int16_t num_processed_pkts; /* <= WARNING it can wrap but we do expect people to giveup earlier */

  int (*extra_packets_func) (struct ndpi_detection_module_struct *, struct ndpi_flow_struct *flow);

  u_int64_t last_packet_time_ms;
  u_int64_t last_packet_time;

  ndpi_protocol_category_t category;

  /* Counters with only packets with L5 data (ie no TCP SYN, pure ACKs, ...) */
  u_int16_t packet_counter;
  u_int16_t packet_direction_counter[2];
  /* Counters with all packets even those without payload */
  u_int16_t all_packets_counter;
  u_int16_t packet_direction_complete_counter[2];

  /*
    the tcp / udp / other l4 value union
    used to reduce the number of bytes for tcp or udp protocol states
  */
  union {
    struct ndpi_flow_tcp_struct tcp;
    struct ndpi_flow_udp_struct udp;
  } l4;

  /* Some protocols calculate the entropy. */
  float entropy;

  /* General purpose field used to save mainly hostname/SNI information.
   * In details it used for: MGCP, COLLECTD, DNS, SSDP and NETBIOS name, HTTP, MUNIN and DHCP hostname,
   * WHOIS request, TLS/QUIC server name, XIAOMI domain and STUN realm.
   *
   * Please, think *very* hard before increasing its size!
   */
  char host_server_name[80];

  u_int8_t initial_binary_bytes[8], initial_binary_bytes_len;
  u_int8_t risk_checked:1, ip_risk_mask_evaluated:1, host_risk_mask_evaluated:1, tree_risk_checked:1, _notused:4;
  ndpi_risk risk_mask; /* Stores the flow risk mask for flow peers */
  ndpi_risk risk, risk_shadow; /* Issues found with this flow [bitmask of ndpi_risk] */
  struct ndpi_risk_information risk_infos[MAX_NUM_RISK_INFOS]; /* String that contains information about the risks found */
  u_int8_t num_risk_infos;

  struct {
    char *fingerprint;
    char *fingerprint_raw;
    ndpi_os os_hint;
  } tcp;

  /*
    This structure below will not not stay inside the protos
    structure below as HTTP is used by many subprotocols
    such as FaceBook, Google... so it is hard to know
    when to use it or not. Thus we leave it outside for the
    time being.
  */
  struct {
    ndpi_http_method method;
    u_int8_t request_version; /* 0=1.0 and 1=1.1. Create an enum for this? */
    u_int8_t websocket:1, request_header_observed:1, first_payload_after_header_observed:1, is_form:1, _pad:4;
    u_int16_t response_status_code; /* 200, 404, etc. */
    char *url, *content_type /* response */, *request_content_type /* e.g. for POST */, *user_agent, *server, *referer, *host;
    char *detected_os; /* Via HTTP/QUIC User-Agent */
    char *nat_ip; /* Via HTTP X-Forwarded-For */
    char *filename; /* Via HTTP Content-Disposition */
    char *username, *password;
  } http;

  u_int8_t flow_multimedia_types;

  /*
     Put outside of the union to avoid issues in case the protocol
     is remapped to something other than Kerberos due to a faulty
     dissector
  */
  struct {
    char *pktbuf;
    u_int16_t pktbuf_maxlen, pktbuf_currlen;
  } kerberos_buf;

  struct {
    u_int8_t maybe_dtls:1, rtcp_seen:1, is_turn : 1, is_client_controlling:1, pad : 4;
    ndpi_address_port mapped_address, peer_address, relayed_address, response_origin, other_address;
    u_int8_t num_xor_relayed_addresses, num_xor_mapped_addresses;
    u_int8_t num_non_stun_pkt, non_stun_pkt_len[2];
    u_int16_t rtp_counters[2];
    u_int32_t t_start, t_end;
  } stun;

  struct {
    message_t message[2]; /* Directions */
    u_int8_t certificate_processed:1, change_cipher_from_client:1, change_cipher_from_server:1, from_opportunistic_tls:1, from_rdp:1, pad:3;
    struct tls_obfuscated_heuristic_state *obfuscated_heur_state;
  } tls_quic; /* Used also by DTLS and POPS/IMAPS/SMTPS/FTPS */

  struct rtp_info rtp[2 /* directions */];

  union {
    /* the only fields useful for nDPI and ntopng */
    struct {
      u_int8_t num_queries, num_answers, reply_code, num_rsp_addr;
      u_int8_t is_query:1, pad:7;
      u_int16_t transaction_id, query_type, query_class, rsp_type, edns0_udp_payload_size;
      u_int8_t is_rsp_addr_ipv6[MAX_NUM_DNS_RSP_ADDRESSES];
      ndpi_ip_addr_t rsp_addr[MAX_NUM_DNS_RSP_ADDRESSES]; /* The first num_rsp_addr address in a DNS response packet (A and AAAA) */
      u_int32_t rsp_addr_ttl[MAX_NUM_DNS_RSP_ADDRESSES];
      char geolocation_iata_code[4];
      char ptr_domain_name[64 /* large enough but smaller than { } tls */];
    } dns;

    struct {
      u_int8_t version;
      u_int8_t mode;
    } ntp;

    struct {
      char hostname[48], domain[48], username[48];
    } kerberos;

    struct {
      char ip[16];
      char port[6];
      char hostname[48];
      char fqdn[48];
    } softether;

    struct {
      char currency[16];
    } mining;

    struct {
      char *server_names, *advertised_alpns, *negotiated_alpn, *tls_supported_versions, *issuerDN, *subjectDN;
      u_int32_t notBefore, notAfter;
      char ja3_server[33], ja4_client[37], *ja4_client_raw;
      u_int16_t server_cipher;
      u_int8_t sha1_certificate_fingerprint[20];
      u_int8_t client_hello_processed:1, ch_direction:1, subprotocol_detected:1, server_hello_processed:1, fingerprint_set:1, webrtc:1, _pad:2;

#ifdef TLS_HANDLE_SIGNATURE_ALGORITMS
      /* Under #ifdef to save memory for those who do not need them */
      u_int8_t num_tls_signature_algorithms;
      u_int16_t client_signature_algorithms[MAX_NUM_TLS_SIGNATURE_ALGORITHMS];
#endif

      struct tls_heuristics browser_heuristics;

      u_int16_t ssl_version, server_names_len;

      struct {
        u_int16_t version;
      } encrypted_ch;

      ndpi_cipher_weakness server_unsafe_cipher;

      u_int32_t quic_version;
      u_int32_t quic_idle_timeout_sec;
    } tls_quic; /* Used also by DTLS and POPS/IMAPS/SMTPS/FTPS */

    struct {
      char client_signature[48], server_signature[48];
      char hassh_client[33], hassh_server[33];
    } ssh;

    struct {
      char filename[128];
    } tftp;

    struct {
      u_int8_t username_detected:1, username_found:1,
	password_detected:1, password_found:1,
	_pad:4;
      u_int8_t character_id;
      char username[32], password[32];
    } telnet;

    struct {
      char client_username[32];
      char server_username[32];
      char command[48];
    } rsh;

    struct {
      char client_username[32];
    } collectd;

    struct {
      char client_ip[16];
    } discord;

    struct {
      char version[64];
    } ubntac2;

    /* In TLS.Bittorent flows there is no hash.
       Nonetheless, we must pay attention to NOT write to /read from this field
       with these flows */
    struct {
      /* Bittorrent hash */
      u_char hash[20];
    } bittorrent;

    struct {
      char options[48];
      char fingerprint[48];
      char class_ident[48];
    } dhcp;

    struct {
      u_int8_t version;   /* 0 = SNMPv1, 1 = SNMPv2c, 3 = SNMPv3 */
      u_int8_t primitive; /* GET, SET... */
      u_int8_t error_status;
    } snmp;

    struct {
      char identity_uuid[37];
      char machine[48];
      char platform[32];
      char services[48];
    } tivoconnect;

    struct {
      u_int16_t result_code;
      u_int16_t internal_port;
      u_int16_t external_port;
      ndpi_ip_addr_t external_address;
    } natpmp;

    struct {
      u_int8_t message_type;
      char method[64];
    } thrift;

    struct {
      u_int8_t url_count;
      char url[4][48];
    } slp;

    struct {
      char *from;
      char from_imsi[16]; /* IMSI is 15 digit long, at most; + 1 for NULL terminator */
      char *to;
      char to_imsi[16];
    } sip;

    struct {
      char mac_addr[6], identity[16], version[48], sw_id[16], board[32], iface_name[32];
      u_int32_t ipv4_addr, uptime;
      struct ndpi_in6_addr ipv6_addr;
    } mikrotik;

    struct {
      char *method;
      char *bootid;
      char *usn;
      char *cache_controle;
      char *location;
      char *household_smart_speaker_audio;
      char *rincon_household;
      char *rincon_bootseq;
      char *rincon_wifimode;
      char *rincon_variant;
      char *sonos_securelocation;
      char *securelocation_upnp;
      char *location_smart_speaker_audio;
      char *nt;
      char *nts;
      char *server;
      char *man;
      char *mx;
      char *st;
      char *user_agent;
    } ssdp;

    struct {
      ndpi_http_method method;
      char user_agent[32];
      char url[64];
    } fast_cgi;

    struct {
      u_int32_t conference_id;
      u_int16_t user_id;
    } bfcp;

  } protos;

  /* **Packet** metadata for flows where monitoring is enabled. It is reset after each packet! */
  struct ndpi_metadata_monitoring *monit;

  NDPI_DISSECTOR_BITMASK excluded_dissectors_bitmask;

  /* NDPI_PROTOCOL_BITTORRENT */
  u_int32_t bittorrent_seq;
  u_int8_t bittorrent_stage;		      // can be 0 - 255
  u_int8_t bt_check_performed : 1;

  /* NDPI_PROTOCOL_ZATTOO */
  u_int8_t zattoo_stage:3;

  /* NDPI_PROTOCOL_OOKLA */
  u_int8_t ookla_stage : 1;

  /* NDPI_PROTOCOL_TEAMVIEWER */
  u_int8_t teamviewer_stage : 3;

  /* NDPI_PROTOCOL_OPENVPN */
  u_int8_t ovpn_session_id[2][8];
  u_int8_t ovpn_alg_standard_state : 2;
  u_int8_t ovpn_alg_heur_opcode_state : 2;
  u_int8_t ovpn_heur_opcode__codes_num : 4;
  u_int8_t ovpn_heur_opcode__num_msgs;
#define OPENVPN_HEUR_MAX_NUM_OPCODES 4
  u_int8_t ovpn_heur_opcode__codes[OPENVPN_HEUR_MAX_NUM_OPCODES];
  u_int8_t ovpn_heur_opcode__resets[2];
  u_int16_t ovpn_heur_opcode__missing_bytes[2];

  /* NDPI_PROTOCOL_TINC */
  u_int8_t tinc_state;

   /* NDPI_PROTOCOL_RTCP */
   u_int8_t rtcp_stage:2;

   /* NDPI_PROTOCOL_RTP */
   u_int8_t rtp_stage:2;
   u_int8_t rtp_seq_set[2];
   u_int16_t rtp_seq[2];

  /* Flow payload */
  u_int16_t flow_payload_len;
  char *flow_payload;

  /*
     Leave this field below at the end
     The field below can be used by third
     party dissectors for storing private data
   */
  u_int8_t priv_data[16];
};

#if !defined(NDPI_CFFI_PREPROCESSING) && defined(__linux__)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(((struct ndpi_flow_struct *)0)->protos) <= 264,
               "Size of the struct member protocols increased to more than 264 bytes, "
               "please check if this change is necessary.");
_Static_assert(sizeof(struct ndpi_flow_struct) <= 1232,
               "Size of the flow struct increased to more than 1232 bytes, "
               "please check if this change is necessary.");
#endif
#endif

#define NDPI_PROTOCOL_DEFAULT_LEVEL	0

typedef struct {
  char *string_to_match, *proto_name;
  u_int16_t protocol_id;
  ndpi_protocol_category_t protocol_category;
  ndpi_protocol_breed_t protocol_breed;
  int level; /* NDPI_PROTOCOL_DEFAULT_LEVEL (0) by default */
} ndpi_protocol_match;

typedef struct {
  u_int32_t network;
  u_int8_t cidr;
  u_int16_t value;
} ndpi_network;

typedef struct {
  char *network;
  u_int8_t cidr;
  u_int16_t value;
} ndpi_network6;

typedef struct {
  u_int32_t protocol_id;
  ndpi_protocol_category_t protocol_category;
  ndpi_protocol_breed_t protocol_breed;
} ndpi_protocol_match_result;

typedef enum {
  ndpi_serialization_format_unknown = 0,
  ndpi_serialization_format_tlv,
  ndpi_serialization_format_json,
  ndpi_serialization_format_csv,
  ndpi_serialization_format_multiline_json, /* new-line separated records */
  ndpi_serialization_format_inner_json /* no outer braces */
} ndpi_serialization_format;

/* Note:
 * - up to 16 types (TLV encoding: "4 bit key type" << 4 | "4 bit value type")
 * - key supports string and uint32 (compressed to uint8/uint16) only, this is also enforced by the API
 * - always add new enum at the end of the list (to avoid breaking backward compatibility) */
typedef enum {
  ndpi_serialization_unknown        =  0,
  ndpi_serialization_end_of_record  =  1,
  ndpi_serialization_uint8          =  2,
  ndpi_serialization_uint16         =  3,
  ndpi_serialization_uint32         =  4,
  ndpi_serialization_uint64         =  5,
  ndpi_serialization_int8           =  6,
  ndpi_serialization_int16          =  7,
  ndpi_serialization_int32          =  8,
  ndpi_serialization_int64          =  9,
  ndpi_serialization_float          = 10,
  ndpi_serialization_string         = 11,
  ndpi_serialization_start_of_block = 12,
  ndpi_serialization_end_of_block   = 13,
  ndpi_serialization_start_of_list  = 14,
  ndpi_serialization_end_of_list    = 15,
  /* Do not add new types!
   * Exceeding 16 types requires reworking the TLV encoding due to key type limit (4 bit) */
  ndpi_serialization_double         = 16 /* FIXX this is currently unusable */
} ndpi_serialization_type;

#define NDPI_SERIALIZER_DEFAULT_HEADER_SIZE 1024
#define NDPI_SERIALIZER_DEFAULT_BUFFER_SIZE  256
#define NDPI_SERIALIZER_DEFAULT_BUFFER_INCR 1024

#define NDPI_SERIALIZER_STATUS_COMMA     (1 << 0)
#define NDPI_SERIALIZER_STATUS_ARRAY     (1 << 1)
#define NDPI_SERIALIZER_STATUS_EOR       (1 << 2)
#define NDPI_SERIALIZER_STATUS_SOB       (1 << 3)
#define NDPI_SERIALIZER_STATUS_NOT_EMPTY (1 << 4)
#define NDPI_SERIALIZER_STATUS_LIST      (1 << 5)
#define NDPI_SERIALIZER_STATUS_SOL       (1 << 6)
#define NDPI_SERIALIZER_STATUS_HDR_DONE  (1 << 7)
#define NDPI_SERIALIZER_STATUS_CEOB      (1 << 8)

typedef struct {
  u_int32_t size_used;
} ndpi_private_serializer_buffer_status;

typedef struct {
  u_int32_t flags;
  ndpi_private_serializer_buffer_status buffer;
  ndpi_private_serializer_buffer_status header;
} ndpi_private_serializer_status;

typedef struct {
  u_int32_t initial_size;
  u_int32_t size;
  u_int8_t *data;
} ndpi_private_serializer_buffer;

typedef struct {
  ndpi_private_serializer_status status;
  ndpi_private_serializer_buffer buffer;
  ndpi_private_serializer_buffer header;
  ndpi_serialization_format fmt;
  char csv_separator[2];
  u_int8_t has_snapshot;
  u_int8_t multiline_json_array;
  u_int8_t inner_json;
  ndpi_private_serializer_status snapshot;
} ndpi_private_serializer;

#define ndpi_private_deserializer ndpi_private_serializer

#ifdef NDPI_CFFI_PREPROCESSING
typedef struct { char c[72]; } ndpi_serializer;
#else
typedef struct { char c[sizeof(ndpi_private_serializer)]; } ndpi_serializer;
#endif

#define ndpi_deserializer ndpi_serializer

typedef struct {
  char *str;
  u_int16_t str_len;
} ndpi_string;

/* **************************************** */

struct ndpi_analyze_struct {
  u_int64_t *values;
  u_int64_t min_val, max_val, sum_total, jitter_total;
  u_int32_t num_data_entries, next_value_insert_index;
  u_int16_t num_values_array_len /* length of the values array */;

  struct {
    u_int64_t sum_square_total;
  } stddev;
};

#define DEFAULT_SERIES_LEN  64
#define MAX_SERIES_LEN      512
#define MIN_SERIES_LEN      8

/* **************************************** */

struct ndpi_rsi_struct {
  u_int8_t empty:1, rsi_ready:1, _notused:6;
  u_int16_t num_values, next_index;
  u_int32_t *gains, *losses;
  u_int32_t last_value, total_gains, total_losses;
};

/* **************************************** */

struct ndpi_jitter_struct {
  u_int8_t empty:1, jitter_ready:1, _notused:6;
  u_int16_t num_values, next_index;
  float *observations, last_value, jitter_total;
};

/* **************************************** */

#ifndef AF_MAC
#define AF_MAC            99
#endif

typedef void (*ndpi_void_fn_t)(void *data);
typedef void (*ndpi_void_fn2_t)(ndpi_prefix_t *prefix, void *data);
typedef void (*ndpi_void_fn3_t)(ndpi_patricia_node_t *node, void *data, void *user_data);

/* **************************************** */

typedef struct ndpi_ptree
{
  ndpi_patricia_tree_t *v4;
  ndpi_patricia_tree_t *v6;
} ndpi_ptree_t;

/* **************************************** */

struct ndpi_hll {
  u_int8_t bits;
  size_t size;
  u_int8_t *registers;
};

struct ndpi_cm_sketch {
  u_int16_t num_hashes;       /* depth: Number of hash tables   */
  u_int32_t num_hash_buckets; /* Number pf nuckets of each hash */
  u_int32_t *tables;
};

struct ndpi_popcount {
  u_int64_t pop_count;       /* Number of bits set to 1 found so far */
  u_int64_t tot_bytes_count; /* Total number of bytes processed so far */
};

/* **************************************** */

enum ndpi_bin_family {
   ndpi_bin_family8,
   ndpi_bin_family16,
   ndpi_bin_family32,
   ndpi_bin_family64,

   kMaxValue = ndpi_bin_family64, /* To ease fuzzing */
};

struct ndpi_bin {
  u_int8_t is_empty;
  u_int16_t num_bins;
  enum ndpi_bin_family family;

  union {
    u_int8_t  *bins8; /* num_bins bins */
    u_int16_t *bins16; /* num_bins bins */
    u_int32_t *bins32; /* num_bins bins */
    u_int64_t *bins64; /* num_bins bins */
  } u;
};

/* **************************************** */

/* Implemented in third_party/src/kdtree.c */
typedef void ndpi_kd_tree;
typedef void ndpi_kd_tree_result;

/* Implemented in third_party/src/ball.c */
typedef void ndpi_btree;

typedef struct {
  double **distances;
  int    **indices;
  int    n_samples;
  int    n_neighbors;
} ndpi_knn;

/* **************************************** */

#define HW_HISTORY_LEN               4
#define MAX_SQUARE_ERROR_ITERATIONS 64 /* MUST be < num_values_rollup (256 max) */

struct ndpi_hw_struct {
  struct {
    u_int8_t use_hw_additive_seasonal;
    double alpha, beta, gamma, ro;
    u_int16_t num_season_periods; /* num of values of a season */
  } params;

  struct {
    double sum_square_error;
    u_int8_t num_values_rollup;
  } prev_error;

  u_int32_t num_values;
  double    u, v, sum_square_error;

  /* These two values need to store the signal history */
  u_int64_t *y;
  double    *s;
};

struct ndpi_ses_struct {
  struct {
    double alpha, ro;
  } params;

  struct {
    double sum_square_error;
    u_int8_t num_values_rollup;
  } prev_error;

  u_int32_t num_values;
  double sum_square_error, last_forecast, last_value;
};

struct ndpi_des_struct {
  struct {
    double alpha, beta, ro;
  } params;

  struct {
    double sum_square_error;
    u_int8_t num_values_rollup;
  } prev_error;

  u_int32_t num_values;
  double sum_square_error, last_forecast, last_trend, last_value;
};

/* **************************************** */

struct ndpi_address_cache_item {
  ndpi_ip_addr_t addr; /* key */
  char *hostname; /* value */
  u_int32_t expire_epoch;
  struct ndpi_address_cache_item *next; /* Linked list */
};

struct ndpi_address_cache {
  u_int32_t num_cached_addresses, num_root_nodes;
  u_int32_t num_entries, max_num_entries;
  struct ndpi_address_cache_item **address_cache_root;
};

/* **************************************** */

/* Prototype used to define custom DGA detection function */
typedef int (*ndpi_custom_dga_predict_fctn)(const char* domain, int domain_length);

/* **************************************** */

#endif /* __NDPI_TYPEDEFS_H__ */
