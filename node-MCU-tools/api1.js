var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');

con.connect('COM3', 115200, true,1)
	.then(function(retconn){
		console.log('retconn=',retconn)
		con.run("test1.lua")
			.then(function(ret){
				console.log(ret)
				//con.disconnect()
			})
			.then(function(ret){
				con.run("test.lua")
					.then(function(ret){
						console.log(ret)
						con.disconnect()
					})
			})
			
})

