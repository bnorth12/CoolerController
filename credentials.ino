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

/** Retrieve the values for the cooler from the EEPROM **/
void loadSettings() {

//  rmTmpSetPt = rmTmpSetPt;
//  fimTmpSetPt = fimTmpSetPt;
//  htrTmpSetPT = htrTmpSetPT;
//  rmDeadBand = rmDeadBand;
//  finDeadBand = finDeadBand;
//  htrDeadBand = htrDeadBand;
//  htrPMWhigh = htrPMWhigh;
//  htrPMWlow = htrPMWlow;

}// end load settings

/** Retrieve the values for the cooler from the EEPROM **/
void saveSettings() {

//  rmTmpSetPt = rmTmpSetPt;
//  fimTmpSetPt = fimTmpSetPt;
//  htrTmpSetPT = htrTmpSetPT;
//  rmDeadBand = rmDeadBand;
//  finDeadBand = finDeadBand;
//  htrDeadBand = htrDeadBand;
//  htrPMWhigh = htrPMWhigh;
//  htrPMWlow = htrPMWlow;

} // end save settings

