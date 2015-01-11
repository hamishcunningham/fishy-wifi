-- init.lua
-- WhiffyFishy entry point

-- libraries
jm = require("joinme")

-- do the work
jm.sayhi()
conf = jm.getconf()
if conf -- we are configured
then
  jm.joinwifi(conf)
else    -- no config, assume first run
  jm.p("no config yet")
  -- jm.writeconf(conf)
end
