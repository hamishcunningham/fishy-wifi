/////////////////////////////////////////////////////////////////////////////
// looooooooooooooooooooop //////////////////////////////////////////////////
void loop() {
  webServer.handleClient();

  if(loopCounter == TICK_MONITOR) { // monitor levels, step valves, push data
    monitor_t* now = &monitorData[monitorCursor];
    if(monitorSize < MONITOR_POINTS)
      monitorSize++;
    now->timestamp = millis();
    if(GOT_TEMP_SENSOR) {
      getTemperature(&now->waterCelsius);               yield();
    }
    if(GOT_HUMID_SENSOR) {
      getHumidity(&now->airCelsius, &now->airHumid);    yield();
    }
    if(GOT_LIGHT_SENSOR) { getLight(&now->lux);         yield(); }
    if(GOT_PH_SENSOR) { getPH(&now->pH);                yield(); }
    if(GOT_LEVEL_SENSOR) {
      getLevel(LEVEL_ECHO_PIN1, &now->waterLevel1);     yield();
      getLevel(LEVEL_ECHO_PIN2, &now->waterLevel2);     yield();
      getLevel(LEVEL_ECHO_PIN3, &now->waterLevel3);     yield();
      dln(valveDBG, "");
      dbg(valveDBG, "wL1: "); dbg(valveDBG, now->waterLevel1);
      dbg(valveDBG, "; wL2: "); dbg(valveDBG, now->waterLevel2);
      dbg(valveDBG, "; wL3: "); dln(valveDBG, now->waterLevel3);
    }

    flowController.step(now); yield();  // set valves on and off etc.
    if(SEND_DATA) {                     // push data to the cloud
      postSensorData(&monitorData[monitorCursor]); yield();
    }
      
    if(++monitorCursor == MONITOR_POINTS)
      monitorCursor = 0;
  }

  if(loopCounter == TICK_WIFI_DEBUG) {
    dbg(netDBG, "SSID: "); dbg(netDBG, apSSID);
    dbg(netDBG, "; IP address(es): local="); dbg(netDBG, WiFi.localIP());
    dbg(netDBG, "; AP="); dln(netDBG, WiFi.softAPIP());
  }
  if(loopCounter == TICK_HEAP_DEBUG) {
    dbg(miscDBG, "free heap="); dln(miscDBG, ESP.getFreeHeap());
  }

  if(loopCounter++ == LOOP_ROLLOVER) loopCounter = 0;
}
