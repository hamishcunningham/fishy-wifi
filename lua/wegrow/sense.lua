-- sense.lua -- sensor work
sense={}
-- local pinreceive = 1
local pinsend = 6
function sense.xrun() 
  print("sense.run()...") -- DEBUG
  gpio.mode(pinsend, gpio.OUTPUT) 
  -- gpio.mode(pinreceive, gpio.INPUT, gpio.FLOAT)

  -- gpio.write(pinsend, gpio.HIGH) -- make sure we're discharged to start with
  print("adc.read 1...", adc.read(0)) -- DEBUG
  -- tmr.delay(1000000)
  gpio.write(pinsend, gpio.LOW)
  tmr.delay(1000000)
  start = tmr.now()
  timeout = start + 5000000
  -- print("adc.read 2...", adc.read(0)) -- DEBUG
  i=0
  max=0
  while tmr.now() <= timeout do -- and adc.read(0) <= 1500 do
    i = i + 1
    val = adc.read(0)
    if max < val then max = val end
    print("val...", val) -- DEBUG
    gpio.write(pinsend, gpio.HIGH)
 --   tmr.delay(10)
    gpio.write(pinsend, gpio.LOW)
   --  print("now= ", tmr.now(), " timeout= ", timeout) -- DEBUG
  --  tmr.delay(10)
    tmr.wdclr()
  end
  print("adc.read 4...", adc.read(0), "i= ", i, "max= ", max) -- DEBUG
  print("done the loop") -- DEBUG
  elapsed = tmr.now() - start

  moisture = elapsed
  return { sense = 345345, moisture = moisture, } 
end
function sense.run()
  moisture = 1000
  return { sense = 345345, moisture = moisture, } 
end
return sense
