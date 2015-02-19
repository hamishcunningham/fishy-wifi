-- act.lua: fire up the actuators Scotty!
act = {}
function act.run(ctn)
  print("act.run...")

  -- TODO fiddle with actuators here...

  freak.continue(ctn) -- give control back to the control freak
end
return act
