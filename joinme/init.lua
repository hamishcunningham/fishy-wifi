-- init.lua: top level entry point for the freakaponics sensor/actuator board

wifi.setmode(wifi.STATIONAP)    -- coding around nil result to first call issue
function lstf(t) if t then for k,_ in pairs(t) do print(k) end end end
wifi.sta.getap(lstf)            -- consume nil result

print("starting wifi setup, heap= ", node.heap()) -- DEBUG
if file.open("skipj.txt") then  -- we're already configured
  print("skipj exists")         -- DEBUG
  file.close()
  f=require("f")
tmr.alarm(0, 2000, 0, f.run)
-- f.run()
else
  j=require("j")
  wifi.sta.getap(j.aplstn)      -- joinme entry point
end -- wifi config
print("wifi config finished, heap= ", node.heap()) -- DEBUG
