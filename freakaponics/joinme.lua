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
local function finish() -- reclaim resources and hand back control
  print("joinme.finish")
  ctn.srv:close(); ctn.srv = nil -- kill the server
  ctn.taskdata.nexttask = ctn.taskdata.nexttask + 1 -- increment task number
  ctn.freak.continue(ctn) -- give control back to the control freak
end
local function httplistener(conn, payload) -- serve HTTP requests
  print("processing web request")
  if string.find(payload, "POST /chz HTTP") then
    ssid, key = string.gmatch(payload, "ssid=(.*)&key=(.*)")()
    if ssid and key then
      wifi.sta.config(ssid, key)
      wifi.sta.connect()
      ctn.taskdata["joinme"] = { ssid=ssid, key=key, skipme=true }
      conn:send("<html><body><h2>Done! Joining...</h2></body></html>")
      conn:on("sent", finish)
    end
  else
    conn:send(frm)
  end
end
local function aplistener(aptbl) -- callback for available APs scanner
  frm = genform(aptbl)
  ctn.srv = net.createServer(net.TCP)
  ctn.srv:listen(80, function(conn) conn:on("receive", httplistener) end)
end
function joinme.run(ctn) -- external entry point
  if ctn.srv then return end
  wifi.sta.getap(aplistener)
end
return joinme
