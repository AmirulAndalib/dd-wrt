/*
 * among_us.c
 *
 * Copyright (C) 2020 - ntop.org
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_AMONG_US

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_among_us_add_connection(struct ndpi_detection_module_struct *ndpi_struct,
                                             struct ndpi_flow_struct *flow)
{
  ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_AMONG_US, NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_among_us(struct ndpi_detection_module_struct *ndpi_struct,
                          struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct * const packet = ndpi_get_packet_struct(ndpi_struct);

  /* handshake packet */
  if (packet->payload_packet_len > 9 &&
      ntohl(*(u_int32_t*)&packet->payload[0]) == 0x08000100 &&
      ntohl(*(u_int32_t*)&packet->payload[4]) == 0x80d90203)
  {
    ndpi_int_among_us_add_connection(ndpi_struct, flow);
  } else {
    NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
  }
}

void init_among_us_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("AmongUs", ndpi_struct,
                     ndpi_search_among_us,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_UDP_WITH_PAYLOAD,
                     1, NDPI_PROTOCOL_AMONG_US);
}

