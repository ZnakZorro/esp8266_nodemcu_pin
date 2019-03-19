collectgarbage()

node.egc.setmode(node.egc.ALWAYS, 4096)  -- This is the default setting at startup. 
node.egc.setmode(node.egc.ON_ALLOC_FAILURE) -- This is the fastest activeEGC mode. 
node.egc.setmode(node.egc.ON_MEM_LIMIT, 30720)  -- Only allow the Lua runtime to allocate at most 30k, collect garbage if limit is about to be hit 
node.egc.setmode(node.egc.ON_MEM_LIMIT, -6144)  -- Try to keep at least 6k heap available for non-Lua use (e.g. network buffers)



-------------------

for i = node.task.LOW_PRIORITY, node.task.HIGH_PRIORITY do 
  node.task.post(i,function(p2)
    print("priority is "..p2)
  end) 
end     


-------------------
local r= node.random(255)
----------------
total_allocated, estimated_used = node.egc.meminfo()
print(total_allocated, estimated_used)

----------------------
lines = {
      luaH_set = 10,
      luaH_get = 24,
      luaH_present = 48,
    }
    a = {}
    for n in pairs(lines) do table.insert(a, n) end
    table.sort(a)
    for i,n in ipairs(a) do print(n) end
	
------------------------


for k,v in pairs(file.list()) do 
	l = string.format("%-15s",k) 
	print(l.."   "..v.." bytes") 
end

---------------------------
for n,v in pairs(_G) do print (n,v);end

print("\n\n");for n,v in pairs(_G) do print (n,v); end

print("\n\n") for n in pairs(_G) do print(n) end

print("\n\n") for n in pairs(_G["package"]) do print(n) end

print("\n\n") for n in pairs(_G["__index"]) do print(n) end


