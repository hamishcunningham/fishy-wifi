-- report.lua: tell the world
report = {}
function report.run(...)
  for i,v in continuation do print("report... i: ", i, "; v: ", v) end
  -- TODO do MQTT stuff
-- TODO continuation.freak.continue(continuation)
end
return report
