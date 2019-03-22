crnr=1
cr ={
{"ssid","pwd"},
{"ssid2","pwd2"}
}

SSID=cr[crnr][1]
PASSWORD=cr[crnr][2]
crnr=crnr+1
for k,v in ipairs(cr) do
	print(k,v[1])
end
