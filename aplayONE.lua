function cb_drained(d)
print("drained")
file.seek("set", 0)
file.close()
file=nil
drv:close()
drv = nil
d=nil
collectgarbage()
print(node.heap())
tmr.wdclr()
end


file.open("slowik1.u8", "r")
drv = pcm.new(pcm.SD, 5)
drv:on("data", function(drv) return file.read(); end)
drv:on("drained", cb_drained)

drv:play(pcm.RATE_8K)

