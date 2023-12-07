/*  name    - Shubham Anil Khangar
    class   - Msc(Electronics Science)
    project - PLE for the third semister in the master in electronics science                                                                             */



//--->including the required packeges into the main program directory 
var mqtt = require('mqtt'); // mqtt.js pack for mqtt specific
const fs = require('fs'); // file system pack for dealing with the file of the file manager directory
const path = require('path');
const express = require("express");


const app = express();
const port = process.env.PORT || 3000;
const hostname = '0.0.0.0';
const { url } = require('inspector');

global.bot1 = "";
global.bot2 = "";
global.bot3 = "";
global.bot4 = "";


//------>this is for rendering the static html file to the clients--
app.use('/static', express.static('static'))


//------>pug specific----------------------
app.set('view engine', 'pug'); //setting the engine as the .pug
app.set('views', path.join(__dirname, 'views')); //setting the view directory named as the views


//_________________Endpoints Specific________________________________________________



app.get("/", (req, res)=> {
    console.log('Refresh');
    res.status(200).render('index.pug', { sens: global.bot4, but3: global.bot1, but2: global.bot2, but1: global.bot3});
});

app.get("/button1", (req, res)=> {
    console.log(req.url);
    client.publish('buttonReq', 'button1 is pressed')
});

app.get("/button2", (req, res)=> {
    console.log(req.url);
    client.publish('buttonReq', 'button2 is pressed')
});

app.get("/button3", (req, res)=> {
    console.log(req.url);
    //res.send("this is the ple made by me");
    client.publish('buttonReq', 'button3 is pressed')
});


app.listen(port, hostname, () => {
    console.log(`Server running on port ${port}`);
});

//_______________ mqtt specific data___________________________



var options = {
    host: process.env.MQTT_HOST || 'f27f91e6d8b24d02a7157ea968dd6c9d.s2.eu.hivemq.cloud',
    port: process.env.MQTT_PORT || 8883,
    protocol: process.env.MQTT_PROTOCOL || 'mqtts',
    username: process.env.MQTT_USERNAME || 'project',
    password: process.env.MQTT_PASSWORD ||  'Project@1'
}

var client = mqtt.connect(options); // mqtt function to connect with the broker

// function called back whe the the server is connected with the mqtt broker 
client.on('connect', function () {
    console.log('Connected with the mqtt broker');
});

// function called when there is an error with the mqtt broker and the node server 
client.on('error', function (error) {
    console.log(error);
});

//=================called when responce from  mqtt broker is recieved==========================================
client.on('message', function (topic, message) {
    console.log('Received message:', topic, message.toString()); // prints the responce
    if(topic == "buttonRes"){
        console.log("its the if block of the buttonRes");
        const jsonObject = JSON.parse(message);
           // Access the data
           global.bot1 = jsonObject.led1; 
           global.bot2 = jsonObject.led2;  
           global.bot3 = jsonObject.led3; 
           global.bot4 = jsonObject.sensor;
        
    }
    else if(topic == "survey"){
        console.log("its the if block of the survey")
        // Parse the JSON string
        const jsonObject = JSON.parse(message);
        // Access the data
        global.bot1 = jsonObject.led1; 
        global.bot2 = jsonObject.led2;  
        global.bot3 = jsonObject.led3; 
        global.bot4 = jsonObject.sensor;

        
    }
});



// mqtt topic subscribing specific function 
client.subscribe('survey', ()=>{
    console.log('survey is subscribed')
});

client.subscribe('buttonRes', ()=>{
    console.log('buttonRes is subscribed')
});
// mqtt topic publishing specific function 
//client.publish('project', 'firts msg at the start', ()=>{
//console.log('firts msg at the start')
//});

