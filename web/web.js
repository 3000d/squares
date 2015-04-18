/**
 * Created by gberger on 15/04/15.
 */
var express= require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io').listen(http);
var util = require('util');
var serialport = require('serialport');
var SerialPort = serialport.SerialPort;
var portName = process.argv[2];

var connected=false;
var debug = false;

console.log(process.argv[2]);

if(!portName || portName == "dev") {
  util.log('No serial port given, DEBUG MODE');
  debug = true;
}


var myPort;

if(!debug) {
  myPort = new SerialPort(portName, {
    baudRate: 9600,
    parser: serialport.parsers.readline("\r\n")
  });


  myPort.on('open', function(){
    util.log('port open');

  });
  myPort.on('close', function() {
    console.log('port closed');

  });
  myPort.on('data', function(data){
    console.log('serial: ', data);
    var args = data.split(' ');
    var cmd = args.shift();
    console.log(cmd, args);

    if(cmd === "calib") {
      io.emit("getcalib", {
        square: args[0],
        min: args[1],
        max: args[2]
      });
    }
  });

// called when there's an error with the serial port:
  myPort.on('error', function(error) {
    console.log('there was an error with the serial port: ' + error);
    myPort.close();

  });
}

var Web = function() {
  app.use(express.static(__dirname + '/public_html/assets'));

  app.get('/', function (req, res) {
    res.sendFile(__dirname + '/public_html/index.html');
  });
  app.get('/calib', function (req, res) {
    res.sendFile(__dirname + '/public_html/calib.html');
  });

  io.sockets.on('connection', function(socket){
    socket.on('disconnect', function(){
      util.log('disconnected');
    });
    socket.on('setmove', function(data){
      writeToSerial('move ' + data.id + ' ' + data.value);
    });

    socket.on('setcalib', function(data) {
      writeToSerial('calib ' + data.square + ' ' + data.min + ' ' + data.max);
    });
  });


  var writeToSerial = function(data) {
    if(!debug) {
      myPort.write(data);
    } else {
      console.log('write : ', data);
    }
  };


  this.startServer = function () {
    http.listen(3000, function () {
      util.log('-- [Web] listening on localhost:3000');
      // util.log(myPort.comName);
    });
  }
};

module.exports = Web;