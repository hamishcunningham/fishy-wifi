mqtt = net.createConnection(net.TCP, 0)
-- init mqtt client with keepalive timer 30sec
mqtt:mqtt("clientid", 30, "user", "password")

-- on publish message receive event
mqtt:on("receive", function(conn, topic, data) print(topic .. ":" .. data) end)
-- on connection event (all event inherit from net module)
mqtt:on("connection", function(con) print("connected") end)
mqtt:connect(1880,"10.0.0.5")
-- subscribe topic with qos = 0
mqtt:subscribe("/topic",0, function(conn) print("subscribe success") end)
-- publish a message
mqtt:send("/topic","hello",0,0, function(conn) print("sent") end)