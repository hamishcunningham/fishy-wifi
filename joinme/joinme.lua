-- joinme.lua
-- do initial wifi config for NodeMCU over HTTP

-- preliminaries
joinme = {}
nu = require("nmutils")

-- debug code
print("are we running on NodeMCU? ", nu.isnodemcu())

-- config data
local conffile = "joinmeconf.lua"

-- local utilities
local function printip() print("Wifi station ip: ", wifi.sta.getip()) end
local function conf2string(conf)
  buf = "{\n"
  for k, v in pairs(conf) do
    buf = buf .. string.format('  %s = "%s",\n', k, v)
  end
  buf = buf .. "}\n"
  return buf
end
local wifiform = [=[
<p>Choose a wifi access point to join:
<form action="chooseap">
  <ol>
_ITEMS_
  </ol>
  <br/>
  <input type="submit" value="Submit">
</form></p>
]=]

function joinme.genform(aplist) -- takes list of APs
  buf = ""
  for ssid, _ in pairs(aplist)
  do
    buf = buf .. '  <li>SSID: <input type="radio" name="' .. ssid ..
      '" value="' .. ssid .. '">' .. ssid .. '<br/>\n'
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end

-- exports
function joinme.p(fmt, ...) return print(string.format(fmt, ...)) end
function joinme.sayhi()
  joinme.p("Fishy wifi up and swimming...")
  joinme.p('MAC: %s; chip: %s; heap: %s',
    wifi.sta.getmac(), node.chipid(), node.heap())
  printip()
end
function joinme.t2s(t) -- one-level table to string
  for k,v in pairs(t) do print(k.." : "..v) end
end
function joinme.getconf() 
  status, results = pcall(dofile, conffile)
  if status then return results else return nil end
end
function joinme.writeconf(conf)
  f = file.open(conffile, "w")
  if not f then return nil end
  file.write("return " .. conf2string(conf))
  file.close()
  return true
end
function joinme.joinwifi(conf)
  joinme.p("config incoming:")
  joinme.p(conf2string(conf))
  wifi.setmode(wifi.STATION)
  wifi.sta.config(conf.ssid, conf.key)
  wifi.sta.connect()
  tmr.alarm(0, 5000, 0, function() printip() end)
end
function joinme.chooserpage(aplist)
  -- TODO header and footer
  return genform(aplist)
end

return joinme
