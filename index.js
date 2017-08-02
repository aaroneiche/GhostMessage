// 'use strict';

var webport;
// var portDevice = '/dev/ttyUSB1';
var portDevice;

if(process.argv.length > 2){
  portDevice = process.argv[2];
}else{
  portDevice = '/dev/ttyS0';
}

webport = (process.argv[3] != undefined)? process.argv[3] : 8080;

console.log("Opening port on " + portDevice);
const SerialPort = require('serialport');
const port = new SerialPort(portDevice);
const express= require('express');
const WebSocket = require('ws');
const randomId = require('random-id');

var bodyParser = require('body-parser');
var app = express();

app.use(express.static('public'));
app.use(bodyParser.json());

var server = require('http').createServer(app);

server.listen(webport,function(){
  console.log("Web Server listening on port " + webport);
});

var wsServer = new WebSocket.Server({server});
var wsConnections = {};

port.on('open', () => {
  console.log('Connected to Serial Port.');
});

port.on('data', (data) => {
  console.log(data.toString());
});

app.post("/message", function(req,res,next){

  var m = req.body;
  console.log("Message from " + req.connection.remoteAddress + ":  " +m.message);
  port.write(m.message + ";" + m.color);
  res.send("Message Sent!");
  wsServer.broadcast(JSON.stringify(m));

})

wsServer.broadcast = function broadcast(data) {
  wsServer.clients.forEach(function each(client) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(data);
    }
  });
};


wsServer.on('connection',(ws,req)=>{
  console.log("User Connected: ", ws._socket.remoteAddress);
})

wsServer.on('close',function(ws,req){
  console.log("socket close");
  console.log(ws)
  connections.splice(connections.indexOf(ws));
})
