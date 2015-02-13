bl= require("batterylevel")
conn=net.createConnection(net.TCP, false) 
conn:on("receive", function(con, pl) print(pl) end)
conn:connect(8003,"10.0.1.31")
headers = "POST /todmorden HTTP/"..bl.getvolts().."\r\n"
conn:send(headers)