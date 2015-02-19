-- bat.lua
-- read supply voltage when used with a 100k/470k resistor divider
-- for vcc - maximum is about 4.9v - tuned for 3AA cells
bat = {}
local function volts()
  local samples = 100
  local result = 0
  for i=1, samples do result = result + adc.read(0) end
  voltage = string.format("%.2f", result/samples * 0.0054)
  print("battery level: ", voltage)     -- DEBUG
  return voltage
end

local function mpub(v)
  m = mqtt.Client("ESP8266", 120, "user", "password")

  -- register going offline event
  m:on("offline", function(con) print ("offline event") end) -- DEBUG

  m:connect("10.0.0.5", 1883, 0, function(conn)
    print("connected to broker")
    m:publish("batterylevel", v,0,0, function(conn) print("sent") end)
  end)
  return v
end

bat.run = function() 
  return mpub(volts())
end
return bat
