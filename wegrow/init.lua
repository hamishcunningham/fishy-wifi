-- init.lua: top level entry point for the WeGrow sensor/actuator board

function run()
  print("starting wifi setup, heap= ", node.heap()) -- DEBUG
  if file.open("skipj.txt") then        -- we're already configured
    print("skipj exists")               -- DEBUG
    file.close()
  else
    j=require("j")
    wifi.sta.getap(j.aplstn)            -- joinme entry point
  end
  print("wifi config finished, heap= ", node.heap()) -- DEBUG

  print("setting up stepper...")        -- DEBUG
  steps = { "bat", "soil" }
  sleeptime = 3 -- in seconds
  step=1
  stepf="step.txt"
  if file.open(stepf, "r") then step = 0 + file.read(); file.close() end
  stepname = steps[step]

  print("taking a step: ", stepname)    -- DEBUG
  s = require(stepname)
  s.run()

  print("incrementing step number")     -- DEBUG
  if step == #steps then step = 0 end
  file.open(stepf, "w")
  file.write(step + 1)
  file.close()

  print("sleeping for ", sleeptime, " secs...")
  node.dsleep(sleeptime * 1000000)
end

print("panic now if you're in that sort of mood") -- DEBUG
tmr.alarm(0, 1500, 0, run)              -- panic space!
