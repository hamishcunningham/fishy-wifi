-- This code provides some basic helper functions dealing with i2c devices
-- Much of this code was inspired by Santos and others on esp8266.com
-- This code is AGPL v3 by gareth@l0l.org.uk and Hamish Cunningham
-- blah blah blah standard licence conditions apply blah blah blah

i2cutils={}

--  read a single byte from register
function i2cutils.read_reg(dev_addr, reg_addr)
  id = 0
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

function i2cutils.write_reg(dev_addr, reg_addr, reg_val)
  id = 0
  i2c.start(id)
  i2c.address(id, dev_addr, i2c.TRANSMITTER)
  i2c.write(id, reg_addr)
  i2c.write(id, reg_val)
  i2c.stop(id)
  print("write command 0x"..string.format("%02X",reg_addr),"0x"..string.format("%02X",reg_val))
end

-- initialize i2c with our id and pins in slow mode :-)
function i2cutils.initialise(dev_addr, init_reg_addr, sda, scl)
  id = 0
  i2c.setup(id, sda, scl, i2c.SLOW)
end

return i2cutils
