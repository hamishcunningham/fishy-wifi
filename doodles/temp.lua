-- read temperature with DS18B20
t=require("ds18b20")
gpio= {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5} -- this maps GPIO numbers to internal IO references
t.setup(gpio[14])
addrs=t.addrs()
-- Just read as centigrade
print(t.read(nil,t.C))
-- Don't forget to release it after use
t = nil
ds18b20 = nil
package.loaded["ds18b20"]=nil