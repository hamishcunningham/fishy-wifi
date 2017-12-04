// AdafruitIFTTT.cpp
// Author - Andrejs Ruzencevs

bool createAdafruitFeed(String feedName) {
  String io_username = IO_USERNAME;
  feedName.toLowerCase();
  if (!feedExists(feedName)) {
    Serial.println("Creating feed: " + feedName);
    String postData = "{\"name\": \"" + feedName + "\"}";
    ioClient.beginRequest();
    ioClient.post("/api/v2/" + io_username + "/feeds/");
    ioClient.sendHeader("Content-Type", "application/json");
    ioClient.sendHeader("Content-Length", postData.length());
    ioClient.sendHeader("X-AIO-Key", IO_KEY);
    ioClient.beginBody();
    ioClient.print(postData);
    ioClient.endRequest();
    int statusCode = ioClient.responseStatusCode();
    String response = ioClient.responseBody();
    if (statusCode == 200) {
      Serial.println("Feed created. ");
      return true;
    } else {
      return false;
    }
    Serial.println(response);
  } else {
    Serial.println("Feed already exists (" + feedName + ")");
    return true;
  }
}

bool feedExists(String feedName) {
  String io_username = IO_USERNAME;
  String path = "/api/v2/" + io_username + "/feeds/" + feedName;
  Serial.print("making GET request to Adafruit.io: ");
  Serial.println(path);

  // send the POST request
  ioClient.beginRequest();
  ioClient.get(path);
  ioClient.sendHeader("X-AIO-Key", IO_KEY);
  ioClient.endRequest();


  // read the status code and body of the response
  int statusCode = ioClient.responseStatusCode();
  String response = ioClient.responseBody();

  return statusCode == 200;
}


bool triggerIFTTWebHook(String URL,String params) {
//  String url = genrateIFTTTWebHookURL(IFTTT_EVENT_NAME, IFTTT_KEY);
  Serial.println("Triggering IFTT WebHook, event: " + URL);
  Serial.println("params: " + params);
  IFTTTClient.post(URL + '?' + params);
  // read the status code and body of the response
  int statusCode = IFTTTClient.responseStatusCode();
  String response = IFTTTClient.responseBody();
  
  Serial.print("Status code: ");
  Serial.println(statusCode);

  return statusCode == 200;
}

bool createAdafruitIOTrigger(String feedID, String op, int value) {
  String contentType = "application/json";
  String io_username = IO_USERNAME;
  String path = "/api/v2/" + io_username + "/triggers";
  Serial.print("making POST request to Adafruit.io: ");
  Serial.println(path);

  // assemble the body of the POST message:
  String postData = "{\"feed_id\": " + feedID;
  postData += ",\"operator\": \"" + op + "\"";
  postData += ",\"value\": " + String(value);
  postData += ",\"action\": \"webhook\"";
  postData += ",\"action_value\": \"https://maker.ifttt.com/trigger/us_readings/with/key/bIg_ReTcrDg468BFTUNt0v?value1=" + generateUltraSonicFeedName() + "&value2=" + String(value) + "\"";
  postData += ",\"trigger_type\": \"reactive\"";
  postData += "}";

  // send the POST request
  ioClient.beginRequest();
  ioClient.post(path);
  ioClient.sendHeader("Content-Type", contentType);
  ioClient.sendHeader("Content-Length", postData.length());
  ioClient.sendHeader("X-AIO-Key", IO_KEY);
  ioClient.beginBody();
  ioClient.print(postData);
  ioClient.endRequest();


  // read the status code and body of the response
  int statusCode = ioClient.responseStatusCode();
  String response = ioClient.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  return statusCode == 201;
}

String genrateIFTTTWebHookURL(String eventName, String IFTTTKey) {
  return "/trigger/" + eventName + "/with/key/" + IFTTTKey;
}

String generateSensorFeedName(String prefix) {
  getMAC(MAC_ADDRESS);
  char USFeedPrefix[20];
  prefix.toCharArray(USFeedPrefix, 50);
  return strcat(USFeedPrefix, MAC_ADDRESS);
}

String getAdafruitIOFeedId(String feedName) {
  String io_username = IO_USERNAME;
  feedName.toLowerCase();
  String path = "/api/v2/" + io_username + "/feeds/" + feedName;
  Serial.print("making GET request to Adafruit.io: ");
  Serial.println(path);

  // send the POST request
  ioClient.beginRequest();
  ioClient.get(path);
  ioClient.sendHeader("X-AIO-Key", IO_KEY);
  ioClient.endRequest();


  // read the status code and body of the response
  int statusCode = ioClient.responseStatusCode();
  String response = ioClient.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  if (statusCode != 200) {
    return "undefined";
  }
  String res1 = response.substring(0, response.indexOf(feedName));
  String res2 = res1.substring(res1.lastIndexOf("id") + 4, res1.lastIndexOf(','));
  Serial.println("Feed id: " + res2);
  return res2;
}


