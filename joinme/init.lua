-- init.lua
-- entry point for JoinMe wifi config utility

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
  print(jm.chooserpage())
  -- jm.writeconf(conf)
end
