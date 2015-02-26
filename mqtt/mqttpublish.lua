-- bl = require("batterylevel")

m = mqtt.Client("ESP8266", 120, "user", "password")

-- register going offline event
m:on("offline", function(con) print ("offline event") end)

function pub() 
  -- m:publish("batterylevel",bl.getvolts(),0,0, function(conn) print("sent") end)
  m:publish("batterylevel", 999,0,0, function(conn) print("sent") end)
end

m:connect("10.0.0.5", 1883, 0, function(conn)
     print("connected to broker") pub() end
)
