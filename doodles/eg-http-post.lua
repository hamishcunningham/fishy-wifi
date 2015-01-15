


THIS IS BROKEN...



-- Demo server v.20150101 for POST method - tested with NodeMcu 0.9.4 build 20141230 and
-- Nokia XPRESS 5, FireFox 34, IE 11, Chrome 39, ANDROID 4.2.2 default browser
-- 1. Flash NodeMcu 0.9.2 build 20141219 or later to ESP module.
-- 2. Load program POST_54.lua to ESP with LuaLoader
-- 3. HW reset module
-- 4. Login module to your AP - wifi.setmode(wifi.STATION),wifi.sta.config("yourSSID","yourPASSWORD")
-- 5. Run program POST_54.lua - dofile(POST_54.lua)
-- 6. Test IP address - wifi.sta.getip()
-- 7. Test it with your browser and true  IP address of module.
-- 8. Keep happy new year 2015!
--------------------------------------------------------------------------------
outpin=3 -- Select right IO index !! Here is settings for GPIO2 (Lua build 20141219 or later)
-------------------------------------------------------------------------------
setpower=0
gpio.mode(outpin,gpio.OUTPUT)
gpio.write(outpin,gpio.LOW)
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,payload)
    print(payload) -- for debugging only
    function ctrlpower() -- Settings ON/OFF or PWM for GPIO
      pwm.close(outpin)
      gpio.mode(outpin,gpio.OUTPUT)
      value=string.sub(payload,strstr[2],#payload)
      if value=="ON"  then gpio.write(outpin,gpio.HIGH)setpower=100 return end
      if value=="OFF" then gpio.write(outpin,gpio.LOW)setpower=0 return end
      if value=="FLC" then pwm.setup(outpin,2,100)pwm.start(outpin)setpower=10 return end
      if value~=nil and tonumber(value)>0 and tonumber(value)<100 then setpower=tonumber(value) else return end
      pwm.setup(outpin,1000,setpower*10)
      pwm.start(outpin)
    end
    --parse a position of POST variable/value from header
    strstr={string.find(payload,"\r\n\r\npwmi=")}
    if(strstr[1]~=nil) then strstr[1]=strstr[1]+4 strstr[2]=strstr[1]+5 end
    if payload~=nil and string.sub(payload,1,5)=="pwmi=" then strstr[1]=1 strstr[2]=6 end
    --If POST value exist, set LED power
    if (strstr[2]~=nil)then ctrlpower()end

    -- generates HTML web site
    htmlpage = [=[HTTP/1.1 200 OK\r\n Content-type: text/html\r\n\r\n<!DOCTYPE HTML>    <html>    <head><meta  content="text/html; charset=utf-8"> <title>ESP8266</title></head>    <body><h3>Sample GPIO output control</h3><form action="" method="post">    <input type="submit" name="pwmi" value="OFF">    <input type="submit" name="pwmi" value="10"> <input type="submit" name="pwmi" value="20">    <input type="submit" name="pwmi" value="30">    <input type="submit" name="pwmi" value="40">]=]
    htmlpage = htmlpage .. [=[<input type="submit" name="pwmi" value="50"> <input type="submit" name="pwmi" value="60">    <input type="submit" name="pwmi" value="70"> <input type="submit" name="pwmi" value="80"> <input type="submit" name="pwmi" value="90">    <input type="submit" name="pwmi" value="ON"><font color="red"><big><big>]=]
    htmlpage = htmlpage .. ' ' .. setpower .. ' '
    htmlpage = htmlpage .. [=[% of power</big></big></font><font color="black"><br></font><br><input type="submit" name="pwmi" value="FLC"> HW blinker</form><br><br><IMG SRC="http://esp8266.fancon.cz/common/led.gif" WIDTH="150" HEIGHT="101" BORDER="1"></body></html>]=]
    conn:send(htmlpage)
    conn:on("sent",function(conn) conn:close() end)
  end)
end)

