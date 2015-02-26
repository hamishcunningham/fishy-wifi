-- init.lua: call the top level entry point for the WeGrow board
print("delete init.lua now if you need to!") -- DEBUG
tmr.alarm(0, 1500, 0, require("wegrow").run) -- panic space!
