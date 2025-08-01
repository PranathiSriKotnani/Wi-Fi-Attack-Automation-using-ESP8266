# Wi-Fi-Attack-Automation-using-ESP8266

# 📖 Project Description

This project explores the vulnerabilities in wireless networks by automating common Wi-Fi attacks using the ESP8266 microcontroller. These attacks include:

- Deauthentication Attack: A denial-of-service (DoS) attack that sends forged deauthentication packets to disconnect users from Wi-Fi networks, exploiting the unencrypted nature of 802.11 deauth frames.

- Beacon Flooding: The ESP8266 sends thousands of fake SSIDs to flood the surrounding area with illegitimate networks, potentially confusing users or overloading devices.

- Evil Twin Attack: A fake access point (AP) is created that mimics a legitimate Wi-Fi network to trick users into connecting, enabling Man-in-the-Middle (MITM) attacks.

Using libraries like ESP8266 Deauther, these attacks are implemented with a web interface or via direct packet crafting. The project emphasizes the ease of executing wireless attacks and the importance of Wi-Fi security protocols like WPA2/3, MAC filtering, and intrusion detection systems.

## 🛠️ Requirements

- Hardware: ESP8266 NodeMCU

- Software: Arduino IDE

- URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`

- ESP8266 Libraries:

    - `ESP8266WiFi.h`

    - `ESP8266WebServer.h`

    - `DNSServer.h`

## ⚙️ Arduino Libraries Installation:

1. Open Arduino IDE → Preferences → Add the ESP8266 board URL to "Additional Board Manager URLs"

2. Go to Tools → Board → Boards Manager → Search and install "ESP8266 by ESP8266 Community"

3. Install libraries via Sketch → Include Library → Manage Libraries

⚠️ Disclaimer

 __Strictly for educational and ethical testing on networks you own or are authorized to audit.__
