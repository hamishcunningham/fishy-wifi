-- Code to read TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit
-- Special hat-tip to lady ada - hacker hero - amongst many others
-- This code is AGPL v3 by gareth@l0l.org.uk and Hamish Cunningham
-- blah blah blah standard licence conditions apply blah blah blah

tl = require("tsl2561lib")
lux = {}

-- this maps GPIO numbers to internal IO references
io_pin= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5},
tl.sda=tl.io_pin[14] -- connect to pin GPIO14
tl.scl=tl.io_pin[12] -- connect to pin GPIO12
tl.addr=tl.TSL2561_ADDR_FLOAT -- I2C address of device with ADDR pin floating

function lux.entrypoint()
  print("Starting...")
  addr = 0x39
  result=tl.initialise(addr, bit.bor(tl.TSL2561_COMMAND_BIT,tl.TSL2561_REGISTER_ID))
  if string.byte(result) == 0x50 then print("Initialised TSL2561T/FN/CL") end
  tl.enable(addr)
  tl.settimegain(addr, tl.TSL2561_INTEGRATIONTIME_13MS, tl.TSL2561_GAIN_0X)
  print("Set integration time and gain")
  chan0,chan1=tl.getFullLuminosity(addr)
  return chan0, chan1
end

return lux
