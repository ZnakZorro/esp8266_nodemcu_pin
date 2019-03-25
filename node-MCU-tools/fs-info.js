var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');
con.connect('COM3', 115200, true,1)
	.then(function(retconn){
		console.log('retconn=',retconn)
		con.fsinfo()
			.then(function(ret){
				console.log('ret=',ret)
				con.disconnect()
			}).catch(function(e){console.log('e=',e);})
})

