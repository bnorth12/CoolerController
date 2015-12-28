
/**********************************************************************************************************************/
/*
 * requestTemps Function called at 1 HZ from Loop. This function commands all probes to measure temp and report internally at probe.
 */
/**********************************************************************************************************************/
void requestTemps() {  //check the temps for the three probes
  sensors.requestTemperatures(); // Send the command to get temperatures
} // end requestTemps


/**********************************************************************************************************************/
/*
 *
 */
/**********************************************************************************************************************/
void setTemps() {
  htrTemp = setTempValues(htrProbe); // Heater Temperature equal to current value
  finTemp = setTempValues(finProbe); // Room Temperature equal to current value
  rmTemp =   setTempValues(rmProbe); // Room Temperature equal to current value
} // end setTemps

/**********************************************************************************************************************/
/*
 * setTempValues retreives values from probes and stores temps in variables.
 */
/**********************************************************************************************************************/
int setTempValues(DeviceAddress deviceAddress) {

  byte tempF = 0;
  float tempC = sensors.getTempC(deviceAddress);

  if (tempC == -127.00) // Measurement failed or no device found
  {
    debugln("Temperature Error");
  }
  else
  {
    tempF = DallasTemperature::toFahrenheit(tempC); // Convert to F
    return tempF;
  }

} //end setTempValues

/**********************************************************************************************************************/
/*
 *Set the Heater State based on the information on hand
 */
/**********************************************************************************************************************/
void setHeaterState() {

  /*
   * The goal of this function is to set the heater output. Set the values for the variable then set the PWM value based on coolCycle and defrost cycle states:
   */

  // Set Cool Cycle Boolean Variable
  if (!coolCycleOn && (rmTemp >= (rmTmpSetPt + rmDeadBand))) {
    coolCycleOn = true;
  }
  else if (coolCycleOn && (rmTemp <= rmTmpSetPt)) {
    coolCycleOn = false;
  } //end of cool cycle evaluation

  // Set Defrost Cycle Boolean Variable
  if (!defrostCycleOn && (finTemp <= fimTmpSetPt)) {
    defrostCycleOn = true;
  }
  else if (defrostCycleOn && (finTemp >= (fimTmpSetPt + finDeadBand))) {
    coolCycleOn = false;
  } //end of defrost cycle evaluation

  // Set max heater temp Boolean Variable
  if (htrTemp >= (htrTmpSetPT + htrDeadBand)) {
    atMaxHtrTmp = true;
  }
  else if (atMaxHtrTmp && (htrTemp <= htrTmpSetPT)) {
    atMaxHtrTmp = false;
  } //end of  max heater temp evaluation

  // Set Heater PWM
  if (!coolCycleOn) {
    htrPWM = htrPMWlow;
  } // cool cycle is off

  if (defrostCycleOn || atMaxHtrTmp) {
    htrPWM = htrPMWlow;
  } // heater off due to defrost cycle or max heater temp
  else if (coolCycleOn) {
    htrPWM = htrPMWhigh;
  }
  else {
    htrPWM = htrPMWhigh;
  } // end of setting heater PWM variables

  analogWrite(htrPin1, htrPWM); // Set PWM output for Heater1
  analogWrite(htrPin2, htrPWM); // Set PWM output for Heater2

#ifdef DEBUG

  debugln("Values from Conrol Logic ");
  debug("Current Cooler Temp ");
  debugln(rmTemp);
  debug("Cooler Set Point ");
  debugln(rmTmpSetPt);
  debugln(" ");
  debug("Current Heater Temp ");
  debugln(htrTemp);
  debug("Current Heater PWM ");
  debugln(htrPWM);
  debug("Heater Set Point ");
  debugln(htrTmpSetPT);
  debug("At Max Heater Temp ");
  debugln(atMaxHtrTmp);
  debugln(" ");
  debug("Current Fin Temp ");
  debugln(finTemp);
  debug("Fin Temp Set Point");
  debugln(fimTmpSetPt);
  debugln(" ");
  debug("Defrost Cycle ON ");
  debugln(defrostCycleOn);
  debug("Cool Cycle On ");
  debugln(coolCycleOn);
  debugln(" ");
  debugln("End of Conrol Logic ");
  debugln("****************************************");
  debugln(" ");

#endif

}
