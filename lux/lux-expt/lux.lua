--dofile("TSL2561noh.lua") 

function getFullLuminosity(dev_addr) -- Do the actual reading from the sensor
     tmr.delay(14000)
     ch0low = read_reg(dev_addr, bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN0_LOW))
     ch0high = read_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN0_HIGH))
     ch0=string.byte(ch0low)+(string.byte(ch0high)*256)

     ch1low = read_reg(dev_addr, bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN1_LOW))
     ch1high = read_reg(dev_addr,bit.bor(TSL2561_COMMAND_BIT, TSL2561_REGISTER_CHAN1_HIGH))
     ch1=string.byte(ch1low)+(string.byte(ch1high)*256)
     return ch0,ch1
end

function calclux (ch0,ch1)
     chScale = TSL2561_LUX_CHSCALE_TINT0 -- integration time 13ms
     channel0 = bit.rshift(ch0 * chScale, TSL2561_LUX_CHSCALE)
     channel1 = bit.rshift(ch1 * chScale, TSL2561_LUX_CHSCALE)

-- find the ratio of the channel values (Channel1/Channel0)
  if channel0 ~=0 then ratio1 = bit.lshift(channel1, TSL2561_LUX_RATIOSCALE+1)/channel0 end

  ratio = bit.rshift(ratio1 + 1, 1) -- round the ratio value

if (ratio >= 0  and ratio <= TSL2561_LUX_K1T) then
     b=TSL2561_LUX_B1T
     m=TSL2561_LUX_M1T
elseif ratio <= TSL2561_LUX_K2T then
    b=TSL2561_LUX_B2T
     m=TSL2561_LUX_M2T
elseif ratio <= TSL2561_LUX_K3T then
     b=TSL2561_LUX_B3T
     m=TSL2561_LUX_M3T
elseif ratio <= TSL2561_LUX_K4T then
     b=TSL2561_LUX_B4T
     m=TSL2561_LUX_M4T
elseif ratio <= TSL2561_LUX_K5T then
     b=TSL2561_LUX_B5T
     m=TSL2561_LUX_M5T
elseif ratio <= TSL2561_LUX_K6T then
     b=TSL2561_LUX_B6T
     m=TSL2561_LUX_M6T
elseif ratio <= TSL2561_LUX_K7T then
     b=TSL2561_LUX_B7T
     m=TSL2561_LUX_M7T
elseif ratio > TSL2561_LUX_K8T then
    b=TSL2561_LUX_B8T
     m=TSL2561_LUX_M8T
end

  temp = ((channel0 * b) - (channel1 * m))

  if temp < 0 then temp = 0 end --do not allow negative lux value

  temp = temp + bit.lshift(1,(TSL2561_LUX_LUXSCALE-1)) -- round lsb (2^(LUX_SCALE-1))

  lux = bit.rshift(temp,TSL2561_LUX_LUXSCALE) -- strip off fractional portion

  return lux
end

print("Starting...")
initialise(addr)
enable(addr)
settimegain(addr, TSL2561_INTEGRATIONTIME_13MS, TSL2561_GAIN_0X)
print("Set integration time and gain")
chan0,chan1=getFullLuminosity(addr)
luxread=calclux(chan0,chan1)
print("Lux :"..luxread)