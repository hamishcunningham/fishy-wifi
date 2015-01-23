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
  file.open(cfile, "w"); file.write("return " .. t2str(t)); file.close()
end
local function continue(ctn) -- main "loop"
  nexttask = ctn.taskdata.nexttask
  taskname = ctn.tasks[nexttask]
  print("freak: running task ", nexttask, ": ", taskname) -- DEBUG
  if type(taskname) == "number" then _=0 -- node.deepsleep(taskname * 1000)
                                         -- TODO awaiting access to gpio16
  else
    if not ctn.taskname.skipme then
      taskchunk = require(taskname)
      pcall(taskchunk.run, ctn) -- run the task
      taskchunk = nil
      package.loaded[taskname] = nil
      collectgarbage()          -- reclaim the task's memory
    end
  end
  nexttask = nexttask + 1
  if(nexttask > #ctn.tasks) then nexttask = 1 end -- start over
  ctn.taskdata.nexttask = nexttask
  if node.heap() < minheap then
    persist(ctn.taskdata)
    node.restart() -- reset the chip and start over
  end
  -- TODO return continue(ctn) -- run the next task (tail call)
  print("not calling continue(ctn...)")
end
function freak.begin(tasks)
  ctn = { tasks=tasks, freak=freak, }
  _, results = getconf()
  ctn.taskdata = results or { }
  return continue(ctn, ctn.taskdata.nexttask or 1)
end
function freak.reset() file.remove(cfile); node.restart() end
return freak
