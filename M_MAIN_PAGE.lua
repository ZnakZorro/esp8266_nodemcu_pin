local moduleName = ... 
local M = {}
_G[moduleName] = M
--module

function M.Start()
print("\n---start wifi---")
dofile("credentials.lua")
zegar = tmr.time()
wifi.setmode(wifi.STATION)

wifi.sta.config({ ssid = SSID, pwd = PASSWORD })
wifi.sta.connect()
wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, function(T)
	print("Connection to AP("..T.SSID..") established!")
	loadM("M_PLAYONE","citac.u8")
	PASSWORD=nil
	SSID=nil
	cr=nil
	crnr=nil
	collectgarbage()
end)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function(T)
	print("Station IP: "..T.IP,"Time WiFi [s]=",tmr.time()-zegar)
	zegar=nil
	loadM("M_PLAYONE","j23.u8")
	print(node.heap())
end)
wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, function()
	loadM("M_PLAYONE","ludzie.u8")
	wifi.ap.config({ssid="ESP8266-"..node.chipid(), auth=wifi.OPEN})	
	enduser_setup.manual(true)
	enduser_setup.start(
		function()
			print("Connected to wifi as:" .. wifi.sta.getip())
			end,
			function(err, str)
			print("enduser_setup: Err #" .. err .. ": " .. str)
		end
	);
end)


pinOUT=4
lastHEAP = node.heap();
gpio.mode(pinOUT, gpio.OUTPUT)
srv = net.createServer(net.TCP)
srv:listen(80, function(conn)
	conn:on("receive", function(client, request)
	--print(request)
	local script="<script>\nfunction $(y){return document.getElementById(y)};\nfunction akcja(r){\nconsole.log(r);\n$('i').innerHTML=r.h;\nif(r.s==1)document.body.className='j'; else document.body.className='';}\nfunction fajax(n){document.body.className='r';var t=parseInt(((new Date()).getTime())/1000);fetch('/?a=x&pin='+n+'&t='+t).then(function(t){return t.json()}).then(function(j){akcja(j);}).catch(function(e){print('ERROR')})}\n</script>\n";
	local style = "\n<style>#c{text-align:center;} body,select,button{margin:5vw;font:normal 18px verdana;}button {width:34vw;max-width:10em; padding:0.5em 0.2em;} button:active,button:focus{color:red;} body{background:#000; color:#888;}.j{background:#fff;}.r{background:#f00;}</style>"
	local body  = "\n<button id='b1' onClick='fajax(\"ON\")'>On 1</button><button id='b2' onClick='fajax(\"OFF\")'>Off 1</button>"
	local _icon  = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACAAgMAAAC+UIlYAAAACVBMVEUAAAAAAAAAAACDY+nAAAAAAnRSTlMAgJsrThgAAADpSURBVFjD7dHBCQMxDERR70VFpJrUo2pURC5SlcEszkCEfwPruRn9wwOPv13vwbMYvNdn8Dz5flUxwqqCCVWM8KpkQjHCZhBMYITPIJHACLuDIAIj/A4SCIywFQQQEOErSCAQwhQEEADhChIIQgBBiEYQAghCNAIgRBACCEI0ghBAEAIIQjSCEEgQQgRGWA+iERDhPUggCAEEIUQAhAgNIQIgRGgIEQAhQkeIwAjbBSECI3wXpAiIsH0QIhDC90GKAAijIBaBEE5BLgIgjIOYBESIQN+xu87bCZ4brPcJTnCCE5zgfp/99gV7t1BMVMeE7wAAAABJRU5ErkJggg=="
	local icon  = "\n<link rel='icon' type='image/png' size='128x128' href='".._icon.."'>"
	local buf = ""
	local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP")
	if (method == nil) then
		_, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP")
	end
	local _GET = {}
	if (vars ~= nil) then
		for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
			_GET[k] = v
		end
	end
	--for n,v in pairs(_GET) do print (n,v);end
	buf = buf .. "<!doctype html><html><head><meta charset='utf-8'><title>myOnOff</title><meta name='viewport' content='width=device-width,initial-scale=1'><meta name='mobile-web-app-capable' content='yes'>\n"..icon..style.."\n"..script.."</head><body><div id='c'><h3>myOnOff - esp8266</h3>"..body.."</div></body></html>"

	local s=0
	if (_GET.pin == "ON") then
		gpio.write(pinOUT, gpio.HIGH)
		loadM("M_PLAYONE","slowik1.u8")
		s=1
	elseif (_GET.pin == "OFF") then
		gpio.write(pinOUT, gpio.LOW)
		loadM("M_PLAYONE","jump_8k.u8")
	end
	buf = buf .. "<br><div id='i'>"..node.heap().."</div>\n</body></html>"
	if (_GET.a == "x") then buf='{"h":"'..node.heap()..', '..lastHEAP..'","s":"'..s..'","p":"'..pinOUT..'"}' end
		if (_GET.t) then
			-- czas w sekundach
			local czas =_GET.t / 60 
			-- czas w minutach
			print('czas=',czas)
			local minuta  = czas % 60
			local godzina = (czas/60) % 24
			print('Czas=',czas,'h='.godzina,'m='.minuta,'tmr=',tmr.time(),'D=',czas-tmr.time())
		end
	client:send(buf)
	buf=nil
	icon=nil
	_icon=nil
	script=nil
	style=nil
	body=nil
	collectgarbage()
	lastHEAP = node.heap();
	end)
	conn:on("sent", function(c) 
	  c:close() 
	end)
end)

--module
end
return M