-- joinme.lua
-- do initial wifi config for NodeMCU over HTTP

-- preliminaries
joinme = {}
nu = require("nmutils")
print("are we running on NodeMCU? ", nu.isnodemcu())

-- config data
local conffile = "joinmeconf.lua"
local conf = nil

-- local utilities
local function printip() print("Wifi station ip: ", wifi.sta.getip()) end
local function sayhi()
  p("Fishy wifi up and swimming...")
  p('MAC: %s; chip: %s; heap: %s',
    wifi.sta.getmac(), node.chipid(), node.heap())
  printip()
end
local function conf2string(conf)
  buf = "{\n"
  for k, v in pairs(conf) do
    buf = buf .. string.format("  %s = %s\n", k, v)
  end
  buf = buf .. "}\n"
  return buf
end

-- exports
local function joinme.p(fmt, ...) return print(string.format(fmt, ...)) end
local function joinme.getconf() return dofile(conffile) end
local function joinme.writeconf(conf)
  f = file.open(conffile, "w")
  if not f then return nil end
  file.write("return " .. conf2str())
  file.close()
  return true
end
local function joinme.joinwifi(conf)
  p("config incoming:")
  p(conf2string())
  wifi.setmode(wifi.STATION)
  wifi.sta.config(conf.ssid, conf.key)
  wifi.sta.connect()
  tmr.alarm(0, 5000, 0, function() printip() end)
end

return joinme
