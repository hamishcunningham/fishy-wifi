-- joinme.lua: JoinMe wifi config utility
joinme={}
local wifiform = [=[<html><body>
<h2>Choose a wifi access point to join</h2><p><form method="POST" action="chz">
_ITEMS_<br/>Pass key: <input type="textarea" name="key"><br/><br/>
<input type="submit" value="Submit"></form></p></body></html>]=]
local function genform(aptbl) -- takes table of APs
  if not aptbl then return "" end
  buf = ""; checked = " checked"
  for ssid, _ in pairs(aptbl) do
    buf = buf .. '<input type="radio" name="ssid" value="' .. ssid .. '"' ..
      checked .. '>' .. ssid .. '<br/>\n'
    checked = ""
  end
  return string.gsub(wifiform, "_ITEMS_", buf)
end
local function httplistener(conn, payload)
  -- print("\n", payload, "\n")
  if string.find(payload, "POST /chz HTTP") then
    ssid, key = string.gmatch(payload, "ssid=(.*)&key=(.*)")()
    if ssid and key then
      wifi.sta.config(ssid, key)
      wifi.sta.connect()
      continuation.taskdata["joinme"] = { ssid=ssid, key=key }
      conn:send("<html><body><h2>Done! Joining...</h2></body></html>")
      conn:on("sent", function (_) srv:close(); continuation.srv = nil end)
-- TODO call back to freak.run here?
      conn:close()
    end
  else
    conn:send(frm)
  end
end
local function aplistener(aptbl)
  frm = genform(aptbl)
  srv = net.createServer(net.TCP)
  continuation.srv = srv
  srv:listen(80, function(conn) conn:on("receive", httplistener) end)
end
function joinme.run(continuation)
  if continuation.srv then return end
  wifi.sta.getap(aplistener)
end
return joinme
