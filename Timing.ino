void timing() {
  // put your main code here, to run repeatedly:

  int currentMillis = millis();

  if ((currentMillis - previousRG1 > rg1)) //Check RG1
  {
    exeRG1(currentMillis);
    previousRG1 = currentMillis;
  } // end if rg1

  if ((currentMillis - previousRGFiveMin > rgFiveMin)) //Check 10 Minute Rate Group
  {
    exeRGFiveMin(currentMillis);
    previousRGFiveMin = currentMillis;
  } // end if exeRGTenMin

} // End Timing


// Rate Group Subroutines

void exeRG1(int mil) // 1Hz
{
  debugln("1HZ Rate Group");
  debug("current mil ");
  debugln(mil);

  requestTemps(); //TODO: call at 1Hz
  delay(10); // delay function on esp8266 library allows OS to run background tasks
  setTemps(); //TODO: call at 1Hz // commented out with no hardware connected
  delay(10); // delay function on esp8266 library allows OS to run background tasks
  setHeaterState(); // Set use temps, set points, current state, previous state to new heater state

}// end exeRG1

void exeRGFiveMin(int mil) // 10 Minute Rate Group
{
  debugln("5 Minute Rate Group");
  debug("current mil ");
  debugln(mil);
  
  publishMqtt(); //Publish MQTT Data
  delay(10); // delay function on esp8266 library allows OS to run background tasks

}// end exe Ten Minute Rate Group


