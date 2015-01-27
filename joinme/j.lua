-- j.lua: wifi config utility
j={}
local skip = "skipj.txt"        -- marker file
local wifiform = [=[<html><body>
<h2>Choose a wifi access point to join</h2><p><form method="POST" action="chz">
_ITEMS_<br/>Pass key: <input type="textarea" name="key"><br/><br/>
<input type="submit" value="Submit"></form></p></body></html>]=]
local function genform(aptbl)   -- takes table of APs
  print("j.genform")            -- DEBUG
  if not aptbl then return "<html><body>No AP data :-(</body></html>" end
  buf = ""; checked = " checked"
  for ssid, _ in pairs(aptbl) do
    buf = buf .. '<input type="radio" name="ssid" value="' .. ssid .. '"' ..
      checked .. '>' .. ssid .. '<br/>\n'
    checked = ""
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end
local function finish()         -- reclaim resources and hand back control
  print("j.finish, heap=", node.heap()) -- DEBUG
  file.open(skip, "w")          -- remember not to do this next time through
  file.close()
  srvr:close()                  -- kill the server
end
local function httplistener(conn, payload) -- serve HTTP requests
  print("processing web request: ", payload:sub(1, 11)) -- DEBUG
  if string.find(payload, "POST /chz HTTP") then
    ssid, key = string.gmatch(payload, "ssid=(.*)&key=(.*)")()
    if ssid and key then -- TODO verify ssid and key more effectively
      wifi.sta.config(ssid, key)
      wifi.sta.connect()
      conn:send("<html><body><h2>Done! Joining...</h2></body></html>")
      conn:on("sent", finish)
    end
  else
    conn:send(frm)
  end
end
function j.aplistener(aptbl)    -- callback for available APs scanner
  print("j.aplistener")         -- DEBUG
  frm = genform(aptbl)
  if not srvr then srvr = net.createServer(net.TCP) end
  srvr:listen(80, function(conn) conn:on("receive", httplistener) end)
end
function j.reset() file.remove(skip) end
return j
