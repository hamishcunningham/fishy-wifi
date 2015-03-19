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
function talk.run()
  print("talk.run...")                          -- DEBUG
  t = recall() -- load all persisted key/value pairs from file
  print("talk: t=")                             -- DEBUG
  prtbl(t)                                      -- DEBUG
  forget() -- this step sequence is over; clear the key/value file
  m = mqtt.Client()
  m:connect("iot.eclipse.org", 1883, 0, function(con) 
    print ("connected")
    for k, v in pairs(t) do
      m:publish(k,v,0,0, function(conn) print("sent") end)  
    end
  end)
end
return talk
