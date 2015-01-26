-- freak.lua: task management utility
freak = {}
local cfile = "freakdata.lua"
local minheap = 5000 -- if we've dropped below this after a task, restart
function freak.getconf() return pcall(dofile, cfile) or {} end
local function t2str(t)
  buf = "{ "
  for k, v in pairs(t) do
    if type(v) == "table" then 
      v = t2str(v)
    else
      v = tostring(v)
    end
    buf = buf .. string.format(' %s="%s",', k, v)
  end
  return buf .. " }\n"
end
local function persist(t)
  file.open(cfile, "w"); file.write("return " .. t2str(t)); file.close()
end
function freak.continue(ctn) -- main "loop"
-- TODO delete previous task here; perhaps increment ctn.nexttask too
-- and keep currtask here instead
  if node.heap() < minheap then
print("oops, heap is low, persisting td but not rebooting... ", node.heap())
--    persist(ctn.taskdata)
    -- node.restart() -- reset the chip and start over
  end
  nexttask = ctn.taskdata.nexttask
  if(nexttask > #ctn.tasks) then -- start over
    nexttask = 1
    ctn.taskdata.nexttask = nexttask
  end
  taskname = ctn.tasks[nexttask]
  if type(taskname) == "number" then
    print("sleep: ", taskname * 1000)
    -- node.deepsleep(taskname * 1000) TODO awaiting access to gpio16
  else
    if not ctn.taskdata.taskname or not ctn.taskdata.taskname.skipme then
      print("freak: running task ", nexttask, ": ", taskname) -- DEBUG
      taskchunk = require(taskname)
      pcall(taskchunk.run, ctn) -- run the task
-- TODO problem here if the task doesn't return in the way joinme does!
      taskchunk = nil
      package.loaded[taskname] = nil
      collectgarbage()          -- reclaim the task's memory
    end
  end
end
function freak.begin(tasks)
  ctn = { tasks=tasks, freak=freak, }
  _, results = freak.getconf()
  ctn.taskdata = results or { }
  ctn.taskdata.nexttask = ctn.taskdata.nexttask or 1
  return freak.continue(ctn)
end
function freak.reset() file.remove(cfile); node.restart() end
return freak
