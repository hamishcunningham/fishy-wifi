id=0
pin=10

-- io map --
-- 3 to GPIO0, 4 to GPIO2
gpio.mode(pin,gpio.OUTPUT)

for i=0,127 do
     gpio.write(pin,gpio.HIGH)
     tmr.delay(10000)
     tmr.wdclr()
     gpio.write(pin,gpio.LOW)
     tmr.delay(10000)
     tmr.wdclr()     
     end
