-- act.lua: fire up the actuators Scotty!
act = {}
function act.run(continuation)
  for i,v in continuation do print("act... i: ", i, "; v: ", v) end
  -- TODO fiddle with actuators here...
end
return act
