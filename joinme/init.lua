-- init.lua
-- entry point for JoinMe wifi config utility

-- libraries
jm = require("joinme")

function prtbl(t) for k, v in pairs(t) do print(k, v) end end

-- do the work
jm.sayhi()
conf = jm.getconf()
if conf -- we are configured
then
  jm.p("we're configured... going to join")
  jm.joinwifi(conf)
else    -- no config, assume first run
  jm.p("no config yet... going to scan")
  wifi.setmode(wifi.STATION)
  aptbl = {}
  wifi.sta.getap(function(t) aptbl = t end)
  -- tmr.alarm(0, 2000, 0, function() return end)
  prtbl(aptbl)
  aplist = {}
  for k, v in pairs(aptbl) do table.insert(aplist, k) end
  prtbl(aplist)
  wifi.setmode(wifi.SOFTAP)
  print(jm.chooserpage(aplist))
  -- jm.writeconf(conf)
end
