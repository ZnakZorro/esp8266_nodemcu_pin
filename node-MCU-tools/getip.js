var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');
con.connect('COM3', 115200, true,1)
	.then(function(retconn){
		console.log('retconn=',retconn)
		con.execute("print(wifi.sta.getip())")
			.then(function(ret){
				console.log('ret=',ret)
				console.log('ret=',ret.echo)
				console.log('ret=',ret.response)
				con.disconnect()
			}).catch(function(e){console.log('e=',e);})
})

