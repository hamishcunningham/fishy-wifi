-- init.lua: call the top level entry point for the WeGrow board
print("delete init.lua now if you need to!") -- DEBUG
pausemsecs=50 -- production pause length; will be active when minified TODO test
pausemsecs=1500 -- DEBUG pause length
tmr.alarm(0, pausemsecs, 0, function()
  if file.open("skipj.txt") then        -- we're already configured
    print("skipj exists")               -- DEBUG
    file.close()
    require("wegrow").run()
  else
    require("j").run()
  end
end)
