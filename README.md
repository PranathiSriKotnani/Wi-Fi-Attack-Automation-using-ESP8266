# Wi-Fi-Attack-Automation-using-ESP8266

📖 Project Description

This project explores the vulnerabilities in wireless networks by automating common Wi-Fi attacks using the ESP8266 microcontroller. These attacks include:

- Deauthentication Attack: A denial-of-service (DoS) attack that sends forged deauthentication packets to disconnect users from Wi-Fi networks, exploiting the unencrypted nature of 802.11 deauth frames.

- Beacon Flooding: The ESP8266 sends thousands of fake SSIDs to flood the surrounding area with illegitimate networks, potentially confusing users or overloading devices.

- Evil Twin Attack: A fake access point (AP) is created that mimics a legitimate Wi-Fi network to trick users into connecting, enabling Man-in-the-Middle (MITM) attacks.

Using libraries like ESP8266 Deauther, these attacks are implemented with a web interface or via direct packet crafting. The project emphasizes the ease of executing wireless attacks and the importance of Wi-Fi security protocols like WPA2/3, MAC filtering, and intrusion detection systems.

🛠️ Requirements
- ESP8266 NodeMCU (e.g., ESP-12E)

- Arduino IDE

- ESP8266 board support package

- Wi-Fi Deauther firmware (e.g., Spacehuhn’s ESP8266 Deauther)

- Micro-USB cable

⚙️ Setup Instructions
1. Install the Arduino IDE.

2. Add the ESP8266 board manager URL in preferences:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

3. Install the ESP8266 Deauther firmware or flash a custom script to send crafted packets.

4. Upload to ESP8266, connect to its Wi-Fi, and access the web interface (if using Deauther UI).

⚠️ Disclaimer
This project is intended only for ethical testing and educational use on networks you own or have permission to test.

