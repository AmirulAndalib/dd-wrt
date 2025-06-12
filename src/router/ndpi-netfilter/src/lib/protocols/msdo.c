/*
 * msdo.c
 *
 * Microsoft Delivery Optimization
 * 
 * Copyright (C) 2025 - ntop.org
 * Copyright (C) 2025 - V.G <v.gavrilov@securitycode.ru>
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_MSDO

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_msdo_add_connection(struct ndpi_detection_module_struct * const ndpi_struct,
                                         struct ndpi_flow_struct * const flow)
{
  NDPI_LOG_INFO(ndpi_struct, "found Microsoft Delivery Optimization protocol\n");
  ndpi_set_detected_protocol(ndpi_struct, flow,
                             NDPI_PROTOCOL_MSDO,
                             NDPI_PROTOCOL_UNKNOWN,
                             NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_msdo(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct const * const packet = ndpi_get_packet_struct(ndpi_struct);

  NDPI_LOG_DBG(ndpi_struct, "search Microsoft Delivery Optimization protocol\n");

  /* 
   * Usually the handshake starts with 0x0E and the string "Swarm protocol" at the start of the payload, 
   * but this may vary.
   */
  if (packet->payload_packet_len > 16 && packet->payload[0] == 0x0E)
  {
    if (memcmp(&packet->payload[1], "Swarm protocol", NDPI_STATICSTRING_LEN("Swarm protocol")) == 0)
    {
      ndpi_int_msdo_add_connection(ndpi_struct, flow);
      return;
    }
  }

  NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
}


void init_msdo_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("MSDO", ndpi_struct,
                     ndpi_search_msdo,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                      1, NDPI_PROTOCOL_MSDO);
}
