dofile("M_LOADMODULE.lc")

print("\n--------\n*.lua")
local mpliki = loadM('M_DIR','lua')
local k=1
for i,plik in pairs(mpliki) do
	local l = string.format("%-20s",plik['name'])
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end
print("Heap="..node.heap())

print("\n--------\n*.lc`")
local mpliki = loadM('M_DIR','lc')
local k=1
for i,plik in pairs(mpliki) do
	local l = string.format("%-20s",plik['name'])
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end
print("Heap="..node.heap())

print("\n--------\n*.u8")
local mpliki = loadM('M_DIR','u8')
local k=1
for i,plik in pairs(mpliki) do
	local l = string.format("%-20s",plik['name'])
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end

s=nil
dat=nil
collectgarbage()
print("Heap="..node.heap())

--print("\n\n") for n in pairs(_G["__index"]) do print(n) end
--print("\n\n");for n,v in pairs(_G) do print (n,v); end
