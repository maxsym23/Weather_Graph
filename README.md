## Synopsis
Code for a weather station using the Particle Photon: https://www.particle.io/ and a local server in node.js with socket.io.  The data is pulled from the Photon using the particle-api-js package and particle.getVariable.  Data is sent using socket.io and socket.emit.  A client-side browser running a p5.js sketch graphs the data output.


## Installation
Flash the firmware to your particle photon using the particle Build IDE.  Run the server
locally with node server.js in the command line, open a browser window and navigate to http://localhost:3000.

## Credits
Adapted from code by Aaron Ardiri: http://ardiri.com/blog/building_a_weather_station_with_the_particle_io_photon and Dan Shiffman: https://github.com/CodingTrain/Rainbow-Code/tree/master/sockets.

