-- sense.lua: read those sensors (it's life Jim, but not as we know it)
sense = {}
function sense.run(ctn)
  print("sense.run...")
  -- TODO do sensor stuff

  freak.continue(ctn) -- give control back to the control freak
end
return sense

