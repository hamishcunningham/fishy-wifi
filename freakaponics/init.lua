-- init.lua

freak = require("freak")
r = freak.restart
phases = {
  "joinme", r, "sense", "report", "act", "sleep", r,
}
cdata = { sleep = { duration = 5 } }
freak(phases, cdata)
