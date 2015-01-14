-- j.lua: JoinMe wifi config utility
-- usage: j=require("j"); j.doinit(); j=nil
j={}
local cfile = "jconf.lua"
local function getconf() 
  status, results = pcall(dofile, cfile)
  if status then return results else return nil end
end
local function conf2string(conf)
  buf = "{\n"
  for k, v in pairs(conf) do
    buf = buf .. string.format('  %s = "%s",\n', k, v)
  end
  return buf .. "}\n"
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
  -- tmr.alarm(0, 5000, 0, function() printip() end)
end
local wifiform = [=[
<p>Choose a wifi access point to join:<form action="chooseap"><ol>
_ITEMS_
</ol><br/><input type="submit" value="Submit"></form></p> ]=] --:
local function genform(aptbl) -- takes table of APs
  buf = ""
  for ssid, _ in pairs(aptbl)
  do
    buf = buf .. '  <li>SSID: <input type="radio" name="' .. ssid ..
      '" value="' .. ssid .. '">' .. ssid .. '\n'
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end
local function sendchooser(aptbl)
  print(genform(aptbl))
  -- TODO call continuation func
  -- wifi.setmode(wifi.SOFTAP)
end
function j.doinit() -- TODO may want to take a continuation param
  wifi.setmode(wifi.STATION) -- we will either scan then swap mode, or join...
  conf = getconf()
  if conf -- we are configured
  then
    joinwifi(conf)
  else    -- no config, assume first run
    wifi.sta.getap(sendchooser)
    -- TODO writeconf(conf)
  end
end
return j
