-- init.lua: top level entry point for the freakaponics sensor/actuator board

wifi.setmode(wifi.STATIONAP)    -- coding around nil result to first call issue
function lstf(t) if t then for k,_ in pairs(t) do print(k) end end end
wifi.sta.getap(lstf)            -- consume nil result
print("starting wifi setup, heap= ", node.heap()) -- DEBUG

skipj = "skipj.txt"
if file.open(skipj) then        -- we're already configured
  print("skipj exists")         -- DEBUG
  file.close()
else
  j=require("j")
  wifi.sta.getap(j.aplistener)  -- joinme entry point
end -- wifi config

print("j finished, heap= ", node.heap()) -- DEBUG

-- TODO read sensors and report on MQTT; fiddle with actuators
