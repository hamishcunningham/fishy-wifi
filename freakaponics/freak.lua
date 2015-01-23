-- freak.lua: task management utility
freak = {}
local cfile = "freakdata.lua"
local minheap = 15000 -- if we've dropped below this after a task, restart
local function getconf() return pcall(dofile, cfile) or {} end
local function t2str(t)
  buf = "{ "
  for k, v in pairs(t) do
    if type(v) == "table" then v = t2str(v) end
    buf = buf .. string.format(' %s="%s",', k, v)
  end
  return buf .. " }\n"
end
local function persist(t)
  file.open(cfile, "w"); file.write("return "..t2str(t)); file.close()
end
--[[
TODO call "run" continue
put freak.continue into the continuation so phases can call back to it
shift nexttask param into continuation
call continuation "c"
]]
local function run(continuation, nexttask) -- main "loop"
  taskname = continuation.tasks[nexttask]
  if type(taskname) == "number" then _=0 -- node.deepsleep(taskname * 1000)
                                         -- TODO awaiting access to gpio16
  else
    preconfunc = continuation.precons[taskname] -- check preconditions
    if not preconfunc or pcall(preconfunc, continuation) then
      print("freak: running task ", nexttask, ": ", taskname) -- DEBUG
      taskchunk = require(taskname)
      pcall(taskchunk.run, continuation)
    end
  end
  nexttask = nexttask + 1
  if(nexttask > #continuation.tasks) then nexttask = 1 end -- start over
  continuation.taskdata.nexttask = nexttask
-- TODO
taskchunk = nil
package.loaded[taskname] = nil
collectgarbage()
  if node.heap() < minheap then
    persist(continuation.taskdata)
    node.restart() -- reset the chip and start over
  end
-- TODO DEBUG  return run(continuation, nexttask) -- run the next task (tail call)
end
function freak.begin(tasks, precons)
  continuation = { tasks=tasks, precons=precons }
  _, results = getconf()
  continuation.taskdata = results or { }
  return run(continuation, continuation.taskdata.nexttask or 1)
end
return freak
