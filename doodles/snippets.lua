print(wifi.getmode())
print(wifi.sta.getip())

function prtbl(t) for k, v in pairs(t) do print(k, v) end end 

function listap(t)
  for k,v in pairs(t) do
    print(k.." : "..v)
  end
end

wifi.sta.getap(listap)

ver1, ver2, ver3 = node.info()
print(ver1, ver2, ver3)

print(tmr.now())

wifi.setmode(wifi.STATION)

function isnodemcu()
  return type(node) == "romtable" and type(node.info()) == "number"
end
print("isnodemcu(): ", isnodemcu())

print(node.heap())

dofile("init.lua")

print(node.info())
