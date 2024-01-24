import socket

# The IP address and port of the server you want to send data to
server_ip = "172.20.10.2"  # Replace with the actual IP address
server_port = 8080  # Replace with the actual port number

# data
data = ''
for deg in range(0,360,5):
    new = f"{deg}:5;"
    data += new

print(data)

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect((server_ip, server_port))

# Send data to the server
client_socket.send(str(data).encode())

# Close the connection
client_socket.close()
