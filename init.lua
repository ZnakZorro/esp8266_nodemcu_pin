dofile("M_LOADMODULE.lua")
loadM("M_PLAYONE","z_roza.u8")
print('\ninit wait---2s---');
tmr.alarm(0,2500,0,function()
	mainPage = require('M_MAIN_PAGE').Start()
	--mainPage = loadM('M_MAIN_PAGE')
	--dofile("webap_toggle_pin.lua");
	--dofile("z-wifi-test.lua");
end);
