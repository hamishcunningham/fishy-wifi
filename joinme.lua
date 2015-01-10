-- joinme.lua
-- do initial wifi config for NodeMCU over HTTP

nu = require nmutils

print("are we running on NodeMCU? ", nu.isnodemcu())
