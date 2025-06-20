/*
 * nintendo.c
 *
 * Copyright (C) 2017-18 by ntop.org
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_NINTENDO

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_nintendo_add_connection(struct ndpi_detection_module_struct *ndpi_struct,
					     struct ndpi_flow_struct *flow) {
  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_NINTENDO, NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
}


static void ndpi_search_nintendo(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow) {
  struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);
  u_int32_t payload_len = packet->payload_packet_len;

  if(packet->udp != NULL) {
    if(payload_len > 48) {
      const char *payload = (const char *)packet->payload;
      const char nintendo_pattern[] = { 0x32, 0xab, 0x98, 0x64, 0x02 };

      if(memcmp(payload, nintendo_pattern, 5) == 0) {
	NDPI_LOG_INFO(ndpi_struct, "found nintendo\n");
	ndpi_int_nintendo_add_connection(ndpi_struct, flow);
	return;
      }
    }
  }

  NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
}

void init_nintendo_dissector(struct ndpi_detection_module_struct *ndpi_struct) {
  register_dissector("Nintendo", ndpi_struct,
                     ndpi_search_nintendo,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_UDP_WITH_PAYLOAD,
                     1, NDPI_PROTOCOL_NINTENDO);
}

