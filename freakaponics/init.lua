-- init.lua
freak = require("freak")
function needwifi(continuation) return not continuation.joinme.ssid end
tasks = {
  "joinme",
  "sense",
  "report",
  "act",
  function () node.deepsleep(5000000) end,
}
taskdata = {
  "joinme"   = { prec = needwifi, },
  "sense"    = {},
  "report"   = {},
  "act"      = {},
  function () node.deepsleep(5000000) end,
}
freak.run({ tasks=tasks, taskdata=taskdata }) -- never returns
