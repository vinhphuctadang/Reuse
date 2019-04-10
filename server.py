import socket

def main ():
	HOST = ''  # local host
	PORT = 8888 # listen port, use as big as posible

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((HOST, PORT))
	s.listen()

	print ('Start listening...')
	conn, addr = s.accept()
	
	print('Connected by', addr)
	while True:
		data = conn.recv(1024)
		if not data:
			break
		conn.sendall(data)
if __name__=="__main__":
	main ()