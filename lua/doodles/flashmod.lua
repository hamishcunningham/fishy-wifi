-- flashmod.lua
-- from http://www.esp8266.com/viewtopic.php?f=19&t=1940
flash = {MOD_NAME = "flash"}
function flash.flashMod(tbl)
  if type(tbl) == "string" then tbl = {MOD_NAME=tbl} end 
  for k,v in pairs(tbl) do
    if type(v) == "function" then
      file.open(string.format("%s_%s.lc", tbl.MOD_NAME, k), "w+")
      file.write(string.dump(v))
      file.close()
      tbl[k] = nil
    end
  end
  return setmetatable(tbl, {
    __index = function(t, k)
      return assert(loadfile(string.format("%s_%s.lc",t.MOD_NAME,k)))
    end
  })
end
flash.flashMod(flash)
flash = nil
module = nil
package = nil
newproxy = nil
require = nil
collectgarbage()
function flashMod(tbl) return loadfile("flash_flashMod.lc")(tbl) end


-- TODO move the rest
-- flashMod can be called in two ways:

flashMod(table)
-- serializes all the functions on the table to flash, removes them, and 
-- augments the table with a metatable that loads the functions on demand.

flashMod("modName")
-- creates an empty table with the metatable populated. Use this if you want 
-- to create the module in one file with the first version, but don't need to 
-- attach any additional in-memory methods or data.

-- so, a full module with public, private, and flash methods will take this
-- form now:
mod = { MOD_NAME = "my_mod"}
function mod.flashFunc1() end
function mod.flashFunc2() end
flashMod(mod) --everything before this is a flash function, everything after it is an in-memory public function
local function privateFunc() end --only visible to public and other private functions
local privateData  --ditto
function mod.publicFunc() end
return mod
