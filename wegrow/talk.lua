-- talk.lua: exchange data with server over MQTT
talk={}
local function prtbl(t) for k, v in pairs(t) do print(k, v) end end -- DEBUG
local function recall()
  if not file.open(w.datafile, "r") then return nil end
  _, t = pcall(loadstring("return {" .. (file.read() or "") .. "}"))
  file.close()
  return t
end
local function forget() 
  file.open(w.datafile, "w")
  file.close()
end
local function pub(k, v)
  if not k then
    return w.done({}, false, false) -- no data left so go to next step
  end
  m:publish(k,v,0,0, function() -- pub, wait, pub next - otherwise complains
    pub(next(t, k))
  end)  
end
function talk.run()
  print("talk.run...")                          -- DEBUG
  t = recall() -- load all persisted key/value pairs from file
  print("talk: t=")                             -- DEBUG
  prtbl(t)                                      -- DEBUG
  forget() -- this step sequence is over; clear the key/value file
  m = mqtt.Client()
  m:connect("iot.eclipse.org", 1883, 0, function(con) 
    print ("connected")
    pub(next(t, nil))
  end)
end
return talk
