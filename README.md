# CoolerController#
version 0.0.1


# Summary
- DIY Replacement for Coolbot with HTML based Controll and data published to io.adafruit.com
-
- When I started looking at options for a walk in cooler on my farm I stumbled across the CoolBot.
- While CoolBot is an interesting commercial product it is priced rather high for what it is.
- I them found another DIY project called ColdSnap. I believe the code can be found here on GITHUB.
- Coldsnap was closer to what I was looking for, but still didn't fit really fit my requirements.
- I have also started playing with the ESP-8266 ESP-12F recently. This is a low cost WIFI enabled microprocessor.
- When starting this project I borrowed heavily from the CaptivePortal Advanced Example included with the ESP8266 board definitions.
-
- The project uses three 18B20 Dallas One Wire Temp Probes to check temperatures for:
- Cooler Temperature
- Thermastat Heater Temperature
- Cooling Fin Temperature
- The project uses 2 50Ohm power resitors driven by 2 TIP41 Transitors from a 12V power supply.
-
- The idea of the project is to cool a walk-in cooler using a window unit A/C.
- To cool the Walk-In to lower temperatures than the window unit A/C norally cools to the two power resitors
- heat the themastat probe on the A/C to force it to run to a temp controlled by the processor.
- A temp probe is inserted into the fins to detect if the fins are freezing up.
- If temperatures below 32 degrees are detected at the fins the thermostat heater shuts off to allow the A/C tp defrost.
-
- I borrowed the idea for the power resitor heater from the ColdSnap Project, http://people.umass.edu/~dac/projects/ColdSnap/ColdSnap.html



# To Do List (pull requests are very welcomed)
- Save settings to eeprom when modified from Cooler Settings page
- Finish Fritzing Layout
- 


