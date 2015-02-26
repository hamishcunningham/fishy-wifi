-- init.lua: call the top level entry point for the WeGrow board
datafile="memory.lua" -- persistence globals (used by all steps)
function store(k, v)
  file.open(datafile, "a")
  file.write(k .. "=" .. '"' .. v .. '", ')
  file.close()
end
function recall()
  if not file.open(datafile, "r") then return nil end
  _, t = pcall(loadstring("return {" .. (file.read() or "") .. "}"))
  file.close()
  return t
end
function forget() file.open(datafile, "w") file.close() end
print("delete init.lua now if you need to!") -- DEBUG
tmr.alarm(0, 1500, 0, require("wegrow").run) -- panic space!
