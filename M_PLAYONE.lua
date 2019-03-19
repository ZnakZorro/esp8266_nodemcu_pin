local moduleName = ... 
local M = {}
_G[moduleName] = M

function M.Start(u8File)

   function cb_drained(d)
   --print("drained")
   file.seek("set", 0)
   file.close()
   file=nil
   drv:close()
   drv = nil
   d=nil
   collectgarbage()
   --print(node.heap())
   tmr.wdclr()
   end

   function cb_stopped(d)
      cb_drained(d)   
   end
   
   if (drv) then drv:stop() end
   
   file.open(u8File, "r")
   drv = pcm.new(pcm.SD, 5)
   drv:on("data", function(drv) return file.read(); end)
   drv:on("stopped", cb_stopped)  
   drv:on("drained", cb_drained)
   drv:play(pcm.RATE_8K)

end

return M