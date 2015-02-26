-- sense.lua -- sensor work
sense={}
function sense.run()
  print("sense.run...")             -- DEBUG
  w.store("sense", 345345)
  return nil
end
return sense
