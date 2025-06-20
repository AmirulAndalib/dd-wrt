/*
 * riotgames.c
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_RIOTGAMES

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_int_riotgames_add_connection(struct ndpi_detection_module_struct * const ndpi_struct,
                                              struct ndpi_flow_struct * const flow)
{
  NDPI_LOG_INFO(ndpi_struct, "found RiotGames\n");
  ndpi_set_detected_protocol(ndpi_struct, flow,
                             NDPI_PROTOCOL_UNKNOWN,
                             NDPI_PROTOCOL_RIOTGAMES,
                             NDPI_CONFIDENCE_DPI);
}

static void ndpi_search_riotgames(struct ndpi_detection_module_struct *ndpi_struct,
                                  struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);

  NDPI_LOG_DBG(ndpi_struct, "searching RiotGames\n");

  if (packet->payload_packet_len > 8 &&
      ntohl(get_u_int32_t(packet->payload, packet->payload_packet_len - 8)) == 0xaaaaaaaa &&
      ntohl(get_u_int32_t(packet->payload, packet->payload_packet_len - 4)) == 0xbbbbbbbb)
  {
    ndpi_int_riotgames_add_connection(ndpi_struct, flow);
    return;
  }

  if (packet->payload_packet_len == 53 &&
      ntohl(get_u_int32_t(packet->payload, packet->payload_packet_len - 4)) == 0xea23460c &&
      ntohl(get_u_int32_t(packet->payload, packet->payload_packet_len - 8)) == 0x3cb11f2d)
  {
    ndpi_int_riotgames_add_connection(ndpi_struct, flow);
    return;
  }

  /*
   * Please add new patterns for games made by RiotGames here
   */

  NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);
  return;
}

void init_riotgames_dissector(struct ndpi_detection_module_struct *ndpi_struct)
{
  register_dissector("RiotGames", ndpi_struct,
                     ndpi_search_riotgames,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_UDP_WITH_PAYLOAD,
                     1, NDPI_PROTOCOL_RIOTGAMES);
}
