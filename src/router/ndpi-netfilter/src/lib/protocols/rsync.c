/*
 * rsync.c
 *
 * Copyright (C) 2013 Remy Mudingay <mudingay@ill.fr>
 * Copyright (C) 2016-18 ntop.org
 *
 * This module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ndpi_protocol_ids.h"

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_RSYNC

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_rsync_add_connection(struct ndpi_detection_module_struct
					  *ndpi_struct, struct ndpi_flow_struct *flow)
{
  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_RSYNC, NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_rsync(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);

  NDPI_LOG_DBG(ndpi_struct, "search RSYNC\n");

  if(packet->tcp) {
    NDPI_LOG_DBG2(ndpi_struct, "calculating RSYNC over tcp\n");
    /*
     * Should match: memcmp(packet->payload, "@RSYNCD:", 8) == 0)
     */
    if (packet->payload_packet_len >= 8 && packet->payload[0] == 0x40 &&
	packet->payload[1] == 0x52 && packet->payload[2] == 0x53 &&
	packet->payload[3] == 0x59 && packet->payload[4] == 0x4e &&
	packet->payload[5] == 0x43 && packet->payload[6] == 0x44 &&
	packet->payload[7] == 0x3a ) {
      NDPI_LOG_INFO(ndpi_struct, "found rsync\n");
      ndpi_int_rsync_add_connection(ndpi_struct, flow);
      return;
    }
  }
  if(flow->packet_counter > 5)
    NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
}


void init_rsync_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("RSYNC", ndpi_struct,
                     ndpi_search_rsync,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                     1, NDPI_PROTOCOL_RSYNC);
}
