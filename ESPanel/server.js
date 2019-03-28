var moduleDIR='../../node_modules/';
var express = require(moduleDIR+'express');
var bodyParser = require(moduleDIR+'body-parser')
var app = express();
var http = require('http').Server(app);
var io = require(moduleDIR+'socket.io')(http);
var socketReady=false;

app.use(express.static(__dirname+'/public/'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}))

var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');

var portUSB 	= 'COM3';
var badeRate 	= 115200;

/*qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq*/
/*
    onError: _virtualTerminal.onError,
    disconnect: _virtualTerminal.disconnect,
    connect: _connect,
    isConnected: _virtualTerminal.isConnected,
    checkConnection: _checkConnection,
    compile: _compile,
    deviceInfo: _deviceInfo,
    listDevices: _devices,
    download: _download,
    upload: _upload,
    execute: _execute,
    format: _format,
    fsinfo: _fsinfo,
    remove: _remove,
    softreset: _reset.softreset,
    hardreset: _reset.hardreset,
    run: _run
	*/
	
let progress=function(p){
	console.log(p+"%")
	IOslij('info',p)
}
	

function serialAction(action){
	console.log('serialAction=',action)
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			if (action=='isConnected') 		con.isConnected().then(function(ret){IOslij('info',ret);}).catch(function(e){console.log('e=',e);})
			if (action=='checkConnection')	con.checkConnection().then(function(ret){IOslij('info',ret);}).catch(function(e){console.log('e=',e);})
			if (action=='connect') 			con.connect(portUSB, badeRate, true,1).then(function(ret){IOslij('info',ret);}).catch(function(e){console.log('e=',e);})
			if (action=='disconnect') 		con.disconnect().then(function(ret){IOslij('info',ret);}).catch(function(e){console.log('e=',e);})
	})
}


function executeLua(code){

	console.log('\n================')
	console.log('executeLua=')
	console.log('..................................')
	console.log(code)
	console.log('..................................')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			//let arr = code.split("\n");
				con.execute(code)
					.then(function(ret){
						console.log(ret)
						IOslij('info',ret)
						con.disconnect()
					}).catch(function(e){console.log('e=',e);})
		})
	
}
function runLua(file){
	console.log('\n================')
	console.log('runLua=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			con.run(file)
				.then(function(ret){
					console.log(ret)
					IOslij('info',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
	
}
	
function resetESP(val){
	console.log('\n================')
	console.log('resetESP=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			con.hardreset()
				.then(function(ret){
					console.log(ret)
					IOslij('info',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
	
}
function fsinfo(val){
	console.log('\n================')
	console.log('fsinfo=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			con.fsinfo()
				.then(function(ret){
					if(ret) IOslij('filelist',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
	
}
function checkSerial(){
	console.log('\n================')
	console.log('checkSerial=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			con.deviceInfo()
				.then(function(ret){
					console.log(ret)
					IOslij('info',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
}

function uploadESP(file){
	console.log('checkSerial=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			IOslij('info',retconn)
			con.upload(file,file,{"minify":null},progress)
				.then(function(ret){
					console.log('upload ret=',ret)
					IOslij('info',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
}

function downloadESP(file){
	console.log('downloadESP=')
	con.connect(portUSB, badeRate, true,1)
		.then(function(retconn){
			console.log('retconn downloadESP=',retconn)
			IOslij('info',retconn)
			con.download(file)
				.then(function(ret){
					con.disconnect()
					console.log('download ret=',ret)
					IOslij('filedownload',ret.toString())
					
				}).catch(function(e){console.log('e=',e);})
	})
}


/*qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq*/


app.get('/messages', (req, res) => {
  try{
    var message = (req.body);
	console.log(1,message)
     io.emit('message', req.body);
     res.sendStatus(200);
  }
  catch (error){res.sendStatus(500);return console.log('error',error);}
  finally{console.log('Message Posted.............')}
  
})

/*
app.get('/messages/:user', (req, res) => {
  var user = req.params.user
  try{
    var message = (req.body);
	console.log(2,message)
     io.emit('message', req.body);
     res.sendStatus(200);
  }
  catch (error){res.sendStatus(500);return console.log('error',error);}
  finally{console.log('Message Posted...')}
})
*/

function test(mess,value){
	console.log('TEST===',mess,value)
	
}

app.post('/messages', async (req, res) => {
  try{
    let message = (req.body);
	console.log('mmm=',message)
	let name  = message.name || null;
	let mess  = message.message || null;
	let value = message.value || null;	
	console.log('name=',name)
	console.log('mess=',mess)
	console.log('value=',value)
	if (name==='test'){test(mess,value);}
	if (name==='serial'){
			serialAction(mess);
		//if (mess==='checkConnection') checkSerial();
		//if (mess==='connect') checkSerial();
		//if (mess==='disconnect') checkSerial(); 
	}
	if (name==='esp'){
		if (mess==='reset') checkSerial('reset');
		if (mess==='fsinfo') fsinfo();
		if (mess==='upload') uploadESP(value);
		if (mess==='download') downloadESP(value);
	}
	if (name==='lua'){	
		if (mess==='run') runLua(value);
		if (mess==='execute') executeLua(value);
	}
	
	console.log(name,mess,value)
     io.emit('message', req.body);
     //io.emit('info', req.body.name);
     res.sendStatus(200);
  }
  catch (error){res.sendStatus(500);return console.log('error',error);}
  finally{console.log('Message Posted.')}
})



//io.on('connection', () =>{
  //console.log('a user is connected')
//})
io.on('connection', function(socket){
	socketReady=true;
	//console.log('socketReady=',socketReady)
	socket.on('message', function(msg){
		console.log("socket.on message=",msg)
		//io.emit('message', msg);
		IOslij('message',msg);
	});
});

function IOslij(topic,msg){
	console.log('\n...............IOslij...')
	console.log(msg)
	if (socketReady)io.emit(topic, msg);
	console.log('..................')
}

var i=0;
IOslij('info',(new Date()).toLocaleString());

setInterval(function(){
	//IOslij('info',(new Date()).toLocaleString());
},10000)




var server = http.listen(3009, () => {
  console.log('server is running on port', server.address().port);
});
