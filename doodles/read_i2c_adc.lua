-- This code is GPL v3 by gareth@l0l.org.uk
-- blah blah blah standard licence conditions apply blah blah blah
-- Special hat-tip to sancho and ryan amongst many others
-- Reads value of MCP3221 I2C ADC
-- As used on minipH project by Sparky's Widgets

id=0  -- need this to identify (software) IC2 bus?
io_pin= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5} -- this maps GPIO numbers to internal IO references
sda=io_pin[12] -- connect to pin GPIO14
scl=io_pin[14] -- connect to pin GPIO12
addr=0x4C -- the I2C address of our device

-- initialize i2c with our id and pins in slow mode :-)
i2c.setup(id,sda,scl,i2c.SLOW)

-- user defined function: read from reg_addr content of dev_addr
function read_adc(dev_addr)
     i2c.start(id)
     i2c.address(id, dev_addr,i2c.RECEIVER)
     c=i2c.read(id,2)
     i2c.stop(id)
     return c
end

print("Reading ADC value")
for i=0,100 do
     result=read_adc(addr)
     print("Result: "..string.byte(result,1)*256+string.byte(result,2).."  - high:low bytes "..string.byte(result, 1).." : "..string.byte(result, 2))
     tmr.delay(250000)
     tmr.wdclr()
end
