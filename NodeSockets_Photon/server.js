// Adapted from Dan Shiffman's Tutorial: https://www.youtube.com/watch?v=bjULmG8fqc8
// Code: https://github.com/CodingTrain/Rainbow-Code/tree/master/sockets
// pulls variables from a particle photon and sends it to a server, with p5.js running client-side


// Using express: http://expressjs.com/
var express = require('express');
// Create the app
var app = express();

// Set up the server
// process.env.PORT is related to deploying on heroku
var server = app.listen(process.env.PORT || 3000, listen);

// This call back just tells us that the server has started
function listen() {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Example app listening at http://' + host + ':' + port);
}

//use the public folder in order to serve our files
app.use(express.static('public'));

//Particle - Photon portion
//require particle
var Particle = require('particle-api-js'); //require that api library
var particle = new Particle();//a new particle object
var token = /*shh*/;  //a variable to hold our token - we can get this from
var myID = /*shh*/;//the id of the device that we want to target

var particleInterval = 3000;//the interval at which we grab the particle data

//all the variables that we want to get from the particle
var ftemp; 
var humidity;
var pressure;
var visible;

var dataArray = [3];

// WebSocket Portion
// WebSockets work with the HTTP server
var io = require('socket.io')(server);

// Register a callback function to run when we have an individual connection
// This is run for each individual user that connects
//when the socket server connects, start the interval to get data from particle and emit
io.sockets.on('connection',
  // We are given a websocket object in our function
  function (socket) {
  
    console.log("We have a new client: " + socket.id);
    
    //now run the emission at an interval
    var socketInterval = setInterval(function () {
        
        //get the data from photon.
        particle.getVariable({ deviceId: myID, name: 'ftemp', auth: token }).then(function(data) {
        //get the data store in variable, then emit it
        console.log('F. temperature : ', data.body.result);
        ftemp = data.body.result;
        dataArray[0] = ftemp;

        }, function(err) {
        console.log('An error occurred while getting attrs:', err);
        });
        
        //get the humidity
        particle.getVariable({ deviceId: myID, name: 'humidity', auth: token }).then(function(data) {
        //get the data, store in variable, then emit
        console.log('Humidity : ', data.body.result);
        humidity = data.body.result;
        dataArray[1] = humidity;

        }, function(err) {
        console.log('An error occurred while getting attrs:', err);
        });
        
        //get the pressure
        particle.getVariable({ deviceId: myID, name: 'pressureHg', auth: token }).then(function(data) {
        //get the data, store in variable, then emit
        console.log('Pressure(Hg) : ', data.body.result);
        pressure = data.body.result;
        dataArray[2] = pressure;

        }, function(err) {
        console.log('An error occurred while getting attrs:', err);
        });
        
        //get the visible light reading
        particle.getVariable({ deviceId: myID, name: 'visible', auth: token }).then(function(data) {
        //get the data, store in variable, then emit
        console.log('Visible light : ', data.body.result);
        visible = data.body.result;
        dataArray[3] = visible;

        }, function(err) {
        console.log('An error occurred while getting attrs:', err);
        });
        
        
        //now emit the data from the array we just built
        socket.emit("weatherData", dataArray);
            
    }, particleInterval);
    

    socket.on('disconnect', function() {
        clearInterval(socketInterval);
        console.log("Client has disconnected");
    });
  }
);