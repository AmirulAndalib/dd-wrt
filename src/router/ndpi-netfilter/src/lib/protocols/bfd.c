/*
 * bfd.c
 *
 * Bidirectional Forwarding Detection
 * 
 * Copyright (C) 2024 - ntop.org
 * Copyright (C) 2024 - V.G <v.gavrilov@securitycode.ru>
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_BFD

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_bfd_add_connection(struct ndpi_detection_module_struct *ndpi_struct, 
                                             struct ndpi_flow_struct *flow)
{
  NDPI_LOG_INFO(ndpi_struct, "found BFD\n");
  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_BFD,
                             NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_bfd(struct ndpi_detection_module_struct *ndpi_struct,
                            struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct const * const packet = ndpi_get_packet_struct(ndpi_struct);

  /* BFD echo message */
  if (packet->payload_packet_len == 12 && ntohs(packet->udp->dest) == 3785) {
    if (ndpi_ntohll(get_u_int64_t(packet->payload, 0)) == 1 && 
        ntohs(get_u_int16_t(packet->payload, packet->payload_packet_len-2)) == 0)
    {
      ndpi_int_bfd_add_connection(ndpi_struct, flow);
      return;
    }
  }

  /* BFD control message */
  if ((packet->payload_packet_len > 23 && packet->payload_packet_len <= 100) && 
      ntohs(packet->udp->dest) == 3784)
  {
    if (((packet->payload[0] & 0xE0) >> 5) < 2 &&
        packet->payload[3] == (u_int8_t)packet->payload_packet_len)
    {
      ndpi_int_bfd_add_connection(ndpi_struct, flow);
      return;
    }
  }

  NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
}
void init_bfd_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("BFD", ndpi_struct,
                     ndpi_search_bfd,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_UDP_WITH_PAYLOAD,
                     1, NDPI_PROTOCOL_BFD);
}
