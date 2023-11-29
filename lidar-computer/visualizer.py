import tkinter as tk
from tkinter import Canvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

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

        self.root.after(2500, self.simulate_sensor_data)  # Simulate sensor data every 100 milliseconds

    def draw_radar_grid(self):
        if self.ax is not None:
            self.ax.clear()
        else:
            self.ax = self.figure.add_subplot(111, projection='polar')

        max_radius = 12
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

        angle_in_radians = np.radians(data['angle'])
        distance_scale = 200 / 100  # Adjust scale as needed
        x = data['distance'] * distance_scale * np.cos(angle_in_radians)
        y = data['distance'] * distance_scale * np.sin(angle_in_radians)

        self.ax.plot(x, y, 'ro')
        self.canvas_tk.draw()

    def simulate_sensor_data(self):
        # Simulate sensor data (replace this with actual data from your sensor)
        sensor_data = {
            'distance': np.random.uniform(0, 100),
            'angle': np.random.uniform(0, 360)
        }

        self.update_radar_display(sensor_data)
        self.root.after(100, self.simulate_sensor_data)  # Repeat after 100 milliseconds


if __name__ == "__main__":
    root = tk.Tk()
    app = RadarApp(root)
    root.mainloop()
