-- act.lua: fire up the actuators Scotty!
act = {}
function act.run(ctn)
  for i,v in ctn do print("act... i: ", i, "; v: ", v) end

  -- TODO fiddle with actuators here...

  ctn.taskdata.nexttask = ctn.taskdata.nexttask + 1 -- increment task number
  freak.continue(ctn) -- give control back to the control freak
end
return act
