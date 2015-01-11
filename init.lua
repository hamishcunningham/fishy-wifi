-- init.lua

jm = require("joinme")

-- config data
local CFILE = "fishyconf.lua"
conf = nil -- not local as we want dofile to overwrite it

-- utilities
function p(fmt, ...) return print(string.format(fmt, ...)) end
function printip() print("Wifi station ip: ", wifi.sta.getip()) end
function sayhi()
  p("Fishy wifi up and swimming...")
  p('MAC: %s; chip: %s; heap: %s',
    wifi.sta.getmac(), node.chipid(), node.heap())
  printip()
end
function getconf() dofile(CFILE); return conf end
function writeconf()
  f = file.open(CFILE, "w")
  if not f then return nil end
  file.write("conf = ")
  file.write(conf2str())
  file.close()
end
function conf2string()
  buf = "{\n"
  for k, v in pairs(conf) do
    buf = buf .. string.format("  %s = %s\n", k, v)
  end
  buf = buf .. "}\n"
  return buf
end

-- do the work
sayhi()
getconf()
if conf -- we are configured
then
  p("config incoming:")
  p(conf2string())
  wifi.setmode(wifi.STATION)
  wifi.sta.config(conf.ssid, conf.key)
  wifi.sta.connect()
  tmr.alarm(0, 5000, 0, function() printip() end)
else    -- no config, assume first run
  p("no config yet")
  -- writeconf(conf)
end
