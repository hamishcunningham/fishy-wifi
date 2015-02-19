count = 0
repeat
  count = count + 1
  print(adc.read(0))
  tmr.delay(10000)
  tmr.wdclr()
until(count > 1000)