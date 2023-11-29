# Importing necessary libraries
from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit
import random
import threading

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
    return render_template('index.html')

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
    return render_template('sensorData.html')

# Define route to get path data
@app.route('/path-data')
def get_path_data():
    # Returns the path data in JSON format
    # In a real application, you would get this data from your GPS system
    return jsonify(path_data)

# Background thread function to emit sensor data
def send_sensor_data():
    with app.app_context():
        while True:
            # Simulated sensor data with random values
            sensor_data = {'distance': random.random() * 100, 'angle': random.random() * 360}
            socketio.emit('sensor-data', sensor_data)
            socketio.sleep(1)  # Use socketio.sleep instead of time.sleep

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
