import socket
import tkinter as tk
from tkinter import Canvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
from threading import Thread

class RadarApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Radar Display")

        self.canvas = Canvas(root, height=0)
        self.canvas.pack()

        self.figure, self.ax = Figure(), None
        self.canvas_tk = FigureCanvasTkAgg(self.figure, master=self.root)
        self.canvas_tk.get_tk_widget().pack()

        self.draw_radar_grid()

        self.landscape = {}

        self.receive_thread = Thread(target=self.receive_data)
        self.receive_thread.start()

    def draw_radar_grid(self):
        if self.ax is not None:
            self.ax.clear()
        else:
            self.ax = self.figure.add_subplot(111, projection='polar')

        max_radius = 6
        self.ax.set_ylim([0, max_radius])
        self.ax.set_yticklabels([])
        self.ax.set_xticklabels([])

        for i in range(1, 6):
            self.ax.plot(np.linspace(0, 2 * np.pi, 100), np.full(100, i * max_radius / 5), color='purple')

        for i in range(0, 360, 30):
            angle_in_radians = np.radians(i)
            self.ax.plot([angle_in_radians, angle_in_radians], [0, max_radius], color='purple')

        self.canvas_tk.draw()

    def update_radar_display(self, data):
        if not self.ax:
            return

        if not all(key in data for key in ['distance', 'angle']):
            print('Invalid data received:', data)
            return

        # update landscape with new distance and angle
        self.landscape[data['angle']] = data['distance']
        print(f"dist: {data['distance']}, angle: {data['angle']}")

        distance_scale = 1
        angle_in_radians = np.radians(data['angle'])

        x = data['distance'] * distance_scale * np.cos(angle_in_radians)
        y = data['distance'] * distance_scale * np.sin(angle_in_radians)

        # Schedule the update in the main thread
        self.root.after(0, lambda: self.ax.plot(x, y, 'ro'))
        self.root.after(0, self.canvas_tk.draw)

        # # update data
        # for deg, distance in self.landscape.items():
        #     print(f'deg:{deg} dist: {distance}')
        #     angle_in_radians = np.radians(deg)
        #     x = distance * distance_scale * np.cos(angle_in_radians)
        #     y = distance * distance_scale * np.sin(angle_in_radians)

        #     # Check if the point exists, and update its data
        #     for line in self.ax.lines:
        #         if np.array_equal(line.get_data(), np.array([x, y])):
        #             line.set_data([x, y])
        #             break
        #     else:
        #         # If the point doesn't exist, plot a new one
        #         self.ax.plot(x, y, 'ro')
        # self.canvas_tk.draw()

    def receive_data(self):
        # distance = np.random.uniform(0,12)
        # angle = np.random.uniform(0,360)
        # data_dict = {'distance': distance, 'angle': angle}
        # self.update_radar_display(data_dict)
        # self.root.after(5000, self.receive_data)
        # Set up the server socket
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        host = '10.0.0.134'
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
                distance_str, angle_str = received_data.strip().split(',')
                distance = float(distance_str)
                angle = float(angle_str)
                data_dict = {'distance': distance, 'angle': angle}
                self.update_radar_display(data_dict)
            except ValueError:
                print("Error parsing data:", received_data)
            
            # Close the connection
            client_socket.close()

if __name__ == "__main__":
    # set up app
    root = tk.Tk()
    app = RadarApp(root)

    root.mainloop()