-- init.lua

-- entry point for JoinMe wifi config utility
function doinit()
  if not jm then jm = require("joinme") end
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
    jm.prtbl(aptbl)
--[[
    tmr.alarm(0, 2000, 0, function() return end)
    aplist = {}
    for k, v in pairs(aptbl) do table.insert(aplist, k) end
    jm.prtbl(aplist)
    wifi.setmode(wifi.SOFTAP)
    print(jm.chooserpage(aplist))
    -- jm.writeconf(conf)
]] --
  end
end
