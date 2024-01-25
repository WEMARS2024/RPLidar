import socket
import time

#Set up the client socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '172.30.120.250'  # Replace with the actual IP address of the server
port = 8080              # Use the same port as the server
#Connect to the server
client_socket.connect((host, port))

#send data to hotspot
d = "360"
for i in range(90):
    d = d + "," + str(5)
print(d)
client_socket.send(d.encode())

#Close the connection
client_socket.close()