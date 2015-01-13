id=0  -- need this to identify (software) IC2 bus?
gpio= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5} -- this maps GPIO numbers to internal IO references
sda=gpio[12] -- connect to pin GPIO14
scl=gpio[14] -- connect to pin GPIO12
addr=0x3C -- the I2C address of our device
ret=0

-- initialize i2c
i2c.setup(id,sda,scl,i2c.SLOW)

-- user defined function: read from reg_addr content of dev_addr
function read_reg(dev_addr, reg_addr)
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

function oled_command(cmd)
     write_reg(addr,0,cmd)
end

function display()
oled_command(0x21)
oled_command(0x00)
oled_command(0x7F)
oled_command(0x22)
oled_command(0x00)
oled_command(0x07)
end

print("starting")
oled_command(0xAE)
oled_command(0xD5)
oled_command(0x80)
oled_command(0xA8)
oled_command(0x3F)
oled_command(0xD3)
oled_command(0x00)
oled_command(0x40)
oled_command(0x8D)
oled_command(0x14)
oled_command(0x20)
oled_command(0x00)
oled_command(0xA1)
oled_command(0xC8)
oled_command(0xDA)
oled_command(0x12)
oled_command(0x81)
oled_command(0xCF)
oled_command(0xD9)
oled_command(0xF1)
oled_command(0xDB)
oled_command(0x40)
oled_command(0xA4)
oled_command(0xA6)
oled_command(0xAF)
print("Init done")

display()
tmr.delay(10000)
tmr.wdclr()
oled_command(0xA7)
display()

tmr.delay(10000)
tmr.wdclr()
oled_command(0xA6)
display()
