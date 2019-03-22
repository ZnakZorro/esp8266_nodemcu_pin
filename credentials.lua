crnr=2
cr ={
{"znakzorro_plus","niemieckiewino"},
{"webWI","wi77tam77"}
}

SSID=cr[crnr][1]
PASSWORD=cr[crnr][2]
crnr=crnr+1
for k,v in ipairs(cr) do
	print(k,v[1])
end
