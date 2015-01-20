-- sense.lua: read those sensors (it's life Jim, but not as we know it)
sense = {}
function sense.run(continuation)
  for i,v in continuation do print("sense... i: ", i, "; v: ", v) end
  -- TODO do sensor stuff
end
return sense

