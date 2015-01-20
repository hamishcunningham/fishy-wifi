-- freak.lua
--[[ A control utility to facilitate complex tasks in tight spaces on the
ESP8266 by allowing periodic restarts to reclaim memory. The task is split
into a set of phases, each represented by a Lua package that implements a
`run` function. Freak loops over these functions, passing (small amounts!) of
data along the pipeline. This data and a record of which phase is due next is
persisted to the filesystem during restarts. ]]
freak = {}
local cfile = "freakdata.lua"
local function getconf() return pcall(dofile, cfile) or {} end
local function conf2string(conf)
  buf = "{"
  for k, v in pairs(conf) do buf = buf .. string.format(' %s="%s",', k, v) end
  return buf .. " }\n"
end
local function writeconf(conf)
  f = file.open(cfile, "w")
  if not f then return nil end
  file.write("return " .. conf2string(conf))
  file.close()
  return true
end

-- oops: mixing two styles here; we should plump for one
-- (i.e. keep all the data in the continuation? and callback here from 
-- each phase, then do the restarts here too? could even measure heap
-- before deciding: if(node.getheap()) <= 20k)...
local function run(stepindex, continuation)
  while true do
    stepindex = stepindex + 1
    continuation.nextphase = stepindex
    if(stepindex > #continuation) then stepindex = 1 end
    for _, p in continuation do
      phasekey = p[1]; phasedata = p[2]
      if type(phasekey) == "function" then
        pcall(phasekey, continuation)
      elseif type(phasekey) == "string" then
        phasechunk = require(phasekey)
        pcall(phasechunk.run, continuation)
      end
    end
  end
end
function freak.run(continuation)
  cdata = getconf()
  return run(cdata.stepindex or 1, continuation, {})
end
return freak
