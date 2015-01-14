-- j.lua: JoinMe wifi config utility
j={}
local function getconf() 
  status, results = pcall(dofile, "joinmeconf.lua")
  if status then return results else return nil end
end
local function joinwifi(conf)
  wifi.setmode(wifi.STATION)
  wifi.sta.config(conf.ssid, conf.key)
  wifi.sta.connect()
  -- tmr.alarm(0, 5000, 0, function() printip() end)
end
local wifiform = [=[
<p>Choose a wifi access point to join:<form action="chooseap">
<ol>
_ITEMS_
</ol> <br/> <input type="submit" value="Submit"> </form></p>
]=] --:
local function genform(aptbl) -- takes table of APs
  buf = ""
  for ssid, _ in pairs(aptbl)
  do
    buf = buf .. '  <li>SSID: <input type="radio" name="' .. ssid ..
      '" value="' .. ssid .. '">' .. ssid .. '<br/>\n'
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end
local function sendchooser(aptbl)
  wifi.setmode(wifi.SOFTAP)
  print(genform(aptbl))
end
function j.doinit()
  conf = getconf()
  if conf -- we are configured
  then
    joinwifi(conf)
  else    -- no config, assume first run
    wifi.setmode(wifi.STATION)
    wifi.sta.getap(sendchooser)
    -- writeconf(conf)
  end
end
return j
