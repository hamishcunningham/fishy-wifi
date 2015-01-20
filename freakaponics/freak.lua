-- freak.lua
--[[ A control utility to facilitate complex tasks in tight spaces on the
ESP8266 by allowing periodic restarts to reclaim memory. The task is split
into a set of phases, each represented by a Lua package that implements a
`run` function. Freak loops over these functions, passing (small amounts!) of
data along the pipeline. This data and a record of which phase is due next is
persisted to the filesystem during restarts. ]]
freak = {}
local cfile = "freakdata.lua"
local minheap = 20000 -- if we've dropped below this after a phase, restart
local function getconf() return pcall(dofile, cfile) or {} end
local function tbl2str(t)
  buf = "{ "
  for k, v in pairs(t) do
    buf == buf .. k "="
    if type(v) = table then buf = buf .. tbl2str(v)
    else buf = buf
    end
    buf .. ","
  end
  return buf .. " }\n"
end
local function persist(c)
  f = file.open(cfile, "w")
  if not f then return nil end
  buf = "{"
  for k, v in pairs(c) do buf = buf .. string.format(' %s="%s",', k, v) end
  file.write("return " .. buf .. " }\n")
  file.close()
  return true
end
local function run(stepindex, continuation)
  stepindex = stepindex + 1
  if(stepindex > #continuation) then stepindex = 1 end
  p = continuation[stepindex]
  phasekey = p[1]
  phasedata = p[2]
  if type(phasekey) == "function" then
    pcall(phasekey, continuation)
  elseif type(phasekey) == "string" then
-- TODO check preconditions
pcall(continuation.taskdata.phasekey, continuation)
    phasechunk = require(phasekey)
    pcall(phasechunk.run, continuation)
  end
  if node.getheap() < minheap then
    continuation.nextphase = stepindex
    persist(continuation)
    node.restart() -- reset the chip and start over
  end
  run(stepindex, continuation) -- run the next phase
end
function freak.run(continuation)
  cdata = getconf()
  return run(continuation.nextphase or 1, continuation)
end
return freak
