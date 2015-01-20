-- freak.lua: a control utility

freak = {}
local cfile = "freakdata.lua"
local function getconf() return pcall(dofile, cfile) or {} end
function freak.continue()
  fconf = getconf()
  -- choose what code to load ...
  phasename = fconf.phase
  phase = require(phasename)
  return phase()
end

function control(phases) return control(1, phases, {}) end
function control(stepindex, phases, continuation)
  for _, p in phases
  do
    p = phases[stepindex]
    if type(p) == "function" then
      return pcall(p, continuation)
    elseif type(p) == "string" then
      phasechunk = require(p)
      return pcall(phasechunk.doit, continuation)
    end
  end
end

return freak
