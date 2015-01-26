-- test.lua -- freaky test code

-- are we running on NodeMCU? if not then emulate...
function isnodemcu()
  return type(node) == "romtable" and type(node.info()) == "number"
end
if not isnodemcu() -- define (quick and dirty!) emulation functions
then
  node = {}
  function node.info()          return -1, -1, -1, -1, -1, -1, -1, -1 end
  function node.chipid()        return "|chip id|" end
  function node.heap()          return "|heap|" end
  wifi = {}
  function wifi.setmode(m)      return "|wifi.setmode|" end
  wifi.STATION = 1
  wifi.SOFTAP = 2
  wifi.STATIONAP = 3
  wifi.sta = {}
  function wifi.sta.getmac()    return "|a mac address|" end
  function wifi.sta.getip()     return "|an ip address|" end
  function wifi.sta.config(s,k) return "|wifi.sta.config|" end
  function wifi.sta.connect()   return "|wifi.sta.connect|" end
  function wifi.sta.getap(f)
    return f( { apone= "onedata", aptwo= "twodata", } )
  end
  file = {}
  function file.write(s)        return io.write(s) end
  function file.open(fname)     return io.output(fname) end
  function file.close()         return io.close(io.output()) end
  tmr = {}
  function tmr.alarm(...)       return "|tmr.alarm " .. ... .. "|" end
end

-- utils
function version() -- what version of NodeMCU is this?
  ver1, ver2, ver3 = node.info()
  return string.format("%d.%d.%d", ver1, ver2, ver3)
end
function t2str(t) return serialize(t, "") end
function serialize (o)
  buf = ""
  if type(o) == "string" then
    buf = buf .. string.format("%q, ", o)
  elseif type(o) == "table" then
    buf = buf .. "{\n"
    for k, v in pairs(o) do
      buf = buf .. k .. " = " .. serialize(v)
    end
    buf = buf .. " },\n"
  else
    buf = buf .. tostring(o) .. ", "
  end
  return buf
end
function prtbl(t) for k, v in pairs(t) do print(k, v) end end 
function listap(t) for k,v in pairs(t) do print(k.." : "..v) end end

-- snippets
function snippets()
  print(wifi.getmode())
  print(wifi.sta.getip())
  wifi.sta.getap(listap)
  print(tmr.now())
  wifi.setmode(wifi.STATION)
  print(node.heap())
  print(node.info())
end
