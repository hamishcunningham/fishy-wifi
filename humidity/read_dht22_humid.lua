dht22 = require("dht22")

-- this maps GPIO numbers to internal IO references
io_pin = {[0]=3,[2]=4,[4]=2,[5]=1,[12]=6,[13]=7,[14]=5}
PIN = io_pin[13]
dht22.read(PIN)
t = dht22.getTemperature()
h = dht22.getHumidity()

if h == nil then
  print("Error reading from DHT22")
else
  print("Temperature: "..(t / 10).." deg C") 
  print("Humidity: "..(h / 10).."% RH")
end

-- release module
dht22 = nil
package.loaded["dht22"]=nil
