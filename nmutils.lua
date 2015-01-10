-- nmutils.lua 
-- NodeMCU utilities

nmutils = {} -- this is a package

function nmutils.isnodemcu() -- are we running on NodeMCU?
  return type(node) == "romtable" and type(node.info()) == "number"
end
function nmutils.version() -- what version of NodeMCU is this?
  ver1, ver2, ver3 = node.info()
  return string.format("%d.%d.%d", ver1, ver2, ver3)
end

if not nmutils.isnodemcu() -- define emulation functions
then
  -- TODO
end

return nmutils -- return the package table
