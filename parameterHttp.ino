
/** Parameter config page handler */
void handleParameters() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1>Set Operating Parameters</h1>"
    "\r\n<br />"
  );

  server.sendContent("<form method='POST' action='parasave'><h4>Set Parameters:</h4>");
  server.sendContent("<p>");
  server.sendContent("<br />Cooler Set point betwen 32 and 85");
  server.sendContent(String() + "<br /><input type='number' name='cool' value='" + rmTmpSetPt + "' step='1' min='32' max='85'>"); //  http://www.html5tutorial.info/html5-number.php
  server.sendContent("<br />Fin Set point betwen 30 and 35");
  server.sendContent(String() + "<br /><input type='number' name='fin' value='" + fimTmpSetPt + "' step='1' min='30' max='35'>");
  server.sendContent("<br />Heater Set point betwen 30 and 85");
  server.sendContent(String() + "<br /><input type='number' name='heat' value='" + htrTmpSetPT + "' step='1' min='30' max='85'>");
  server.sendContent("<br />Fin Dead Band betwen 1 and 5");
  server.sendContent(String() + "<br /><input type='number' name='fdb' value='" + finDeadBand + "' step='1' min='1' max='5'>");
  server.sendContent("<br />Cooler Deadband betwen 1 and 5");
  server.sendContent(String() + "<br /><input type='number' name='cdb' value='" + rmDeadBand + "' step='1' min='1' max='5'>");
  server.sendContent("<br />Heater Dead Band betwen 1 and 4");
  server.sendContent(String() + "<br /><input type='number' name='hdb' value='" + htrDeadBand + "' step='1' min='1' max='4'>");
  server.sendContent("<br />Heater Low PWM set between 1 and 75");
  server.sendContent(String() + "<br /><input type='number' name='lpwm' value='" + htrPMWlow + "' step='1' min='1' max='75'>");
  server.sendContent("<br />Heater High PWM set between 75 and 250");
  server.sendContent(String() + "<br /><input type='number' name='hpwm' value='" + htrPMWhigh + "' step='1' min='75' max='250'>");
  server.sendContent("<br />");
  server.sendContent("<br />");
  server.sendContent("<input type='submit' value='Save Settings'/>");
  server.sendContent("</p></form>");
  server.sendContent("<p>You may want to <a href='/'>return to the home page</a>.</p>");
  server.sendContent("</body></html>");

  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleParaSave() {
  debugln("Parameter save");

  rmTmpSetPt = server.arg("cool").toInt();
  fimTmpSetPt = server.arg("fin").toInt();
  htrTmpSetPT = server.arg("heat").toInt();
  finDeadBand = server.arg("fdb").toInt();
  rmDeadBand = server.arg("cdb").toInt();
  htrDeadBand = server.arg("hdb").toInt();
  htrPMWlow = server.arg("lpwm").toInt();
  htrPMWhigh = server.arg("hpwm").toInt();

  server.sendHeader("Location", "para", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveSettings(); // does not save settings to eeprom yet

}
