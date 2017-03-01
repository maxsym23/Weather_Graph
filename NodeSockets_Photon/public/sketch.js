// ITP Networked Media, Fall 2014
// https://github.com/shiffman/itp-networked-media
// Daniel Shiffman

// Keep track of our socket connection
var socket;

//variables to hold the data coming from the server
var ftemp;
var humidity;
var pressure;
var visible;


//graph
//var for the xpos of the graph
var xpos=0;
//height of ea graph
var graphHeight = 100;
//and width
var graphWidth;
//var that holds the background color
var graphBGColor = 240;

//graph vals
var graphTemp;
var graphHumid;
var graphPress;
var graphVisible;

var timerCount = 0;




function setup() {
  createCanvas(640, 540);
  background(0);
  textSize(16);
    
  //setup the graphs
  graphBackgroundReset();


  // Start a socket connection to the server
  // Some day we would run this server somewhere else
  socket = io.connect('http://localhost:3000');
  
   //now get the data
   socket.on('weatherData', function(data){
       //log the data
       console.log(data);
       
       //then assign the variables
       ftemp = data[0];
       humidity = data[1];
       pressure = data[2];
       visible = data[3];
       
       
       //draw sth.  call update values function
       updateVals();
       
       //now call the updateGraph() function
       updateGraph();
   });
}//end of setup

function draw() {
  //nothing
}//end of draw

function updateVals() {
       noStroke();
       textSize(12);
       fill(0);
       rect(0,110,200,18);
       fill(255,(ftemp/2.4),(ftemp/2.4));
       text('temp (F): ' + ftemp, 10, 122);
    
       fill(0);
       rect(0,230,200,18);
       fill(66, (humidity+50), 244);
       text('humidity: ' + humidity, 10, 242);
    
       fill(0);
       rect(0,350,200,18);
       fill(30, (pressure+90), 80);
       text('pressure (Hg): ' + pressure, 10, 362);
    
       fill(0);
       rect(0,470,200,18);
       fill(225,225, (visible/4));
       text('visible light (lux): ' + visible, 10, 482);
    
       //number of times we've gotten data
       timerCount++;
    
       fill(0);
       rect(0,515,240,22);
       fill(255);
       text("number of 'weather data' events: " + timerCount, 10, 524);
    
       
       
}//end of update vals

function updateGraph() {
    //graph lines are in red
    stroke(255,0,0);
    strokeWeight(2);
    //draw the temp line
    //calc temp val, scale and position (from bottom of graph)
    //scale = 70 -> 100, bottom line at vert 110 = 32 degrees f.
    var graphTemp = 110 - ((ftemp-32) * (100/70));
    point(xpos, graphTemp);
    
    //draw the humidity line
    //calc humidity val, scale and position from bottom of graph
    //scale = 1->1, bottom line at vert 230
    var graphHumid = (230 - humidity);
    point(xpos, graphHumid);
    
    //draw the pressure line
    //calc pressure val, scale and position from bottom of graph
    //scale = 10 -> 100, bottom line at vert 350
    var graphPress = 350 - ((pressure - 25) * (100/10));
    point(xpos, graphPress);
    
    //draw the visible light graph
    //calc visible light val, scale and position from bottom of graph
    //scale = 300 -> 100 or 3->1, bottom line at vert 470
    var graphVisible = 470 - ((visible-200) * (1/3));
    point(xpos, graphVisible);
    
    //now move forward
    xpos = xpos + 4;
    //and if we go over the right edge put the canvas back to clear and the ypos back to 1
    // and run the graph backgrounds again
    if(xpos>=width) {
        graphBackgroundReset();
        xpos = 0;
    }
} //end of update graph
    
    
function graphBackgroundReset() {
//set the width of the graphs
  graphWidth = width;
  textSize(8);
  noStroke();
    
//and the background color
    
  //draw the first graph backgrounds.
  //temperature
  fill(graphBGColor);
  rect(0,10,graphWidth,graphHeight);
  //102-32 is our scale for the temp
    //we'll multiply the temp val by 100/70
  fill(0);
  text("102", 2,18);
  text("32", 2,106);
  //humidity
    fill(graphBGColor);
  rect(0,130,graphWidth,graphHeight);
  //0-100 is the scale for the humidity
    //humidity val is 1-1
    fill(0);
  text("100", 2, 138);
  text("0", 2, 226);
  //
  
  //pressure
    fill(graphBGColor);
  rect(0,250,graphWidth,graphHeight);
  //25-35 (10) - 100 is the scale for pressure in Hg
    //we'll multiply the pressure val by 100/10
    fill(0);
  text("35", 2, 258);
  text("25", 2, 346);
  //visible light
  //visible light varies between 200-400 (just in case we go outside)
    //we'll divide the visible val by 2,
    fill(graphBGColor);
  rect(0,370,graphWidth,graphHeight);
    fill(0);
  text("400", 2, 378);
  text("200", 2, 466);
}



