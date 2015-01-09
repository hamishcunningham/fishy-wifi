    --wifi.setmode(wifi.STATION)
    --wifi.sta.config("yourSSID","yourPassword")
    outpin=3 -- Select right IO index !! Here is settings for GPIO2 (Lua build 20141219)

    local scanresults = ""
    function listap(t)
      res = "\n"
      for k,v in pairs(t) do
        res = res .. k .. " : " .. v .. "\n"
      end
      scanresults = res
    end
    wifi.sta.getap(listap)

    srv=net.createServer(net.TCP) srv:listen(80,function(conn)
    conn:on("receive",function(conn,payload)
    --next row is for debugging output only
    print(payload)

    function ctrlpower()
    pwm.close(outpin)
    gpio.mode(outpin,gpio.OUTPUT)
    dotaz=string.sub(payload,kdesi[2]+1,#payload)
    if dotaz=="ON"  then gpio.write(outpin,gpio.HIGH)return end
    if dotaz=="OFF" then gpio.write(outpin,gpio.LOW)return end
    if dotaz=="FLC" then pwm.setup(outpin,2,512)pwm.start(outpin)return end
    pwm.setup(outpin,1000,dotaz*10)
    pwm.start(outpin)
    end
    --parse position POST value from header
    kdesi={string.find(payload,"pwmi=")}
    --If POST value exist, set LED power
    if kdesi[2]~=nil then ctrlpower()end

    conn:send('HTTP/1.1 200 OK\n\n')
    conn:send('<!DOCTYPE HTML>\n')
    conn:send('<html>\n')
    conn:send('<head><meta  content="text/html; charset=utf-8">\n')
    conn:send('<title>ESP8266</title></head>\n')
    conn:send('<body><h1>Sample GPIO output control</h1>\n')
    conn:send('<h3>For Lua NodeMcu 0.9.2 build 20141219 only !!!</h3>\n')
    conn:send('<h3>Tested with IE 11. and Chrome 39.</h3>\n')
    conn:send('<IMG SRC="http://esp8266.fancon.cz/common/led.gif" WIDTH="150" HEIGHT="101" BORDER="1"><br><br>\n')
    conn:send('<p>' .. scanresults .. '</p>')
    conn:send('<form action="" method="POST">\n')
    conn:send('<input type="submit" name="pwmi" value="OFF">\n')
    conn:send('<input type="submit" name="pwmi" value="10">\n')
    conn:send('<input type="submit" name="pwmi" value="20">\n')
    conn:send('<input type="submit" name="pwmi" value="30">\n')
    conn:send('<input type="submit" name="pwmi" value="40">\n')
    conn:send('<input type="submit" name="pwmi" value="50">\n')
    conn:send('<input type="submit" name="pwmi" value="60">\n')
    conn:send('<input type="submit" name="pwmi" value="70">\n')
    conn:send('<input type="submit" name="pwmi" value="80">\n')
    conn:send('<input type="submit" name="pwmi" value="90">\n')
    conn:send('<input type="submit" name="pwmi" value="ON"> % of power<br><br>\n')
    conn:send('<input type="submit" name="pwmi" value="FLC"> HW blinker</form>\n')
    conn:send('</body></html>\n')
    conn:on("sent",function(conn) conn:close() end)
    end)
end)

