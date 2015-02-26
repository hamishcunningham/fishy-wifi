-- bat.lua
-- read supply voltage when used with a 100k/470k resistor divider
-- for vcc - maximum is about 4.9v - tuned for 3AA cells
bat = {}
local function volts()
  local samples = 100
  local result = 0
  for i=1, samples do result = result + adc.read(0) end
  voltage = string.format("%.2f", result/samples * 0.0054)
  print("battery level: ", voltage)     -- DEBUG
  return voltage
end
bat.run = function()
  v = volts()
  w.store("volts", v)
  return v
end
return bat
