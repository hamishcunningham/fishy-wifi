-- j.lua: _joinme_ wifi config utility
j={}
local frm
local skip = "skipj.txt"        -- marker file

local function genform(aptbl)   -- takes table of APs
  print("j.genform")            -- DEBUG
  if not aptbl then return "<html><body>No AP data :-(</body></html>" end
  wfrm = [=[<html><body>
<h2>Choose a wifi access point to join</h2><p><form method="POST" action="chz">
_ITEMS_<br/>Pass key: <input type="textarea" name="key"><br/><br/>
<input type="submit" value="Submit"></form></p></body></html>]=]
  buf = ""; chked = " checked"
  for ssid, _ in pairs(aptbl) do
    buf = buf .. '<input type="radio" name="ssid" value="' .. ssid .. '"' ..
      chked .. '>' .. ssid .. '<br/>\n'
    chked = ""
  end
  return string.gsub(wfrm, "_ITEMS_", buf)
end
local function finish()         -- reclaim resources, hand back control
  print("j.finish, heap=", node.heap()) -- DEBUG
  file.open(skip, "w")          -- remember not to do this next time
  file.close()
  srvr:close()                  -- kill server
  tmr.alarm(0, 1000, 0, node.restart) -- wait second, start over
end
local function httplstn(conn, req) -- serve HTTP requests
  print("processing req: ", req:sub(1, 11)) -- DEBUG
  if string.find(req, "POST /chz HTTP") then
    ssid, key = string.gmatch(req, "ssid=(.*)&key=(.*)")()
    if ssid and key then -- TODO verify ssid and key better
      wifi.sta.config(ssid, key)
      wifi.sta.connect()
      conn:send("<html><body><h2>Done! Joining...</h2></body></html>")
      return conn:on("sent", finish())
    end
  end
  conn:send(frm)
end

function j.aplstn(aptbl)        -- callback for available APs scanner
  print("j.aplstn") 
  frm = genform(aptbl)
  if not srvr then srvr = net.createServer(net.TCP) end
  srvr:listen(80, function(conn) conn:on("receive", httplstn) end)
end

return j
