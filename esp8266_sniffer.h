#ifndef __ESP8266_SNIFFER_H
#define __ESP8266_SNIFFER_H

// This structure is documented in the ESP8266 Technical Reference document
// in the “Sniffer Introduction” chapter.
struct RxControl {
  signed rssi : 8; // signal intensity of packet
  unsigned rate : 4;
  unsigned is_group : 1;
  unsigned : 1;
  unsigned sig_mode : 2; // 0:is not 11n packet; non-0:is 11n packet;
  unsigned legacy_length : 12; // if not 11n packet, shows length of packet.
  unsigned damatch0 : 1;
  unsigned damatch1 : 1; 
  unsigned bssidmatch0 : 1;
  unsigned bssidmatch1 : 1;
  unsigned MCS : 7; // if is 11n packet, shows the modulation
                    // and code used (range from 0 to 76)
  unsigned CWB : 1; // if is 11n packet, shows if is HT40 packet or not
  unsigned HT_length : 16; // if is 11n packet, shows length of packet.
  unsigned Smoothing : 1;
  unsigned Not_Sounding : 1;
  unsigned : 1;
  unsigned Aggregation : 1;
  unsigned STBC : 2;
  unsigned FEC_CODING : 1; // if is 11n packet, shows if is LDPC packet or not.
  unsigned SGI : 1;
  unsigned rxend_state : 8;
  unsigned ampdu_cnt : 8;
  unsigned channel : 4; // which channel this packet in.
  unsigned : 12;
};

typedef struct esp8266_mgmt_pkt {
  struct RxControl rx_ctrl;
  uint8_t buf[112]; // IEEE 802.11 frame data.
  uint16_t packet_count; // Number of packets.
  uint16_t packet_len; // Packet length.
} esp8266_mgmt_pkt_t;

#endif
