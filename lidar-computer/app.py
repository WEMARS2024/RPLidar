# Importing necessary libraries
from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit
import random
import threading
import socket
import numpy as np

# Flask application setup
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode='threading')

# Mock data for initial testing
camera_feeds = {
    # Dictionary containing URLs for camera streams
    # You can change these URLs to point to your actual camera feeds
    
    'camera2': 'http://camera2/stream',
    'camera3': 'http://camera3/stream',
    'camera4': 'http://camera4/stream'
}

path_data = {
      # Dictionary containing a list of coordinates
    # Add or modify the coordinates as per your requirements
    'coordinates': [
        {'lat': 35.6895, 'lon': 139.6917},  # Example coordinates
        {'lat': 35.6896, 'lon': 139.6918},
        # ... add as many points as needed
    ]
}

# Define route for the home page
@app.route('/')
def index():
    # Renders the 'index.html' template when the root URL is accessed
    return render_template("index.html")

# Define route to get camera feed data
@app.route('/camera-feeds')
def get_camera_feeds():
     # Returns the camera feeds data in JSON format
    # In a real application, replace mock data with actual camera feed data
    return jsonify(camera_feeds)

# Define route for sensor data visualization
@app.route('/sensor-data')
def sensor_data():
   # Renders the 'sensorData.html' template for the radar page
    return render_template("sensorData.html")

# Define route to get path data
@app.route('/path-data')
def get_path_data():
    # Returns the path data in JSON format
    # In a real application, you would get this data from your GPS system
    return jsonify(path_data)

# Background thread function to emit sensor data
def send_sensor_data():
    with app.app_context():
        debug = 1
        while True:
            if debug == 0:
                # DEBUG
                data_dict = {}
                for x in range(0,360,30):
                    data_dict[x+90] = np.random.uniform(0,12)
                socketio.emit('sensor-data', data_dict)
                socketio.sleep(2)
            elif debug == 1:
                # Set up the server socket
                server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                host = '172.20.10.2' # wireless LAN adapter
                port = 8080

                server_socket.bind((host, port))
                server_socket.listen(1)
                print(f"Server listening on {host}:{port}")

                while True:
                    # Accept a client connection
                    client_socket, client_address = server_socket.accept()
                    print(f"Connection from {client_address}")

                    # Receive data from the client
                    received_data = client_socket.recv(1024).decode()
                    if not received_data:
                        print("Refused connection")
                        continue
                    print(f"Received data from server: {received_data}")
                    
                    try:
                        data_pairs = received_data.split(';')
                        data_dict = {}

                        for pair in data_pairs:
                            if ':' in pair:
                                deg, dist = pair.split(':')
                                try:
                                    deg = float(deg)
                                    dist = float(dist)
                                    data_dict[deg] = dist
                                except ValueError:
                                    print(f"Invalid data pair: {pair}")
                        print(data_dict)
                        dictLength = len(data_dict)
                        print(f'length of dict: {dictLength}')
                        if (dictLength > 100):
                            socketio.emit('sensor-data', data_dict)
                        socketio.sleep(1)  # Use socketio.sleep instead of time.sleep
                    except ValueError:
                        print("Error parsing data:", received_data)
                    
                    # Close the connection
                    client_socket.close()

# SocketIO event handlers
@socketio.on('connect')
def test_connect():
    # Handles client connection
    print('Client connected')
    # Start sending sensor data in a background thread
    thread = threading.Thread(target=send_sensor_data)
    thread.daemon = True  # Daemonize thread to have it shut down with the main program
    thread.start()

@socketio.on('disconnect')
def test_disconnect():
    print('Client disconnected')

# Main entry point
if __name__ == '__main__':
    socketio.run(app, debug=True)# Starts the Flask application with SocketIO support
