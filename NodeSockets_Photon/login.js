//a quick login app to generate a token, and list the Device IDs on
//an account
var Particle = require('particle-api-js'); //require that api library
var particle = new Particle();
var token;  //a variable to hold our login info

//username and password stored in variables.  
var username = /*shh*/;
var password = /*shh*/;

//login to particle, and include promises.  The "then" method.
//for more information on promises see https://promisesaplus.com/
particle.login({username: username, password: password}).then(
  function(data){
    console.log('API call completed on promise resolve: ', data.body.access_token);
    token = data.body.access_token;
  },
  function(err) {
    console.log('API call completed on promise fail: ', err);
  }
);

//use that particle object to .listDevices to get the ID of the
//devices on acct.
var devicesPr = particle.listDevices({ auth: token });

devicesPr.then(
  function(devices){
    console.log('Devices: ', devices);
  },
  function(err) {
    console.log('List devices call failed: ', err);
  }
);
