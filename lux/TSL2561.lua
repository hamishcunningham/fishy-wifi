-- This code is GPL v3 by gareth@l0l.org.uk
-- blah blah blah standard licence conditions apply blah blah blah
-- Special hat-tip to lady ada - hacker hero - amongst many others
-- Reads value of TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit

dofile("TSL2561.h.lua") -- this is our static definitions
print("loaded header file")

id=0  -- need this to identify (software) I2C bus?
io_pin= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5} -- this maps GPIO numbers to internal IO references
sda=io_pin[14] -- connect to pin GPIO14
scl=io_pin[12] -- connect to pin GPIO12
addr=TSL2561_ADDR_FLOAT -- the I2C address of our device with ADDR pin floating

function read_reg(dev_addr, reg_addr) --  read a single byte from register
     i2c.start(id)
     i2c.address(id, dev_addr ,i2c.TRANSMITTER)
     i2c.write(id,reg_addr)
     i2c.stop(id)
     i2c.start(id)
     i2c.address(id, dev_addr,i2c.RECEIVER)
     c=i2c.read(id,1)
     i2c.stop(id)
     return c
end

function write_reg(dev_addr, reg_addr, reg_val)
     i2c.start(id)
     i2c.address(id, dev_addr, i2c.TRANSMITTER)
     i2c.write(id, reg_addr)
     i2c.write(id, reg_val)
     i2c.stop(id)
     return c
end

function initialise(addr) -- initialize i2c with our id and pins in slow mode :-)
     i2c.setup(id,sda,scl,i2c.SLOW)
     result=read_reg(addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_ID))
     print(string.byte(result))
     if string.byte(result) == 0x0A then
         print("Found TSL2561")
     end
end

function enable(eaddr) -- enable the device
     write_reg(addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_CONTROL), TSL2561_CONTROL_POWERON)
end

function disable(daddr) -- disable the device
     write_reg(addr,bit.bor(TSL2561_COMMAND_BIT,TSL2561_REGISTER_CONTROL), TSL2561_CONTROL_POWEROFF)
end

disable(addr)
print("disabled")

print(string.byte(read_reg(addr,TSL2561_REGISTER_ID)))

initialise(addr)
print(string.byte(read_reg(addr,TSL2561_REGISTER_ID)))
enable(addr)
print("enabled")
initialise(addr)
