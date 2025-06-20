/*
 * postgres.c
 *
 * Copyright (C) 2009-11 - ipoque GmbH
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


#include "ndpi_protocol_ids.h"

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_POSTGRES

#include "ndpi_api.h"
#include "ndpi_private.h"


static void ndpi_int_postgres_add_connection(struct ndpi_detection_module_struct
					     *ndpi_struct, struct ndpi_flow_struct *flow)
{
  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_POSTGRES, NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_postgres_tcp(struct ndpi_detection_module_struct
				     *ndpi_struct, struct ndpi_flow_struct *flow)
{
	struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);

	if (flow->l4.tcp.postgres_stage == 0) {
		//SSL
		if (packet->payload_packet_len > 7 &&
			packet->payload[4] == 0x04 &&
			packet->payload[5] == 0xd2 &&
			packet->payload[6] == 0x16 &&
			packet->payload[7] == 0x2f && ntohl(get_u_int32_t(packet->payload, 0)) == packet->payload_packet_len) {
			flow->l4.tcp.postgres_stage = 1 + packet->packet_direction;
			return;
		}
		//no SSL
		if (packet->payload_packet_len > 7 &&
			//protocol version number - to be updated
			ntohl(get_u_int32_t(packet->payload, 4)) < 0x00040000 &&
			ntohl(get_u_int32_t(packet->payload, 0)) == packet->payload_packet_len) {
			flow->l4.tcp.postgres_stage = 3 + packet->packet_direction;
			return;
		}
		//GSS
		if (packet->payload_packet_len > 7 &&
			packet->payload[4] == 0x04 &&
			packet->payload[5] == 0xd2 &&
			packet->payload[6] == 0x16 &&
			packet->payload[7] == 0x30 &&
			ntohl(get_u_int32_t(packet->payload, 0)) == packet->payload_packet_len) {
			flow->l4.tcp.postgres_stage = 5 + packet->packet_direction;
			return;
		}
	} else {
		if (flow->l4.tcp.postgres_stage == 2 - packet->packet_direction) {
			//SSL accepted
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'S') {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, SSL accepted\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
			//SSL denied
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'N') {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, SSL denied\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
		}
		//no SSL
		if (flow->l4.tcp.postgres_stage == 4 - packet->packet_direction) {
			if (packet->payload_packet_len > 8 &&
				ntohl(get_u_int32_t(packet->payload, 5)) < 10 &&
				ntohl(get_u_int32_t(packet->payload, 1)) == (uint32_t)packet->payload_packet_len - 1 && packet->payload[0] == 0x52) {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, no SSL\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
			if (packet->payload_packet_len > 8 &&
				ntohl(get_u_int32_t(packet->payload, 5)) == 0 &&
				ntohl(get_u_int32_t(packet->payload, 1)) == 8 && packet->payload[0] == 0x52) {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, no SSL, auth succ, multiple msg\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
		}
		//GSS
		if (flow->l4.tcp.postgres_stage == 6 - packet->packet_direction) {
			//GSS accepted
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'G') {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, GSS accepted\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
			//GSS denied
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'N') {
				NDPI_LOG_INFO(ndpi_struct, "PostgreSQL detected, GSS denied\n");
				ndpi_int_postgres_add_connection(ndpi_struct, flow);
				return;
			}
		}
	}

	NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
}


void init_postgres_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("PostgreSQL", ndpi_struct,
                     ndpi_search_postgres_tcp,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                     1, NDPI_PROTOCOL_POSTGRES);
}

