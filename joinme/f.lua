-- f.lua -- fishy sensors etc.

f={}
function f.run()
  -- TODO read sensors and report on MQTT; fiddle with actuators
  print("f.run...")             -- DEBUG

  -- TODO experimental capacitative charge reading (not working?)
  pin=1
  pause=1600
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
      c = c + 1
    end
    sum = sum + c
  end
  print("cap reading: ", sum/1000)

  return nil
end
return f
