-- wifiform.lua
-- a template form for choosing wifi AP and sending credentials

return [=[
<p>Choose a wifi access point to join:
<form action="chooseap">
  <ol>
$(T)
# for _, ssid in ipairs(T) do
  <li>SSID: <input type="radio" name="ssid" value="$(ssid)">$(ssid)<br/>
# end
  </ol>
  <br/>
  <input type="submit" value="Submit">
</form></p>
]=]
