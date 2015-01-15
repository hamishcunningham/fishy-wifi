-- This code is GPL v3 by gareth@l0l.org.uk
-- blah blah blah standard licence conditions apply blah blah blah
-- Special hat-tip to lady ada - hacker hero - amongst many others
-- Reads value of TSL2561 I2C Luminosity sensor
-- As used on breakout board by Adafruit

id=0  -- need this to identify (software) IC2 bus?
io_pin= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5} -- this maps GPIO numbers to internal IO references
sda=io_pin[12] -- connect to pin GPIO13
scl=io_pin[14] -- connect to pin GPIO14
addr=0x39 -- the I2C address of our device
-- TSL2561_REGISTER_ID=0x0A -- part of the register table
TSL2561_REGISTER_ID=0x01 

function initialise(addr)
     -- initialize i2c with our id and pins in slow mode :-)
     i2c.setup(id,sda, scl, i2c.SLOW)
     tmr.delay(1000000)
     result=read_reg(addr, TSL2561_REGISTER_ID)
     x, y = tonumber(result)
     print("emsg: ", y)
     print("read_reg= ", tonumber(result))
     print("result: ", result)
     print("string.byte: ", string.byte(result))
     print("string.byte 2: ", string.byte(result, 2))
     print(string.format("str.fmt: |%s|", result, result))
     if bit.band(string.byte(result), 0x0A) == 0x0A then
          print("Found TSL2561")
     end
     print(
       string.format(
         "s.b: %u", bit.band(string.byte(result), 0x0A)
        ), " ", result
      )
end

-- user defined function: read from reg_addr content of dev_addr

function read_reg(dev_addr, reg_addr)
     i2c.start(id)
     tmr.delay(500000)
     print("id,dev_addr,TRANS ", id, dev_addr, i2c.TRANSMITTER)
     ack = i2c.address(id, dev_addr, i2c.TRANSMITTER)
     print("ack: ", ack)
     writeres = i2c.write(id, reg_addr)
     print("writeres: ", writeres)
     tmr.delay(500000)
     i2c.stop(id)
     tmr.delay(500000)
     i2c.start(id)
     tmr.delay(500000)
     ack = i2c.address(id, dev_addr, i2c.RECEIVER)
     print("ack: ", ack)
     c=i2c.read(id,1)
     print("c: ", c)
     print(string.format("c: %s", c))
     i2c.stop(id)
     return c
end

initialise(addr)
