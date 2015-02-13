-- we need to setup some pins to turn led's off
for pin=1,8 do
     gpio.mode(pin,gpio.OUTPUT)
end
for pin=1,5 do
     gpio.write(pin,gpio.HIGH)
end
for pin=6,8 do
     gpio.write(pin,gpio.LOW)
end

uart.setup(0,115200,8,0,1,1)
print()
majorVer, minorVer, devVer, chipid, flashid, flashsize, flashmode, flashspeed = node.info();
print("NodeMCU "..majorVer.."."..minorVer.."."..devVer)
print('init.lua ver 0.3 by gareth based on 4refr0nt')