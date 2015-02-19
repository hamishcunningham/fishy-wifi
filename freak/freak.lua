-- freak.lua: task management utility
freak = {}
local cfile = "freakdata.lua"
local minheap = 4000 -- if we've dropped below this after a task, restart
function freak.getconf() return pcall(dofile, cfile) or {} end
local function t2str(t)
  buf = "{ "
  for k, v in pairs(t) do
    if type(v) == "table" then v = t2str(v) else v = tostring(v) end
    buf = buf .. string.format(' %s="%s",', k, v)
  end
  return buf .. " }\n"
end
local function persist(t)
  file.open(cfile, "w"); file.write("return " .. t2str(t)); file.close()
end
function freak.continue(ctn) -- main "loop"
  print("freak.contin: ctn.taskdata.nexttask=", ctn.taskdata.nexttask) -- DEBUG
  if ctn.prevtask then -- reclaim the previous task's memory
    package.loaded[ctn.prevtask] = nil
    collectgarbage()
    print("done garbage collection; heap = ", node.heap()) -- DEBUG
  end
  if node.heap() < minheap then
    print("oops, heap is low... ", node.heap()) -- DEBUG
    -- TODO persist(ctn.taskdata)
    -- TODO node.restart() -- reset the chip and start over
  end
  currtask = ctn.taskdata.nexttask
  if(currtask > #ctn.tasks) then currtask = 1 end -- start over

  taskname = ctn.tasks[currtask]
  ctn.taskdata.nexttask = currtask + 1 -- increment task number

  if type(taskname) == "number" then
    print("freak: sleeping ", taskname * 1000, ", task#=", currtask) -- DEBUG
    ctn.prevtask = nil
    -- node.deepsleep(taskname * 1000) TODO awaiting access to gpio16
    -- TODO recursive call to continue
  else
    if not ctn.taskdata.taskname or not ctn.taskdata.taskname.skipme then
      print("freak: running task ", currtask, ": ", taskname) -- DEBUG
      ctn.prevtask = taskname
      taskchunk = require(taskname)
      pcall(taskchunk.run, ctn) -- run the task
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
