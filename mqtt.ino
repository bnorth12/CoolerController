
void publishMqtt() {

  Adafruit_MQTT_Subscribe *subscription;

  connectMQTT();


  // Publish data

  debugln("");
  debugln("****************************");
  debugln("****************************");
  debugln("");
  if (! heatertemp.publish(htrTemp))
    {debugln("Failed to publish heater temp");}
  else
    {debugln("heater temp published!");}

  if (! heatersettemp.publish(htrTmpSetPT))
    {debugln("Failed to publish heater temp set point");}
  else
    {debugln("heater temp set point published!");}

  if (! heaterpwm.publish(htrPWM))
    {debugln("Failed to publish heater PWM");}
  else
    {debugln("heater PWM published!");}

  if (! heatermaxtemp.publish(atMaxHtrTmp))
    {debugln("Failed to at max heater temp");}
  else
    {debugln("at max heater temp published!");}

  if (! fintemp.publish(finTemp))
    {debugln("Failed to publish fin temp");}
  else
    {debugln("fin temp published!");}

  if (! finsettemp.publish(fimTmpSetPt))
    {debugln("Failed to publish fin temp set point");}
  else
    {debugln("fin temp set point published!");}

  if (! defrostcycle.publish(defrostCycleOn))
    {debugln("Failed to publish defrost cycle on");}
  else
    {debugln("Temperature defrost cycle on!");}

  if (! coolertemp.publish(rmTemp))
    {debugln("Failed to publish cooler temp");}
  else
    {debugln("cooler temp published!");}

  if (! coolersetpoint.publish(rmTmpSetPt))
    {debugln("Failed to publish cooler temp set point");}
  else
    {debugln("cooler temp set point published!");}

  if (! coolcycle.publish(coolCycleOn))
    {debugln("Failed to publish cool cycle on");}
  else
    {debugln("cool cycle on published!");}
  
  debugln("");
  debugln("****************************");
  debugln("****************************");
  debugln("");

  mqtt.disconnect();
}



// connect to adafruit io via MQTT
void connectMQTT() {

  debugln("Connecting to Adafruit IO... ");

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: debugln("Wrong protocol"); break;
      case 2: debugln("ID rejected"); break;
      case 3: debugln("Server unavail"); break;
      case 4: debugln("Bad user/pass"); break;
      case 5: debugln("Not authed"); break;
      case 6: debugln("Failed to subscribe"); break;
      default: debugln("Connection failed"); break;
    }

    if (ret >= 0)
      mqtt.disconnect();
  }
}
