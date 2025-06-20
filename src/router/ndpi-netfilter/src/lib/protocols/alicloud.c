/*
 * alicloud.c
 *
 * Copyright (C) 2022-23 - ntop.org
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_ALICLOUD

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_alicloud_add_connection(struct ndpi_detection_module_struct * const ndpi_struct,
                                             struct ndpi_flow_struct * const flow)
{
  NDPI_LOG_INFO(ndpi_struct, "found alicloud\n");

  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_ALICLOUD, NDPI_PROTOCOL_UNKNOWN,
                             NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_alicloud(struct ndpi_detection_module_struct *ndpi_struct,
                                 struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct const * const packet = ndpi_get_packet_struct(ndpi_struct);

  NDPI_LOG_DBG(ndpi_struct, "search alicloud\n");

  if (packet->payload_packet_len < 8)
  {
    NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
    return;
  }

  if (ntohl(get_u_int32_t(packet->payload, 0)) == 0xcefabeba)
  {
    u_int32_t pdu_len = ntohl(get_u_int32_t(packet->payload, 4));

    if (packet->payload_packet_len == 8 && pdu_len > 0)
    {
      ndpi_int_alicloud_add_connection(ndpi_struct, flow);
      return;
    } else if (pdu_len == (u_int32_t)packet->payload_packet_len - 8) {
      ndpi_int_alicloud_add_connection(ndpi_struct, flow);
      return;
    }
  }

  if (flow->packet_counter > 3)
  {
    NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
  }
}

void init_alicloud_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("AliCloud", ndpi_struct,
                     ndpi_search_alicloud,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                     1, NDPI_PROTOCOL_ALICLOUD);
}
