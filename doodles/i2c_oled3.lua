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
oled_command(0x21) -- set column addresses
oled_command(0x00) -- reset column start address
oled_command(0x7F) -- reset column end address
oled_command(0x22) -- set page addresses
oled_command(0x00) -- reset page start address
oled_command(0x07) -- reset page end address
end

function init() -- Initialises the 128x64 oled
print("starting")
oled_command(0xAE) -- turn off oled panel
oled_command(0xA8) -- set multiplex ratio to 1 byte following
oled_command(0x3F) -- 1/64 duty (N+1 MUX: 1-64))
oled_command(0xD3) -- set display offset to 1 byte following
oled_command(0x00) -- no offset

oled_command(0x40) -- set start line address
oled_command(0xA0) -- set segment remap
oled_command(0xC0) -- set COM output scan direction
oled_command(0xDA) -- set com pins hardware configuration to 1 byte following
oled_command(0x12) -- set pins

oled_command(0x81) -- set contrast control register
oled_command(0x7f) -- reset contrast to default value

oled_command(0xA4) -- set display on
oled_command(0xA6) -- set normal display
oled_command(0xD5) -- set display clock divide ratio/oscillator frequency 
oled_command(0x80) -- set divide ratio 

oled_command(0x8D) -- set Charge Pump enable/disable 
oled_command(0x14) -- set(0x10) disable

oled_command(0xAF) -- turn on oled panel
print("Init done")
end

init()
for a=1,20 do
display()
tmr.delay(100000)
tmr.wdclr()
oled_command(0xA7)
display()

tmr.delay(100000)
tmr.wdclr()
oled_command(0xA6)
display()
end