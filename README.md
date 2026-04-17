# CoolerController

[![Arduino CI](https://github.com/bnorth12/CoolerController/actions/workflows/ci.yml/badge.svg)](https://github.com/bnorth12/CoolerController/actions/workflows/ci.yml)

version 0.1.0

## Summary

DIY replacement for the CoolBot, with an HTML-based control interface and data published to [io.adafruit.com](https://io.adafruit.com) via MQTT.

When I started looking at options for a walk-in cooler on my farm I came across the CoolBot.
While CoolBot is an interesting commercial product it is priced rather high for what it is.
I then found another DIY project called ColdSnap (code on GitHub).
ColdSnap was closer to what I was looking for, but still didn't quite fit my requirements.
I have also started playing with the ESP8266 ESP-12F, a low-cost Wi-Fi enabled microprocessor.
When starting this project I borrowed heavily from the CaptivePortal Advanced Example included with the ESP8266 board definitions.

The project uses three DS18B20 Dallas OneWire temperature probes to measure:

- **Cooler Temperature** — room/box air temperature
- **Thermostat Heater Temperature** — A/C thermostat probe temperature
- **Cooling Fin Temperature** — A/C evaporator fin temperature

Two 50 Ω power resistors driven by TIP41 transistors from a 12 V supply act as a heater clamped to
the A/C thermostat probe. By heating that probe the processor can force the A/C to run to
temperatures lower than it would normally allow.

A temperature probe inserted into the fins detects if they are freezing up.
If fin temperature falls below 32 °F the thermostat heater shuts off so the A/C can defrost.

The resistor-heater idea was borrowed from the [ColdSnap project](http://people.umass.edu/~dac/projects/ColdSnap/ColdSnap.html).

---

## Getting Started

### Prerequisites

- [Arduino CLI](https://arduino.github.io/arduino-cli/) **or** Arduino IDE 2.x
- ESP8266 board support package
- The three libraries listed in `sketch.yaml` (OneWire, DallasTemperature, Adafruit MQTT Library)

### Configuration

1. **Copy the secrets template and fill in your credentials:**

   ```bash
   cp secrets.h.example secrets.h
   ```

   Edit `secrets.h` with your Wi-Fi SSID/password and Adafruit IO username/key.
   `secrets.h` is listed in `.gitignore` and will **never** be committed.

2. **Update the sensor probe addresses** in `CoolerController.ino` (`rmProbe`, `finProbe`, `htrProbe`)
   to match your own DS18B20 sensors.

3. **Compile and upload** using the Arduino CLI:

   ```bash
   arduino-cli compile --fqbn esp8266:esp8266:generic CoolerController/CoolerController.ino
   arduino-cli upload  --fqbn esp8266:esp8266:generic --port /dev/ttyUSB0 CoolerController/CoolerController.ino
   ```

   Or open the sketch in Arduino IDE 2.x, select *ESP8266 Generic*, and click **Upload**.

### First Boot

1. Connect to the `ESP_ap` Wi-Fi network (password: `12345678`).
2. Navigate to `http://192.168.4.1/wifi` and enter your home Wi-Fi credentials.
3. The ESP8266 will connect and be reachable at its assigned IP address or `http://esp8266.local`.
4. Visit `http://esp8266.local/` to see current temperatures and state.
5. Visit `http://esp8266.local/para` to adjust set points and dead bands (values persist across reboots).

---

## Hardware

See the wiring diagram in the `Fritzing/` folder.

| Component | Details |
|---|---|
| Microcontroller | ESP8266 ESP-12F |
| Temperature sensors | DS18B20 OneWire (×3) |
| Heater resistors | 50 Ω power resistors (×2) |
| Heater drivers | TIP41 NPN transistors (×2) |
| Supply | 12 V DC |

---

## Known Issues / To Do

- [ ] Finish Fritzing layout
- [ ] Test heater logic end-to-end with real hardware
- [ ] Tune `htrPWMhigh` and `htrPWMlow` PWM values for optimal heater performance
- [ ] Change 30-second MQTT interval back to 5 minutes (`rgFiveMin = 300000`) for production use
- [ ] Add door-sensor support (pin defined but not yet wired into control logic)

Pull requests are very welcome!

