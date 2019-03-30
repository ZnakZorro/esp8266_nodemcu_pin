var moduleDIR='../node_modules/';
var express = require(moduleDIR+'express');
var bodyParser = require(moduleDIR+'body-parser')
var app = express();
var http = require('http').Server(app);
var io = require(moduleDIR+'socket.io')(http);
var socketReady=false;

			var OPTIONS = require('./public/js/options');
			var FUNCTIONS = require('./public/js/functions');
			/*
			console.log(OPTIONS)
			console.log(OPTIONS.portUSB)
			console.log(OPTIONS.baudRate)
			
			console.log(FUNCTIONS)
			console.log(FUNCTIONS.Calc.add)
			var ad=FUNCTIONS.Calc.add(2,4)
			console.log(ad)
			*/
			var portUSB 	= OPTIONS.portUSB;
			var baudRate 	= OPTIONS.baudRate;
			var isCONN 		= false

app.use(express.static(__dirname+'/public/'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}))

//var con = require('./NodeMCU-Tool/lib/nodemcu-connector.js');
var con = require(moduleDIR+'nodemcu-tool/lib/nodemcu-connector.js');

con.onError(function(err){console.log('con.onError=',err);})


	
let progress=function(p){
	console.log(p+"%")
	IOslij('info',p)
}
	
function resetESP(){
	console.log('\n================')
	console.log('#28 resetESP=')
	con.hardreset()
		.then(function(ret){
			console.log(ret)
			IOslij('info',ret)
		}).catch(function(e){console.log('e=',e);})	
}
	
function serialAction(action){
	console.log('serialAction=',action)

	if (action=='isConnected') 		{let v=con.isConnected(); 		IOslij('info',v);	console.log(v);}
	if (action=='checkConnection')	{con.checkConnection().then(function(v){IOslij('info',v);console.log(v);})}
	
	if (action=='connect') 			{con.connect(portUSB, baudRate, true,1).then(function(v){IOslij('startPage',true);console.log(v);})}
	if (action=='disconnect') 		{con.disconnect().then(function(v){IOslij('startPage',v);console.log(v);})}

	if (action=='deviceInfo') 		{con.deviceInfo().then(function(v){IOslij('info',v);console.log(v);})}
	if (action=='listDevices') 		{con.listDevices().then(function(v){IOslij('info',v);console.log(v);})}

}

function executeLua(code){
	console.log('\n================')
	console.log('#52 executeLua=')
	console.log('..................................')
	console.log(code)
	console.log('..................................')
	con.execute(code)
		.then(function(ret){
			console.log(ret)
			IOslij('info',ret)
		}).catch(function(e){console.log('e=',e);})
	
}
function runLua(file){
	console.log('\n================')
	console.log('#65 runLua=')
	con.run(file)
		.then(function(ret){
			console.log(ret)
			IOslij('info',ret)
		}).catch(function(e){console.log('e=',e);})	
}
	

function fsinfo(val){
	console.log('\n================')
	console.log('#76 fsinfo=')
	con.fsinfo()
		.then(function(ret){
			if(ret) IOslij('filelist',ret)			
		}).catch(function(e){console.log('e=',e);})	
}

function uploadESP(file){
	console.log('#84 uploadESP=')
	con.upload(file,file,{"minify":null},progress)
		.then(function(ret){
			console.log('upload ret=',ret)
			IOslij('info',ret)
		}).catch(function(e){console.log('e=',e);})
}

function downloadESP(file){
	console.log('\n================')
	console.log('#94 downloadESP=')
	con.download(file)
		.then(function(ret){
			console.log('download ret=',ret)
			IOslij('filedownload',ret.toString())
			
		}).catch(function(e){console.log('e=',e);})
}

function test(mess,value){
	console.log('TEST===',mess,value)
	
}

function startPAGE(){
	isCONN =con.isConnected();
	if (!isCONN){
		serialAction('connect');
		isCONN =con.isConnected();
		IOslij('startPage',isCONN)
	} else IOslij('startPage',isCONN)

	
}

/*qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq*/

app.use((req, res, next) => {
    console.log('#123 next newTime: ', Date.now(),(new Date()).toLocaleString());
    next();
});

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
		if (mess==='reset') resetESP();
		if (mess==='fsinfo') fsinfo();
		if (mess==='upload') uploadESP(value);
		if (mess==='download') downloadESP(value);
	}
	if (name==='lua'){	
		if (mess==='run') runLua(value);
		if (mess==='execute') executeLua(value);
	}
	if (name==='startPAGE'){startPAGE();}	
	
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
