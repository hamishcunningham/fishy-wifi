-- init.lua: top level entry point for the freakaponics sensor/actuator board

wifi.setmode(wifi.STATIONAP) -- coding around nil result to first call issue
function lstf(t) if t then for k,_ in pairs(t) do print(k) end end end
wifi.sta.getap(lstf) -- consume nil result

freak = require("freak")
tasks = { "joinme", "sense", "report", "act", 5000 }
-- freak.begin(tasks)
