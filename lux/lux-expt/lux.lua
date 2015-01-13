lux = {}
tl = require("tsl2561lib")

local function getFullLuminosity(dev_addr) -- Do the actual reading from the sensor
  tmr.delay(14000)
  ch0low = read_reg(dev_addr, bit.bor(tl.TSL2561_COMMAND_BIT, tl.TSL2561_REGISTER_CHAN0_LOW))
  ch0high = read_reg(dev_addr,bit.bor(tl.TSL2561_COMMAND_BIT, tl.TSL2561_REGISTER_CHAN0_HIGH))
  ch0=string.byte(ch0low)+(string.byte(ch0high)*256)

  ch1low = read_reg(dev_addr, bit.bor(tl.TSL2561_COMMAND_BIT, tl.TSL2561_REGISTER_CHAN1_LOW))
  ch1high = read_reg(dev_addr,bit.bor(tl.TSL2561_COMMAND_BIT, tl.TSL2561_REGISTER_CHAN1_HIGH))
  ch1=string.byte(ch1low)+(string.byte(ch1high)*256)
  return ch0,ch1
end

function lux.entrypoint()
  print("Starting...")
  initialise(addr)
  enable(addr)
  settimegain(addr, tl.TSL2561_INTEGRATIONTIME_13MS, tl.TSL2561_GAIN_0X)
  print("Set integration time and gain")
  chan0,chan1=getFullLuminosity(addr)
end

return lux
