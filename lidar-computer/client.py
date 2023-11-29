import socket

#Set up the client socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '172.30.121.193'  # Replace with the actual IP address of the server
port = 8080              # Use the same port as the server

#Connect to the server
client_socket.connect((host, port))

#send data to hotspot
data_to_send = "Hello, client! I hate you cole."
client_socket.send(data_to_send.encode())

#Close the connection
client_socket.close()