-- Code to read TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit
-- Special hat-tip to lady ada - hacker hero - amongst many others

lux = {}
tl = require("tsl2561lib")
i2cutils = require("i2cutils")

-- this maps GPIO numbers to internal IO references
local io_pin = {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5}
local sda = io_pin[14] -- connect to pin GPIO14
local scl = io_pin[12] -- connect to pin GPIO12
local busid = 0 -- i2c bus id

function lux.getlux()
  dev_addr = tl.TSL2561_ADDR_FLOAT -- I2C address of device with ADDR pin floating
  i2c.setup(busid, sda, scl, i2c.SLOW)
  result = i2cutils.read_reg(
    dev_addr,
    bit.bor(tl.TSL2561_COMMAND_BIT, tl.TSL2561_REGISTER_ID)
  )
  --if string.byte(result) == 0x50 then print("Initialised TSL2561T/FN/CL") end
  tl.enable(dev_addr)
  tl.settimegain(dev_addr, tl.TSL2561_INTEGRATIONTIME_13MS, tl.TSL2561_GAIN_16X)
  tl.disable(dev_addr)
  tmr.delay(1000) -- give 1ms for sensor to settle
  tl.enable(dev_addr)
  tmr.delay(14000) -- gives 14ms for integration time
  chan0,chan1 = tl.getFullLuminosity(dev_addr)
  tl.disable(dev_addr)
  return chan0, chan1
end

return lux
