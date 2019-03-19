dofile("M_LOADMODULE.lc")

print("\n\n") for n in pairs(_G["__index"]) do print(n) end

print("--------\n*.lua")
mpliki = loadM('M_DIR','lua')
k=1
for i,plik in pairs(mpliki) do
	--print(plik['name'],plik['size'])
   	l = string.format("%-20s",plik['name']) 
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end
print("Heap="..node.heap())

print("--------\n*.lua")
mpliki = loadM('M_DIR','lc')
k=1
for i,plik in pairs(mpliki) do
	--print(plik['name'],plik['size'])
   	l = string.format("%-20s",plik['name']) 
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end
print("Heap="..node.heap())

print("--------\n*.lua")
mpliki = loadM('M_DIR','u8')
k=1
for i,plik in pairs(mpliki) do
	--print(plik['name'],plik['size'])
   	l = string.format("%-20s",plik['name']) 
	if k<10 then s=" "; else s=""; end
	print(s..k..". "..l.."   "..plik['size'].." bytes")
	k=k+1
end

print("Heap="..node.heap())