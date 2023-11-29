//run using sensor to get real data interpretation
// const express = require('express');
// const http = require('http');
// const socketIo = require('socket.io');
// const SerialPort = require('serialport');
// const Readline = require('@serialport/parser-readline');

// const app = express();
// const server = http.createServer(app);
// const io = socketIo(server);

// // Replace with your actual serial port path
// const port = new SerialPort('COM3', { baudRate: 115200 });
// const parser = port.pipe(new Readline({ delimiter: '\n' }));

// app.use(express.static('public'));

// io.on('connection', (socket) => {
//   console.log('A client connected');
// });

// parser.on('data', (data) => {
//   // Assuming the Arduino outputs JSON strings
//   try {
//     const sensorData = JSON.parse(data);
//     io.emit('sensor-data', sensorData);
//   } catch (e) {
//     console.error(e);
//   }
// });

// const webServerPort = 3000;
// server.listen(webServerPort, () => {
//   console.log(`Server listening on port ${webServerPort}`);
// });




//Test run with simulated data
// Importing necessary modules
const express = require('express');
// express is a web application framework for Node.js, used for building web applications and APIs
const http = require('http');
// http module is used to create HTTP server
const socketIo = require('socket.io');
// socket.io enables real-time bidirectional event-based communication

// Initializing express application
const app = express();
// Creates an instance of an express application
const server = http.createServer(app);
// Creates an HTTP server that listens to server ports and gives a response back to the client
const io = socketIo(server);
// Initialize socket.io with the server, allowing it to listen for incoming sockets

// Serving static files
app.use(express.static('public'));
// Express middleware to serve static files from the 'public' directory

// Handling socket.io connections
io.on('connection', (socket) => {
  console.log('A client connected');
  // This callback is executed when a new client connects
  // Emitting simulated data every second
  setInterval(() => {
    const simulatedData = {
      distance: Math.random() * 100, // Generating a random distance
      angle: Math.random() * 360, // Generating a random angle
      quality: Math.random() * 100 // Generating a random quality value
    };
    socket.emit('sensor-data', simulatedData);
    // Emitting the 'sensor-data' event to the client with the simulated data
  }, 1000);
  // The setInterval function is called every 1000 milliseconds (1 second)
});

// Handling HTTP GET request to the root URL '/'
app.get('/', function(req, res) {
    res.sendFile(__dirname + '/public/sensorData.html');
    // Responds by sending the 'sensorData.html' file when the root URL is accessed
});

// Starting the server
const webServerPort = 3000;
server.listen(webServerPort, () => {
  console.log(`Server listening on port ${webServerPort}`);
  // The server starts listening on port 3000
});
