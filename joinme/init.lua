-- init.lua
-- entry point for JoinMe wifi config utility

-- libraries
jm = require("joinme")

-- do the work
jm.sayhi()
conf = jm.getconf()
if conf -- we are configured
then
  jm.p("we're configured... going to join")
  jm.joinwifi(conf)
else    -- no config, assume first run
  jm.p("no config yet... going to scan")
  aplist = wifi.sta.getap(function(t) return t end)
  print(jm.chooserpage(aplist))
  -- jm.writeconf(conf)
end
