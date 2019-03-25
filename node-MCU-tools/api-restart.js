var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');
con.connect('COM3', 115200, true,1)
	.then(function(retconn){
		console.log('retconn=',retconn)
		con.hardreset()
			.then(function(ret){
				con.disconnect()
			}).catch(function(e){console.log('e=',e);})
})

