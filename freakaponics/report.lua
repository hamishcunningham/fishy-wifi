-- report.lua: tell the world
report = {}
function report.run(...)
  for i,v in ctn do print("report... i: ", i, "; v: ", v) end
  -- TODO do MQTT stuff

  ctn.taskdata.nexttask = ctn.taskdata.nexttask + 1 -- increment task number
  freak.continue(ctn) -- give control back to the control freak
end
return report
