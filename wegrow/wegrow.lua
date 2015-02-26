-- wegrow.lua: top level entry point for the WeGrow sensor/actuator board
datafile="memory.lua"                   -- persistence
w={}
local steps = { "bat", "soil", "talk" } -- processing sequence
local sleeptime = 3   -- deepsleep duration after each step (in seconds)
function w.store(k, v)
  file.open(datafile, "a")
  file.write(k .. "=" .. '"' .. v .. '", ')
  file.close()
end
function w.recall()
  if not file.open(datafile, "r") then return nil end
  _, t = pcall(loadstring("return {" .. (file.read() or "") .. "}"))
  file.close()
  return t
end
function w.forget() file.open(datafile, "w") file.close() end
function w.run()
  print("starting wifi setup, heap= ", node.heap()) -- DEBUG
  if file.open("skipj.txt") then        -- we're already configured
    print("skipj exists")               -- DEBUG
    file.close()
  else
    j=require("j")
    return wifi.sta.getap(j.aplstn)     -- joinme entry point
  end
  print("wifi config finished, heap= ", node.heap()) -- DEBUG

  step=1
  stepf="step.txt"
  if file.open(stepf, "r") then step = 0 + file.read(); file.close() end
  stepname = steps[step]
  print("taking a step: ", stepname)    -- DEBUG
  s = require(stepname)
  s.run()
  if step == #steps then step = 0 end
  file.open(stepf, "w")
  file.write(step + 1)
  file.close()

  print("sleeping for ", sleeptime, " secs...")
  node.dsleep(sleeptime * 1000000)
end
return w
