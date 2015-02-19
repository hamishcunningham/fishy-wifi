mqtt = net.createConnection(net.TCP, 0)
-- on connection event (all event inherit from net module)
mqtt:on("connection", function(con) print("connected") end)

tmr.alarm(0, 500, 0, function()
   mqtt:mqtt("ESP8266-2", 30, "user", "password")
end)

tmr.alarm(0, 1000, 0, function()
   mqtt:connect(1883,"littlelaptop")
end)