/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
  char ok[2 + 1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  debugln("Recovered credentials:");
  debugln(ssid);
  debugln(strlen(password) > 0 ? "********" : "<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
  char ok[2 + 1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}

// EEPROM layout for cooler settings (starting at offset SETTINGS_EEPROM_OFFSET):
//   Byte 0: rmTmpSetPt
//   Byte 1: fimTmpSetPt
//   Byte 2: htrTmpSetPT
//   Byte 3: rmDeadBand
//   Byte 4: finDeadBand
//   Byte 5: htrDeadBand
//   Byte 6: htrPWMhigh
//   Byte 7: htrPWMlow
//   Bytes 8-9: sentinel "OK"
#define SETTINGS_EEPROM_OFFSET 100

/** Retrieve the values for the cooler from the EEPROM **/
void loadSettings() {
  EEPROM.begin(512);
  char ok[3] = {0};
  EEPROM.get(SETTINGS_EEPROM_OFFSET + 8, ok);
  if (String(ok) == String("OK")) {
    rmTmpSetPt  = EEPROM.read(SETTINGS_EEPROM_OFFSET);
    fimTmpSetPt = EEPROM.read(SETTINGS_EEPROM_OFFSET + 1);
    htrTmpSetPT = EEPROM.read(SETTINGS_EEPROM_OFFSET + 2);
    rmDeadBand  = EEPROM.read(SETTINGS_EEPROM_OFFSET + 3);
    finDeadBand = EEPROM.read(SETTINGS_EEPROM_OFFSET + 4);
    htrDeadBand = EEPROM.read(SETTINGS_EEPROM_OFFSET + 5);
    htrPWMhigh  = EEPROM.read(SETTINGS_EEPROM_OFFSET + 6);
    htrPWMlow   = EEPROM.read(SETTINGS_EEPROM_OFFSET + 7);
  }
  EEPROM.end();
  debugln("Cooler settings loaded from EEPROM");
} // end load settings

/** Save the values for the cooler to the EEPROM **/
void saveSettings() {
  EEPROM.begin(512);
  EEPROM.write(SETTINGS_EEPROM_OFFSET,     rmTmpSetPt);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 1, fimTmpSetPt);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 2, htrTmpSetPT);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 3, rmDeadBand);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 4, finDeadBand);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 5, htrDeadBand);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 6, htrPWMhigh);
  EEPROM.write(SETTINGS_EEPROM_OFFSET + 7, htrPWMlow);
  char ok[3] = "OK";
  EEPROM.put(SETTINGS_EEPROM_OFFSET + 8, ok);
  EEPROM.commit();
  EEPROM.end();
  debugln("Cooler settings saved to EEPROM");
} // end save settings

