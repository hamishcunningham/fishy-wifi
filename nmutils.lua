-- nmutils.lua 
-- NodeMCU utilities

-- a table for the package
nmutils = {}

-- are we running on NodeMCU? if not then emulate...
local function nmutils.isnodemcu()
  return type(node) == "romtable" and type(node.info()) == "number"
end
if not nmutils.isnodemcu() -- define emulation functions
then
  node = {}
  function node.info() return -1, -1, -1, -1, -1, -1, -1, -1 end
end

-- utils
local function nmutils.version() -- what version of NodeMCU is this?
  ver1, ver2, ver3 = node.info()
  return string.format("%d.%d.%d", ver1, ver2, ver3)
end

-- return the package table
return nmutils
