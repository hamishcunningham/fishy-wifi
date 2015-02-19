-- soil.lua -- soil moisture sensor
soil={}
function soil.run()
  print("soil.run...")             -- DEBUG
--[[
  pin=1
  pause=16000
  readings={}
  sum=0
  for i=1,1000 do
    gpio.mode(pin, gpio.OUTPUT)
    gpio.write(pin, gpio.LOW)
    tmr.wdclr()
    tmr.delay(pause)

    c=0
    gpio.mode(pin, gpio.INPUT)
    while gpio.read(pin) == gpio.LOW do
      tmr.wdclr()
      c = c + 1
    end
    sum = sum + c
  end
  print("cap reading: ", sum/1000)
]]
  return nil
end
return soil
