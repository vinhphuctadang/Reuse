import socket

def main ():
	HOST = '192.168.0.106'  # The server's hostname or IP address
	PORT = 8888        # The port used by the server
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((HOST, PORT))
	
	s.sendall(b'Hello, world')
	data = s.recv(1024)
	print('Received', repr(data))

if __name__=="__main__":
	main ()