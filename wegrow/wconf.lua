-- wconf.lua: wifi config step
-- when wifi is already configured does nothing; otherwise it loads the joinme
-- module and allows a remote client to send ssid/psk details for a wifi net
wconf = {}
wconf.run = function()
  print("starting wifi setup, heap= ", node.heap()) -- DEBUG
  if file.open("skipj.txt") then        -- we're already configured
    print("skipj exists")               -- DEBUG
    file.close()
    w.done( {}, true, false) -- no need to sleep (stayawake=true), but don't wait
  else
    wifi.sta.getap(require("j").aplstn) -- joinme entry point
  end
end
return wconf
