-- returns voltage reading when used with a 100k/470k resistor divider
-- for vcc - maximum is about 4.9v - tuned for 3AA cells
-- This code is AGPL v3 by gareth@l0l.org.uk and Hamish Cunningham
-- blah blah blah standard licence conditions apply blah blah blah

bl = {}

bl.getvolts = function()
     local samples=100
     local result=0

     for i=1,samples do
          result=result+adc.read(0)
     end
     voltage=string.format("%.2f",result/samples*0.0055)
     return voltage
end

return bl
