/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head>"
    "<META HTTP-EQUIV="
    "\""
    "refresh"
    "\""
    "CONTENT="
    "\""
    "30"
    "\""
    ">"
    "</head><body>"
    "<h1>Cooler Controller V0.?.?</h1>"
  );
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>You are connected through the soft AP: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>You are connected through the wifi network: ") + ssid + "</p>");
  }

  server.sendContent(
    "\r\n<br />"
    "<table><tr><th align='left'>Variable</th><th align='left'>Value</th></tr>"
  );
  
  server.sendContent(String() + "<tr><td>Room Temp </td><td>" + String(rmTemp) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Cooler Dead Band </td><td>" + String(rmDeadBand) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Room Set Point </td><td>" + String(rmTmpSetPt) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Cool Cycle On </td><td>" + String(coolCycleOn) + "</td></tr>");
  server.sendContent(String() + "<tr><td> </td><td> </td></tr>");
  server.sendContent(String() + "<tr><td>Fin Temp </td><td>" + String(finTemp) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Fin Set Point </td><td>" + String(fimTmpSetPt) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Fin Dead Band </td><td>" + String(finDeadBand) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Defrost Cycle On </td><td>" + String(defrostCycleOn) + "</td></tr>");
  server.sendContent(String() + "<tr><td> </td><td> </td></tr>");
  server.sendContent(String() + "<tr><td>Heater Temp</td><td>" + String(htrTemp) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Heater Temp Set Point</td><td>" + String(htrTmpSetPT) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Heater PWM </td><td>" + String(htrPWM) + "</td></tr>");
  server.sendContent(String() + "<tr><td>At Max Heater Temp </td><td>" + String(atMaxHtrTmp) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Heater Dead Band </td><td>" + String(htrDeadBand) + "</td></tr>");
  server.sendContent(String() + "<tr><td> </td><td> </td></tr>");
  server.sendContent(String() + "<tr><td>High PWM Setting </td><td>" + String(htrPMWhigh) + "</td></tr>");
  server.sendContent(String() + "<tr><td>Low PWM Setting </td><td>" + String(htrPMWlow) + "</td></tr>");
  
  server.sendContent(
    "</table>"
    "\r\n<br />");

  server.sendContent(
    "<p>You may want to <a href='/wifi'>config the wifi connection</a>.</p>"
    "<p>You may also want to <a href='/para'>config the cooler settings</a>.</p>"
    "</body></html>"
  );
  
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    debug("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(
    "<html><head></head><body>"
    "<h1>Wifi config</h1>"
  );
  if (server.client().localIP() == apIP) {
    server.sendContent(String("<p>You are connected through the soft AP: ") + softAP_ssid + "</p>");
  } else {
    server.sendContent(String("<p>You are connected through the wifi network: ") + ssid + "</p>");
  }
  server.sendContent(
    "\r\n<br />"
    "<table><tr><th align='left'>SoftAP config</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(softAP_ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.softAPIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN config</th></tr>"
  );
  server.sendContent(String() + "<tr><td>SSID " + String(ssid) + "</td></tr>");
  server.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr>");
  server.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>"
  );
  debugln("scan start");
  int n = WiFi.scanNetworks();
  debugln("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      server.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : " *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
    }
  } else {
    server.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
  }
  server.sendContent(
    "</table>"
    "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
    "<input type='text' placeholder='network' name='n'/>"
    "<br /><input type='password' placeholder='password' name='p'/>"
    "<br /><input type='submit' value='Connect/Disconnect'/></form>"
    "<p>You may want to <a href='/'>return to the home page</a>.</p>"
    "</body></html>"
  );
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  debugln("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}


