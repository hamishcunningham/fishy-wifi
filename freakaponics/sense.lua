-- sense.lua: read those sensors (it's life Jim, but not as we know it)
sense = {}
function sense.run(ctn)
  for i,v in ctn do print("sense... i: ", i, "; v: ", v) end
  -- TODO do sensor stuff

  ctn.taskdata.nexttask = ctn.taskdata.nexttask + 1 -- increment task number
  freak.continue(ctn) -- give control back to the control freak
end
return sense

