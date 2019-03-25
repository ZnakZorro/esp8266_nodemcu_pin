let progress=function(p){console.log(p+"%")}

var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');

con.connect('COM3', 115200, true,1)
	.then(function(retconn){
		console.log('retconn=',retconn)
		con.upload("aaa.lua","aaa.lua",{"minify":null},progress)
			.then(function(ret){
				console.log('upload=',ret)
			})
			.catch(function(err){console.log('err=',err);})
			.then(function(ret){
				con.run("aaa.lua")
					.then(function(ret){
						console.log('run=\n----------\n'+ret+'\n==========\n')
						con.run("test.lua").then(function(a){con.disconnect()}).catch(function(er){console.log('er=',er);})
						
					}).catch(function(e){console.log('e=',e);})
			})
			
})

