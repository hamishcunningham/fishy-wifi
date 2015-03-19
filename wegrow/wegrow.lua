-- wegrow.lua: top level entry point for the WeGrow sensor/actuator board
w={}
-- steps: each is a module defining a run() function
-- which must call done() when it is has finished
local step
local steps = { "bat", "sense", "talk"} -- processing sequence
local stime = 3     -- deepsleep duration after each step (in seconds)
local stepf="step.txt"  -- persistence file for the next step index
w.datafile="memory.lua" -- general key/value persistence file
local function store(k, v)  -- remember a key/value pair
  file.open(w.datafile, "a")
  file.write(k .. "=" .. '"' .. v .. '", ')
  file.close()
end

local function w.done(data, nosleep) 
  for k,v in pairs(data) do store(k, v) end
  if step == #steps then step = 0 end
  file.open(stepf, "w")
  file.write(step + 1)
  file.close()
  if nosleep then return w.run(done) end  -- do next step
  print("sleep ", stime, "s")       -- DEBUG
  return node.dsleep(stime * 1000000)
end

function w.run()
  -- TODO setup timer to go to deep sleep, in case a step doesn't return?
  step=1 -- index into the list of steps; 1 by default, or set from file
  if file.open(stepf, "r") then step = 0 + file.read(); file.close() end              -- DEBUG
  print(step)
  return require(steps[step]).run()
end

return w
