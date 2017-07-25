#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

#include "ieee80211.h"
#include "esp8266_sniffer.h"

int32_t find_wifi_channel_for_ssid(const char* ssid) {
  int n = WiFi.scanNetworks();

  for (int i = 0; i < n; i++) {
    if (WiFi.SSID(i).equals(ssid)) {
      return WiFi.channel(i);
    }
  }

  return -1;
}

static int32_t g_wifi_channel = -1;

void wifi_rf_cb(uint8_t* buf, uint16_t len) {
  // Skip all non-management packets.
  if (len != sizeof(esp8266_mgmt_pkt_t))
    return;
    
  esp8266_mgmt_pkt_t* mgmt_pkt = reinterpret_cast<esp8266_mgmt_pkt_t*>(buf);
  ieee80211::Header* hdr = reinterpret_cast<ieee80211::Header*>(mgmt_pkt->buf);

  if (hdr->is_deauth()) {
    Serial.print("Deauth - To: ");
    Serial.printf(MACSTR, MAC2STR(hdr->addr1));
    Serial.print(" From: ");
    Serial.printf(MACSTR, MAC2STR(hdr->addr2));
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  while (g_wifi_channel == -1) {
    int32_t wifiChannel = find_wifi_channel_for_ssid("mknet");

    if (wifiChannel == -1) {
      delay(1000);
      Serial.print(".");
    } else {
      Serial.println();
      Serial.print("SSID found on channel ");
      Serial.println(wifiChannel);
      
      g_wifi_channel = wifiChannel;
    }
  }

  // Enable promiscious mode.
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(wifi_rf_cb);
  wifi_set_channel(g_wifi_channel);
  wifi_promiscuous_enable(1);
}

void loop() {
  
}
