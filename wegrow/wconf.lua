-- wconf.lua: wifi config step
-- when wifi is already configured does nothing; otherwise it loads the joinme
-- module and allows a remote client to send ssid/psk details for a wifi net
wconf = {}
wconf.run = function(done)
  print("starting wifi setup, heap= ", node.heap()) -- DEBUG
  if file.open("skipj.txt") then        -- we're already configured
    print("skipj exists")               -- DEBUG
    file.close()
    return w.done ({}, true, false)               -- don't sleep; run next step
  end
  j=require("j");
  print(node.heap());
  wifi.sta.getap(j.aplstn) -- no need for "done" - handles own restart
end
return wconf
 
