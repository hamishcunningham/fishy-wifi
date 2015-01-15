lighton=0

pin = 10
pwm.setup(pin, 500, 512)
pwm.start(pin)
        
tmr.alarm(0, 1000, 1, function()
  if lighton == 0 then
    lighton = 1
    pwm.setduty(pin, 0, 512)
  else
    lighton = 0
    pwm.setduty(pin, 0, 0)
  end
end)