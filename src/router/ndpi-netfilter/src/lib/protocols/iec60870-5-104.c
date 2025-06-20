/*
 * iec60870-5-104.c
 * Extension for industrial 104 protocol recognition
 *
 * Copyright (C) 2019-22 - ntop.org
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

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_IEC60870

#include "ndpi_api.h"
#include "ndpi_private.h"

static void ndpi_search_iec60870_tcp(struct ndpi_detection_module_struct *ndpi_struct,
                            struct ndpi_flow_struct *flow) {
  struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);

  /* Check connection over TCP */
  NDPI_LOG_DBG(ndpi_struct, "search IEC60870\n");

  if(packet->tcp) {
    u_int16_t offset = 0, found = 0;
    
    while(offset + 1 < packet->payload_packet_len) {
      /* The start byte of 104 is 0x68 */
      if(packet->payload[offset] == 0x68) {
	u_int8_t len = packet->payload[offset+1];
	
	if(len == 0) 
	  break;
	else {
	  
	  if((len + offset + 2) == packet->payload_packet_len) {
	    found = 1;
	    break;
	  }

	  offset += len + 2;
	}
      } else
	break;
    }
    
    if(found) {
      NDPI_LOG_INFO(ndpi_struct, "Found IEC60870-104\n");
      
      ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_IEC60870, NDPI_PROTOCOL_UNKNOWN, NDPI_CONFIDENCE_DPI);
      return;
    }
  }
  
  NDPI_EXCLUDE_DISSECTOR(ndpi_struct, flow);   
}


void init_104_dissector(struct ndpi_detection_module_struct *ndpi_struct) {
  register_dissector("IEC60870", ndpi_struct,
                     ndpi_search_iec60870_tcp,
                     NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                     1, NDPI_PROTOCOL_IEC60870);
}
