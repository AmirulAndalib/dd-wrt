/*************************************************************************
 *
 * Copyright (C) 2018-2025 Ruilin Peng (Nick) <pymumu@gmail.com>.
 *
 * smartdns is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * smartdns is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DNS_CLIENT_QUIC_H_
#define _DNS_CLIENT_QUIC_H_

#include "dns_client.h"

#include <sys/epoll.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */
#ifdef HAVE_OPENSSL
int _dns_client_create_socket_quic(struct dns_server_info *server_info, const char *hostname, const char *alpn);

int _dns_client_send_quic(struct dns_query_struct *query, struct dns_server_info *server_info, void *packet,
						  unsigned short len);

int _dns_client_send_quic_data(struct dns_query_struct *query, struct dns_server_info *server_info, void *packet,
							   unsigned short len);

int _dns_client_process_quic(struct dns_server_info *server_info, struct epoll_event *event, unsigned long now);
#endif
#ifdef __cplusplus
}
#endif /*__cplusplus */
#endif
