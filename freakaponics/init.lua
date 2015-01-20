-- init.lua: top level entry point for the freakaponics sensor/actuator board
freak = require("freak")
function needwifi(continuation)
  return not continuation.taskdata["joinme"] or
         not continuation.taskdata["joinme"].ssid
end
tasks = { "joinme", "sense", "report", "act", 5000 }
precons = { joinme=needwifi } -- task preconditions
-- freak.begin(tasks, precons) -- never returns but may trigger restarts
