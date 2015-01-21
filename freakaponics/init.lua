-- init.lua: top level entry point for the freakaponics sensor/actuator board

-- TODO releasing modules: forgot package.loaded!
-- p = nil
-- package.loaded["p"] = nil

wifi.setmode(wifi.STATIONAP) -- coding around nil result to first call issue
function lstf(t) if t then for k,v in pairs(t) do print(k..": "..v) end end end
wifi.sta.getap(lstf) -- consume nil result

freak = require("freak")
function needwifi(continuation)
  return not continuation.taskdata["joinme"] or
         not continuation.taskdata["joinme"].ssid
end
tasks = { "joinme", "sense", "report", "act", 5000 }
precons = { joinme=needwifi } -- task preconditions
-- freak.begin(tasks, precons) -- never returns but may trigger restarts
