-- joinme.lua
-- do initial wifi config for NodeMCU over HTTP

-- preliminaries
joinme = {}
nu = require("nmutils")
-- TODO te = require("pltemplate")
template=require("pl.template")
tmpl = [[
<ul>
# for i,val in ipairs(T) do
<li>$(i) = $(val:upper())</li>
# end
</ul>
]]
res = template.substitute(tmpl,{T = {'one','two','three'}})
print(res)

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
local function genform(T) -- takes list of APs
-- TODO
  templ = dofile("wifiform.lua")
  print(templ)
  return te.substitute(templ, { T = { "apone", "aptwo", "apthree" } })
end

-- exports
function joinme.p(fmt, ...) return print(string.format(fmt, ...)) end
function joinme.sayhi()
  joinme.p("Fishy wifi up and swimming...")
  joinme.p('MAC: %s; chip: %s; heap: %s',
    wifi.sta.getmac(), node.chipid(), node.heap())
  printip()
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

tmpl = [[
<ul>
# for i,val in ipairs(T) do
<li>$(i) = $(val:upper())</li>
# end
</ul>
]]
print(tmpl)
res = template.substitute(tmpl,{T = {'one','two','three'}})
print(res)
return res


  -- TODO header and footer
--  return genform(aplist)
end

return joinme
