-- talk.lua: exchange data with server over MQTT
talk={}
local function prtbl(t) for k, v in pairs(t) do print(k, v) end end
function talk.run()
  print("talk.run...")                          -- DEBUG
  t = w.recall()
  print("talk: t=")
  prtbl(t)
  w.forget()
  return nil
end
return talk
