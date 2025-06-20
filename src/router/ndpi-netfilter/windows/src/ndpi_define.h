/*
 *
 * Copyright (C) 2011-17 - ntop.org
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

#ifndef __NDPI_DEFINE_INCLUDE_FILE__
#define __NDPI_DEFINE_INCLUDE_FILE__

/*
 * Not supported for Visual Studio.
 */
#define NDPI_API_VERSION 0

/*
  gcc -E -dM - < /dev/null |grep ENDIAN
*/

#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <sys/endian.h>
#endif

#ifdef __OpenBSD__
#include <endian.h>
#define __BYTE_ORDER BYTE_ORDER
#if BYTE_ORDER == LITTLE_ENDIAN
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif /* __LITTLE_ENDIAN__ */
#else
#define __BIG_ENDIAN__
#endif/* BYTE_ORDER */
#endif/* __OPENBSD__ */

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 0
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#else
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__
#endif
#endif

#ifdef WIN32
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#endif

#if !(defined(__LITTLE_ENDIAN__) || defined(__BIG_ENDIAN__))
#if defined(__mips__)
#undef __LITTLE_ENDIAN__
#undef __LITTLE_ENDIAN
#define __BIG_ENDIAN__
#endif

/* Everything else */
#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__))
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#else
#define __BIG_ENDIAN__
#endif
#endif

#endif

#define NDPI_SELECTION_BITMASK_PROTOCOL_SIZE			u_int32_t

#define MAX_DEFAULT_PORTS                                        5

#define NDPI_EXCLUDE_DISSECTOR(mod,flow) exclude_dissector(mod, flow, mod->current_dissector_idx, __FILE__, __FUNCTION__, __LINE__)

/**
 * macro for getting the string len of a static string
 *
 * use it instead of strlen to avoid runtime calculations
 */
#define NDPI_STATICSTRING_LEN( s ) ( sizeof( s ) - 1 )

/** macro to compare 2 IPv6 addresses with each other to identify the "smaller" IPv6 address  */
#define NDPI_COMPARE_IPV6_ADDRESS_STRUCTS(x,y)  \
  ((x.u6_addr.u6_addr64[0] < y.u6_addr.u6_addr64[0]) || ((x.u6_addr.u6_addr64[0] == y.u6_addr.u6_addr64[0]) && (x.u6_addr.u6_addr64[1] < y.u6_addr.u6_addr64[1])))

#define NDPI_NUM_BITS              512
#define NDPI_NUM_BITS_MASK         (512-1)

#define NDPI_BITS /* 32 */ (sizeof(ndpi_ndpi_mask) * 8 /* number of bits in a byte */)        /* bits per mask */
#define howmanybits(x, y)   (((x)+((y)-1))/(y))


#define NDPI_SET(p, n)    ((p)->fds_bits[(n)/NDPI_BITS] |=  (1ul << (((u_int32_t)n) % NDPI_BITS)))
#define NDPI_CLR(p, n)    ((p)->fds_bits[(n)/NDPI_BITS] &= ~(1ul << (((u_int32_t)n) % NDPI_BITS)))
#define NDPI_ISSET(p, n)  ((p)->fds_bits[(n)/NDPI_BITS] &   (1ul << (((u_int32_t)n) % NDPI_BITS)))
#define NDPI_ZERO(p)      memset((char *)(p), 0, sizeof(*(p)))
#define NDPI_ONE(p)       memset((char *)(p), 0xFF, sizeof(*(p)))

#define NDPI_NUM_FDS_BITS     howmanybits(NDPI_NUM_BITS, NDPI_BITS)

#define NDPI_PROTOCOL_BITMASK ndpi_protocol_bitmask_struct_t

#define NDPI_BITMASK_ADD(a,b)     NDPI_SET(&a,b)
#define NDPI_BITMASK_DEL(a,b)     NDPI_CLR(&a,b)
#define NDPI_BITMASK_RESET(a)     NDPI_ZERO(&a)
#define NDPI_BITMASK_SET_ALL(a)   NDPI_ONE(&a)
#define NDPI_BITMASK_SET(a, b)    { memcpy(&a, &b, sizeof(NDPI_PROTOCOL_BITMASK)); }

#define NDPI_SET_BIT(num, n)    num |= 1ULL << ( n )
#define NDPI_CLR_BIT(num, n)    num &= ~(1ULL << ( n ))
#define NDPI_CLR_BIT(num, n)    num &= ~(1ULL << ( n ))
#define NDPI_ISSET_BIT(num, n)  (num & (1ULL << ( n )))
#define NDPI_ZERO_BIT(num)      num = 0

#define NDPI_MAX_NUM_DISSECTORS                 288      /* Multiple of 32, i.e. NDPI_BITS */
#define NDPI_NUM_FDS_BITS_DISSECTORS            howmanybits(NDPI_MAX_NUM_DISSECTORS, NDPI_BITS)
#define NDPI_DISSECTOR_BITMASK                  ndpi_dissector_bitmask_struct_t
#define NDPI_DISSECTOR_BITMASK_IS_SET(p, n)     NDPI_ISSET(&(p), (n))
#define NDPI_DISSECTOR_BITMASK_SET(p, n)        NDPI_SET(&(p), (n))

/* this is a very very tricky macro *g*,
 * the compiler will remove all shifts here if the protocol is static...
 */
#define NDPI_ADD_PROTOCOL_TO_BITMASK(bmask,value)     NDPI_SET(&bmask,   value & NDPI_NUM_BITS_MASK)
#define NDPI_DEL_PROTOCOL_FROM_BITMASK(bmask,value)   NDPI_CLR(&bmask,   value & NDPI_NUM_BITS_MASK)
#define NDPI_COMPARE_PROTOCOL_TO_BITMASK(bmask,value) NDPI_ISSET(&bmask, value & NDPI_NUM_BITS_MASK)

#define NDPI_SAVE_AS_BITMASK(bmask,value)  { NDPI_ZERO(&bmask) ; NDPI_ADD_PROTOCOL_TO_BITMASK(bmask, value); }


#define ndpi_min(a,b)   ((a < b) ? a : b)
#define ndpi_max(a,b)   ((a > b) ? a : b)

#define NDPI_NUM_DEFAULT_ROOT_NODES        512

#define NDPI_IPSEC_PROTOCOL_ESP    50
#define NDPI_IPSEC_PROTOCOL_AH     51
#define NDPI_GRE_PROTOCOL_TYPE     0x2F
#define NDPI_ICMP_PROTOCOL_TYPE    0x01
#define NDPI_IGMP_PROTOCOL_TYPE    0x02
#define NDPI_EGP_PROTOCOL_TYPE     0x08
#define NDPI_OSPF_PROTOCOL_TYPE    0x59
#define NDPI_VRRP_PROTOCOL_TYPE    112
#define NDPI_SCTP_PROTOCOL_TYPE    132
#define NDPI_IPIP_PROTOCOL_TYPE    0x04
#define NDPI_ICMPV6_PROTOCOL_TYPE  0x3a
#define NDPI_PGM_PROTOCOL_TYPE     0x71
#define NDPI_PIM_PROTOCOL_TYPE     0x67

#define NDPI_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

/* the get_uXX will return raw network packet bytes !! */
#define get_u_int8_t(X,O)   (*(u_int8_t  *)((&(((u_int8_t *)X)[O]))))
#define get_u_int16_t(X,O)  (*(u_int16_t *)((&(((u_int8_t *)X)[O]))))
#define get_u_int32_t(X,O)  (*(u_int32_t *)((&(((u_int8_t *)X)[O]))))
#if defined(__arm__)
static inline u_int64_t get_u_int64_t(const u_int8_t* X, int O)
{
  u_int64_t tmp;
  memcpy(&tmp, X + O, sizeof(tmp));
  return tmp;
}
#else
#define get_u_int64_t(X,O)  (*(u_int64_t *)((&(((u_int8_t *)X)[O]))))
#endif // __arm__

/* new definitions to get little endian from network bytes */
#define get_ul8(X,O) get_u_int8_t(X,O)

#if defined(__LITTLE_ENDIAN__) || defined(_LITTLE_ENDIAN)
#define get_l16(X,O)  get_u_int16_t(X,O)
#define get_l32(X,O)  get_u_int32_t(X,O)
#elif defined(__BIG_ENDIAN__) || defined(__BIG_ENDIAN)
/* convert the bytes from big to little endian */
# define get_l16(X,O) bswap_16(get_u_int16_t(X,O))
# define get_l32(X,O) bswap_32(get_u_int32_t(X,O))
#else
#error "__BYTE_ORDER MUST BE DEFINED !"
#endif							/* __BYTE_ORDER */

/* define memory callback function */
#define match_first_bytes(payload,st) (memcmp((payload),(st),(sizeof(st)-1))==0)

#if defined(WIN32)
#undef strtok_r
#define strtok_r strtok_s

#if BYTE_ORDER == LITTLE_ENDIAN
#define le16toh(x) (x)
#define le32toh(x) (x)
#else
#error "byte order not supported"
#endif

#endif /* WIN32 */

/*
 * Not supported for Visual Studio.
 */
#define NDPI_MAJOR                              0
#define NDPI_MINOR                              0
#define NDPI_PATCH                              0

#define NDPI_MAX_DNS_REQUESTS                   48
#define NDPI_MIN_NUM_STUN_DETECTION             8

/* IMPORTANT: order according to its severity */
#define NDPI_CIPHER_SAFE                        0
#define NDPI_CIPHER_WEAK                        1
#define NDPI_CIPHER_INSECURE                    2

#define NDPI_OPTIMAL_HLL_NUM_BUCKETS           16

#define NDPI_MAX_NUM_TLS_APPL_BLOCKS            8

//#define TLS_HANDLE_SIGNATURE_ALGORITMS 1

#ifdef __APPLE__

#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#endif /* __APPLE__ */


#if defined(_WINDOWS_)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define htobe16(x) htons(x)
#define htole16(x) (x)
#define be16toh(x) ntohs(x)
#define le16toh(x) (x)
#define htobe32(x) htonl(x)
#define htole32(x) (x)
#define be32toh(x) ntohl(x)
#define le32toh(x) (x)
#define htobe64(x) ndpi_htonll(x)
#define htole64(x) (x)
#define be64toh(x) ndpi_ntohll(x)
#define le64toh(x) (x)

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define htobe16(x) (x)
#define htole16(x) __builtin_bswap16(x)
#define be16toh(x) (x)
#define le16toh(x) __builtin_bswap16(x)
#define htobe32(x) (x)
#define htole32(x) __builtin_bswap32(x)
#define be32toh(x) (x)
#define le32toh(x) __builtin_bswap32(x)
#define htobe64(x) (x)
#define htole64(x) __builtin_bswap64(x)
#define be64toh(x) (x)
#define le64toh(x) __builtin_bswap64(x)

#else
#error Unexpected __BYTE_ORDER__

#endif /* __BYTE_ORDER__ */
#endif /* __MINGW32__ */


#ifndef ETH_ARP
#define ETH_ARP                0x0806
#endif

#ifndef ETH_P_IP
#define ETH_P_IP               0x0800 	/* IPv4 */
#endif

#ifndef ETH_P_IPV6
#define ETH_P_IPV6	       0x86dd	/* IPv6 */
#endif

#ifndef ETH_P_VLAN
#define ETH_P_VLAN             0x8100
#endif

#ifndef ETH_P_MPLS_UNI
#define ETH_P_MPLS_UNI         0x8847
#endif

#ifndef ETH_P_MPLS_MULTI
#define ETH_P_MPLS_MULTI       0x8848
#endif

#ifndef ETH_P_PPPoE
#define ETH_P_PPPoE            0x8864
#endif

#define MAX_NBPF_CUSTOM_PROTO  8

#endif /* __NDPI_DEFINE_INCLUDE_FILE__ */
