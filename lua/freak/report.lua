-- report.lua: tell the world
report = {}
function report.run(...)
  print("report.run...")
  -- TODO do MQTT stuff

  freak.continue(ctn) -- give control back to the control freak
end
return report
