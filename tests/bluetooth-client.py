import socket

client_socket=socket.socket(socket.AF_BT,socket.SOCK_STREAM)

client_socket.connect(("00:18:F8:89:8B:A5",1))

client_socket.send("Hello World")

client_socket.close()