-- talk.lua: exchange data with server over MQTT
talk={}
local function prtbl(t) for k, v in pairs(t) do print(k, v) end end -- DEBUG
local function recall()
  if not file.open(w.datafile, "r") then return nil end
  _, t = pcall(loadstring("return {" .. (file.read() or "") .. "}"))
  file.close()
  return t
end
local function forget() file.open(w.datafile, "w") file.close() end
function talk.run()
  print("talk.run...")                          -- DEBUG
  t = recall() -- load all persisted key/value pairs from file
  print("talk: t=")                             -- DEBUG
  prtbl(t)                                      -- DEBUG
  forget() -- this step sequence is over; clear the key/value file
  conn=net.createConnection(net.TCP,0) 
  conn:on("receive", function(conn, pl) print("response: ",pl) end)
  conn:on("connection",function(conn, payload)
    print('foo')
    conn:send("POST /?foo=foo HTTP/1.1\r\nHost: saltmarsh.webarch.net\r\n".."Connection: close\r\nAccept: */*\r\n\r\n")
  end)
  return {}
end
return talk
