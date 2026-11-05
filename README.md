# awesome repo generator 9000

an ESP32 gadget that shows a random github repo from the `topic:awesome` list on an OLED. press the button to reroll.

<img width="4032" height="1908" alt="image" src="https://github.com/user-attachments/assets/864b6fea-e825-46ae-8f92-b94e8c53dcef" />

## hardware

- ESP32 board
- SSD1306 OLED (128×64, I2C, `0x3C`)
- push button on GPIO `4` (pull-down)

**wiring:** 

* OLED `SDA` -> GPIO `21`
* OLED `SCL` -> GPIO `22`
* OLED `VCC` -> 3.3V
* OLED `GND` -> GND
* button leg 1 (top left) -> 3.3V
* button leg 4 (bottom right) -> GPIO `4`
## setup

1. install libs: `Adafruit GFX`, `Adafruit SSD1306`, `ArduinoJson`
2. edit **lines 15 & 16** in `awesome-repo-gen.ino` with your wifi ssid and password
3. flash to your ESP32, open Serial Monitor at `115200` to watch it boot

## license

whatevs

## wiring diagram (bad)
<img width="1255" height="772" alt="image" src="https://github.com/user-attachments/assets/fbfc991a-8c8e-47ad-97c0-8e5b817f9f8e" />
