-- j.lua: JoinMe wifi config utility
-- usage: j=require("j"); j.doinit(); j=nil
j={}
local cfile = "jconf.lua"
local function getconf() 
  status, results = pcall(dofile, cfile) -- might just do return pcall(...) or {}
  if status then return results else return nil end
end
local function conf2string(conf)
  buf = "{"
  for k, v in pairs(conf) do
    buf = buf .. string.format(' %s = "%s",', k, v)
  end
  return buf .. " }\n"
end
local function writeconf(conf)
  f = file.open(cfile, "w")
  if not f then return nil end
  file.write("return " .. conf2string(conf))
  file.close()
  return true
end
local function joinwifi(conf)
  wifi.sta.config(conf.ssid, conf.key)
  wifi.sta.connect()
end
local wifiform = [=[ <html><body>
<h2>Choose a wifi access point to join</h2><p><form method="POST" action="c">
_ITEMS_<br/>Pass key: <input type="textarea" name="key"><br/><br/>
<input type="submit" value="Submit"></form></p></body></html> ]=] --:
local function genform(aptbl) -- takes table of APs
  buf = ""; checked = " checked"
  for ssid, _ in pairs(aptbl) do
    buf = buf .. '<input type="radio" name="ssid" value="' .. ssid .. '"' ..
      checked .. '>' .. ssid .. '<br/>\n'
    checked = ""
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end
local function httplistener(conn, payload)
  -- print("\n", payload, "\n")
  if string.find(payload, "POST /c HTTP") then
    ssid, key = string.gmatch(payload, "ssid=(.*)&key=(.*)")()
    if ssid and key then
      writeconf({ ssid=ssid, key=key })
      conn:send("<html><body><h2>Done! Restarting...</h2></body></html>")
      conn:on("sent", function(_) node.restart() end)
    end
  else
    conn:send(frm)
  end
end
local function sendchooser(aptbl)
  frm = genform(aptbl) -- TODO move to httplistener so gets updated scan
                       -- or have a loop in doinit?
  wifi.setmode(wifi.SOFTAP)
  srv=net.createServer(net.TCP)
  srv:listen(80, function(conn) conn:on("receive", httplistener) end)
end
function j.doinit()
  wifi.setmode(wifi.STATION) -- we will either scan then swap mode, or join...
  conf = getconf()
  if conf then  -- we are configured
    joinwifi(conf)
  else          -- no config, assume first run
    wifi.sta.getap(sendchooser) -- get AP list and pass to continuation
  end
end
return j
