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

function init() -- Initialises the 128x64 oled
print("starting")
oled_command(0xAE) -- turn off oled panel
oled_command(0x00) -- set low column address 
oled_command(0x10) -- set high column address 
oled_command(0x40) -- set start line address 
oled_command(0x81) -- set contrast control register

oled_command(0xA0) --
oled_command(0xC0) -- 
  
oled_command(0xA6) -- set normal display 
oled_command(0xA8) -- set multiplex ratio(1 to 64) 
oled_command(0x3F) -- 1/64 duty 
oled_command(0xD3) -- set display offset 
oled_command(0x00) -- not offset 
oled_command(0xD5) -- set display clock divide ratio/oscillator frequency 
oled_command(0x80) -- set divide ratio 
oled_command(0xD9) -- set pre-charge period 
oled_command(0xF1) -- 
oled_command(0xDA) -- set com pins hardware configuration 
oled_command(0x12) -- 
oled_command(0xDB) -- set vcomh 
oled_command(0x40) -- 
oled_command(0x8D) -- set Charge Pump enable/disable 
oled_command(0x14) -- set(0x10) disable
oled_command(0xAF) -- turn on oled panel
print("Init done")
end

display()
tmr.delay(10000)
tmr.wdclr()
oled_command(0xA7)
display()

tmr.delay(10000)
tmr.wdclr()
oled_command(0xA6)
display()
