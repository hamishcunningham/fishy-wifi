-- Library for working with TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit
-- Special hat-tip to lady ada - hacker hero - amongst many others
-- This code is AGPL v3 by gareth@l0l.org.uk and Hamish Cunningham
-- blah blah blah standard licence conditions apply blah blah blah
-- Adapted by Aeprox@github for use in ESPlogger
-- Altered by DrRob@github to try up to four different combinations
--   of gain on/off and integration time 101ms or 402ms to get a 
--   fuller range but retain the best possible resolution.
--   See: http://ideasandbox.blogspot.co.uk/2015/06/tsl2561-light-sensor-ranges.html

local i2cutils = require("i2cutils") -- get our helper functions loaded

local moduleName = "tsl2561lib"
-- the package table, and lots of constants
local M = {
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
    
    TSL2561_GAIN_1X               = 0x00,   -- No gain
    TSL2561_GAIN_16X              = 0x10,   -- 16x gain
}
_G[moduleName] = M

local busid = 0 -- i2c bus id

-- add a few recursive definitions
local function enable(dev_addr) -- enable the device
  i2cutils.write_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CONTROL),
    M.TSL2561_CONTROL_POWERON
  )
end
local function disable(dev_addr) -- disable the device
  i2cutils.write_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CONTROL),
    M.TSL2561_CONTROL_POWEROFF
  )
end
local function settimegain(dev_addr, time, gain) -- set the integration time and gain together
  enable(dev_addr)
  i2cutils.write_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_TIMING),
    bit.bor(time, gain)
  )
  disable(dev_addr)
end
local function getFullLuminosity(dev_addr) -- Do the actual reading from the sensor
  local ch0low = i2cutils.read_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CHAN0_LOW)
  )
  local ch0high = i2cutils.read_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CHAN0_HIGH)
  )
  ch0=string.byte(ch0low)+(string.byte(ch0high)*256)

  local ch1low = i2cutils.read_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CHAN1_LOW)
  )
  local ch1high = i2cutils.read_reg(
    dev_addr,
    bit.bor(M.TSL2561_COMMAND_BIT, M.TSL2561_REGISTER_CHAN1_HIGH)
  )
  ch1=string.byte(ch1low)+(string.byte(ch1high)*256)
  return ch0, ch1
end
function M.getchannels()
  local rc0,rc1 = nil,nil
  local dev_addr = M.TSL2561_ADDR_FLOAT 
  i2c.setup(busid, SDA_PIN , SCL_PIN, i2c.SLOW)

  -- 16 = fixed-point maths fudge factor

  -- 16 * (322/322), 16
  settimegain(dev_addr, M.TSL2561_INTEGRATIONTIME_402MS, M.TSL2561_GAIN_16X)

  enable(dev_addr)
  tmr.delay(500000)
  local ch0,ch1 = getFullLuminosity(dev_addr)
  disable(dev_addr)

  if ch0 < 65535 then
    rc0 = ch0
  end
  if ch1 < 65535 then
    rc1 = ch1
  end

  if ( rc0 == nil or rc1 == nil) then
    -- 16 * (322/81), 16
    -- nom,denom = 322,81
    settimegain(dev_addr, M.TSL2561_INTEGRATIONTIME_101MS, M.TSL2561_GAIN_16X)

    enable(dev_addr)
    tmr.delay(500000)
    ch0,ch1 = getFullLuminosity(dev_addr)
    disable(dev_addr)

    if rc0 == nil and ch0 < 37177 then
      rc0 = ch0*322/81
    end
    if rc1 == nil and ch1 < 37177 then
      rc1 = ch1*322/81
    end

    if ( rc0 == nil or rc1 == nil) then
      -- 16 * (322/322), 1
      -- nom,denom = 16,1
      settimegain(dev_addr, M.TSL2561_INTEGRATIONTIME_402MS, M.TSL2561_GAIN_1X)

      enable(dev_addr)
      tmr.delay(500000)
      ch0,ch1 = getFullLuminosity(dev_addr)
      disable(dev_addr)

      if rc0 == nil and ch0 < 65535 then
        rc0 = ch0*16
      end
      if rc1 == nil and ch1 < 65535 then
        rc1 = ch1*16
      end

      if ( rc0 == nil or rc1 == nil) then
        -- 16 * (322/81), 1
        -- nom,denom = 5152,81
        settimegain(dev_addr, M.TSL2561_INTEGRATIONTIME_101MS, M.TSL2561_GAIN_1X)

        enable(dev_addr)
        tmr.delay(500000)
        ch0,ch1 = getFullLuminosity(dev_addr)
        disable(dev_addr)

        if rc0 == nil then
          rc0 = ch0*5152/81
        end
        if rc1 == nil then
          rc1 = ch1*5152/81
        end
      end
    end
  end
  return rc0,rc1
end

return M
