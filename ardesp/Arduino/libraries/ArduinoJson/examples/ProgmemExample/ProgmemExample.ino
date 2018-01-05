// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// This example shows the different ways you can use Flash strings with
// ArduinoJson.
//
// Use Flash strings sparingly, because ArduinoJson duplicates them in the
// JsonBuffer. Prefer plain old char*, as they are more efficient in term of
// code size, speed, and memory usage.

#include <ArduinoJson.h>

void setup() {
#ifdef PROGMEM  // <- check that Flash strings are supported

  DynamicJsonBuffer jsonBuffer;

  // You can use a Flash String as your JSON input.
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  JsonObject& root =
      jsonBuffer.parseObject(F("{\"sensor\":\"gps\",\"time\":1351824120,"
                               "\"data\":[48.756080,2.302038]}"));

  // You can use a Flash String to get an element of a JsonObject
  // No duplication is done.
  long time = root[F("time")];

  // You can use a Flash String to set an element of a JsonObject
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  root[F("time")] = time;

  // You can set a Flash String to a JsonObject or JsonArray:
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  root["sensor"] = F("gps");

  // You can compare the content of a JsonVariant to a Flash String
  if (root["sensor"] == F("gps")) {
    // ...
  }

#else

#warning PROGMEM is not supported on this platform

#endif
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// The website arduinojson.org contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any memory
// problem.
// Please check it out at: https://arduinojson.org/
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. It also tells why you
// should not abuse Flash strings with ArduinoJson.
// Please check it out at: https://leanpub.com/arduinojson/