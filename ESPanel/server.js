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
function resetESP(val){
	console.log('checkSerial=')
	con.connect('COM3', 115200, true,1)
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
	console.log('checkSerial=')
	con.connect('COM3', 115200, true,1)
		.then(function(retconn){
			console.log('retconn=',retconn)
			con.fsinfo()
				.then(function(ret){
					console.log(ret)
					IOslij('info',ret)
					con.disconnect()
				}).catch(function(e){console.log('e=',e);})
	})
	
}
function checkSerial(){
	console.log('checkSerial=')
	con.connect('COM3', 115200, true,1)
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

app.post('/messages', async (req, res) => {
  try{
    let message = (req.body);
	console.log('mmm=',message)
	let name  = message.name || null;
	let mess  = message.message || null;
	let value = message.value || null;	
	if (name==='serial'){
		if (mess==='check') checkSerial();
	}
	if (name==='esp'){
		if (mess==='reset') checkSerial('reset');
		if (mess==='fsinfo') fsinfo();
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
	console.log(msg)
	if (socketReady)io.emit(topic, msg);
}

var i=0;
IOslij('info',(new Date()).toLocaleString());

setInterval(function(){
	//IOslij('info',(new Date()).toLocaleString());
},10000)




var server = http.listen(3009, () => {
  console.log('server is running on port', server.address().port);
});
