-- init.lua
print("xmqtt, ip = ", wifi.sta.getip())
-- while wifi.sta.getip() == nil do tmr.delay(1000) end
tmr.delay(1000)
print("ip = ", wifi.sta.getip())
m = mqtt.Client("ESP8266", 120, "user", "password")
m:on("offline", function(con) print ("xoffline event") end)
function pub() 
  m:publish(
    "xbatterylevel", node.heap(), 0, 0, function(conn) print("xsent") end
  )
end
m:connect(
  "10.0.0.9", 1883, 0, function(conn) print("xconnected to broker") pub() end
)
print("xmqtt done")


-- bat.lua
  m = mqtt.Client("ESP8266", 120, "user", "password")
  -- m:on("offline", function(con) print ("offline event") end)
  function pub() 
    m:publish("batterylevel", v, 0, 0, function(conn) print("sent") end)
  end
  m:connect("10.0.0.9", 1883, 0, function(conn)
    print("connected to broker") pub() end
  )
  ]]

  --
  m = mqtt.Client("ESP8266", 120, "user", "password")
  m:on("offline", function(con) print ("offline event") end) -- DEBUG
  m:connect("10.0.0.9", 1883, 0, function(conn)
    print("connected to broker")
    m:publish("batterylevel", v,0,0, function(conn) print("sent") end)
  end)


-- soil.lua
  pin=1
  pause=16000
  readings={}
  sum=0
  for i=1,1000 do
    gpio.mode(pin, gpio.OUTPUT)
    gpio.write(pin, gpio.LOW)
    tmr.wdclr()
    tmr.delay(pause)

    c=0
    gpio.mode(pin, gpio.INPUT)
    while gpio.read(pin) == gpio.LOW do
      tmr.wdclr()
      c = c + 1
    end
    sum = sum + c
  end
  print("cap reading: ", sum/1000)


-- mqttpublish
m = mqtt.Client("ESP8266", 120, "user", "password")

-- register going offline event
m:on("offline", function(con) print ("offline event") end)

function pub() 
  m:publish("batterylevel", node.heap(),0,0, function(conn) print("sent") end)
end

m:connect("10.0.0.9", 1883, 0, function(conn)
     print("connected to broker") pub() end
)
