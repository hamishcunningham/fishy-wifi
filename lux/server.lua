srv=net.createServer(net.TCP) srv:listen(80,function(conn)
conn:on("receive",function(conn,payload)

    -- generates HTML web site
    conn:send('HTTP/1.1 200 OK\r\n Content-type: text/html\r\n\r\n\
    <!DOCTYPE HTML>\
    <html>\
    <head><meta  content="text/html; charset=utf-8">\
    <title>ESP8266</title></head>\
    <body><TEXT="#FF69B4"><font size="7">Hello Kitty</font>\
    <IMG SRC="http://saltmarsh.webarch.net/rect5235.png" WIDTH="400" HEIGHT="65">\
    </body></html>')
    conn:on("sent",function(conn) conn:close() end)
    end)
end)
