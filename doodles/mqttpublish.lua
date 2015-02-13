bl = require("batterylevel")

m = mqtt.Client("ESP8266", 120, "user", "password")
-- register on connection event
m:on("connect", function(con) print("connected event") end)
-- register going offline event
m:on("offline", function(con) print ("offline event") end)
m:on("message", function(conn, topic, data) 
  print(topic .. ":" ) 
  if data ~= nil then
    print(data)
  end
end)

--tmr.alarm(0, 500, 0, function()
   --print("0.5s delay")
   --mqtt:mqtt("ESP8266", 30, "user", "password")
--end)

--tmr.alarm(0, 1000, 0, function()
     --print("1s delay")
--mqtt:connect(8443,"10.0.1.31")
--end)

print("Supply voltage is", bl.getvolts(),"V")
m:connect("10.0.1.31", 1883, 0, function(conn) print("connected to broker") end)
--m:subscribe("hello_world",0, function(conn) print("subscribe success") end)
m:publish("test_publish3","hello",0,0, function(conn) print("sent") end)
