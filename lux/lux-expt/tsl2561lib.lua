-- This code is AGPL v3 by gareth@l0l.org.uk
-- blah blah blah standard licence conditions apply blah blah blah
-- Special hat-tip to lady ada - hacker hero - amongst many others
-- Reads value of TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit

-- the package table, and lots of constants
tsl2561lib = {
  TSL2561_ADDR_FLOAT            = 0x39,
  TSL2561_COMMAND_BIT           = 0x80,   -- Must be 1
  TSL2561_CONTROL_POWERON       = 0x03,
  TSL2561_CONTROL_POWEROFF      = 0x00,
  TSL2561_REGISTER_CONTROL      = 0x00,
  TSL2561_REGISTER_TIMING       = 0x01,
  TSL2561_REGISTER_ID           = 0x0A,
  TSL2561_REGISTER_CHAN0_LOW    = 0x0C,
  TSL2561_REGISTER_CHAN0_HIGH   = 0x0D,
  TSL2561_REGISTER_CHAN1_LOW    = 0x0E,
  TSL2561_REGISTER_CHAN1_HIGH   = 0x0F,

  TSL2561_INTEGRATIONTIME_13MS  = 0x00,   -- 13.7ms
  TSL2561_INTEGRATIONTIME_101MS = 0x01,   -- 101ms
  TSL2561_INTEGRATIONTIME_402MS = 0x02,   -- 402ms

  TSL2561_GAIN_0X               = 0x00,   -- No gain
  TSL2561_GAIN_16X              = 0x10,   -- 16x gain

  TSL2561_LUX_CHSCALE           = 10,     -- Scale channel values by 2^10
  TSL2561_LUX_CHSCALE_TINT0     = 0x7517, -- 322/11 * 2^TSL2561_LUX_CHSCALE
  TSL2561_LUX_LUXSCALE          = 14,     -- Scale by 2^14
  TSL2561_LUX_RATIOSCALE        = 9,      -- Scale ratio by 2^9

  -- T, FN and CL package values
  TSL2561_LUX_K1T               = 0x0040, -- 0.125 * 2^RATIO_SCALE
  TSL2561_LUX_B1T               = 0x01f2, -- 0.0304 * 2^LUX_SCALE
  TSL2561_LUX_M1T               = 0x01be, -- 0.0272 * 2^LUX_SCALE
  TSL2561_LUX_K2T               = 0x0080, -- 0.250 * 2^RATIO_SCALE
  TSL2561_LUX_B2T               = 0x0214, -- 0.0325 * 2^LUX_SCALE
  TSL2561_LUX_M2T               = 0x02d1, -- 0.0440 * 2^LUX_SCALE
  TSL2561_LUX_K3T               = 0x00c0, -- 0.375 * 2^RATIO_SCALE
  TSL2561_LUX_B3T               = 0x023f, -- 0.0351 * 2^LUX_SCALE
  TSL2561_LUX_M3T               = 0x037b, -- 0.0544 * 2^LUX_SCALE
  TSL2561_LUX_K4T               = 0x0100, -- 0.50 * 2^RATIO_SCALE
  TSL2561_LUX_B4T               = 0x0270, -- 0.0381 * 2^LUX_SCALE
  TSL2561_LUX_M4T               = 0x03fe, -- 0.0624 * 2^LUX_SCALE
  TSL2561_LUX_K5T               = 0x0138, -- 0.61 * 2^RATIO_SCALE
  TSL2561_LUX_B5T               = 0x016f, -- 0.0224 * 2^LUX_SCALE
  TSL2561_LUX_M5T               = 0x01fc, -- 0.0310 * 2^LUX_SCALE
  TSL2561_LUX_K6T               = 0x019a, -- 0.80 * 2^RATIO_SCALE
  TSL2561_LUX_B6T               = 0x00d2, -- 0.0128 * 2^LUX_SCALE
  TSL2561_LUX_M6T               = 0x00fb, -- 0.0153 * 2^LUX_SCALE
  TSL2561_LUX_K7T               = 0x029a, -- 1.3 * 2^RATIO_SCALE
  TSL2561_LUX_B7T               = 0x0018, -- 0.00146 * 2^LUX_SCALE
  TSL2561_LUX_M7T               = 0x0012, -- 0.00112 * 2^LUX_SCALE
  TSL2561_LUX_K8T               = 0x029a, -- 1.3 * 2^RATIO_SCALE
  TSL2561_LUX_B8T               = 0x0000, -- 0.000 * 2^LUX_SCALE
  TSL2561_LUX_M8T               = 0x0000, -- 0.000 * 2^LUX_SCALE

  id                            = 0,      -- identify (software) I2C bus

  -- this maps GPIO numbers to internal IO references
  io_pin= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5},
}

-- add a few recursive definitions
tl = tsl2561lib
tl.sda=tl.io_pin[14] -- connect to pin GPIO14
tl.scl=tl.io_pin[12] -- connect to pin GPIO12
tl.addr=tl.TSL2561_ADDR_FLOAT -- I2C address of device with ADDR pin floating

function tl.read_reg(dev_addr, reg_addr) --  read a single byte from register
  id = tl.id
  i2c.start(id)
  i2c.address(id, dev_addr, i2c.TRANSMITTER)
  i2c.write(id,reg_addr)
  i2c.stop(id)
  i2c.start(id)
  i2c.address(id, dev_addr,i2c.RECEIVER)
  c=i2c.read(id,1)
  print("read command 0x"..string.format("%02X",reg_addr),"0x"..string.format("%02X",string.byte(c)))
  i2c.stop(id)
  return c
end

function tl.write_reg(dev_addr, reg_addr, reg_val)
  id = tl.id
  i2c.start(id)
  i2c.address(id, dev_addr, i2c.TRANSMITTER)
  i2c.write(id, reg_addr)
  i2c.write(id, reg_val)
  i2c.stop(id)
  print("write command 0x"..string.format("%02X",reg_addr),"0x"..string.format("%02X",reg_val))
  return c
end

function tl.initialise(dev_addr) -- initialize i2c with our id and pins in slow mode :-)
  id = tl.id
  i2c.setup(id, tl.sda, tl.scl, i2c.SLOW)
  result=tl.read_reg(dev_addr,bit.bor(tl.TSL2561_COMMAND_BIT,tl.TSL2561_REGISTER_ID))
  if string.byte(result) == 0x50 then
    print("Initialised TSL2561T/FN/CL")
  end
end

function tl.enable(dev_addr) -- enable the device
  write_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_CONTROL), TSL2561_CONTROL_POWERON)
-- if we add the below statement, then require("lux") will trigger reboot
-- --[[
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
  print(" ")
-- ]] -- 
end

function tl.disable(dev_addr) -- disable the device
  write_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_CONTROL), TSL2561_CONTROL_POWEROFF)
end

function tl.settimegain(dev_addr, time, gain) -- set the integration time and gain together
  write_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_TIMING), bit.bor(time,gain))
end

function tl.getFullLuminosity(dev_addr) -- Do the actual reading from the sensor
  tmr.delay(14000)
  ch0low = read_reg(dev_addr, bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN0_LOW))
  ch0high = read_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN0_HIGH))
  ch0=string.byte(ch0low)+(string.byte(ch0high)*256)

  ch1low = read_reg(dev_addr, bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN1_LOW))
  ch1high = read_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN1_HIGH))
  ch1=string.byte(ch1low)+(string.byte(ch1high)*256)
  return ch0,ch1
end

-- print("Setup 1 complete")

return tl
