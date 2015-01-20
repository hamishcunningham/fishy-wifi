-- init.lua
freak = require("freak")
continuation = {
  "joinme"   = {},
  node.restart, -- TODO or is that done within the phase itself?
                -- if so it should be a param in the phase data, as it
                -- depends on how much ram the phase has used?
  "sense"    = {},
  "report"   = {},
  "act"      = {},
  function () node.deepsleep(5000000) end,
  node.restart,
  -- OR?!
  rf = function() freak.writeconf(continuation); node.restart() end,
}
freak.run(continuation) -- never returns but may trigger restarts
