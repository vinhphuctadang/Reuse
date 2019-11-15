import socket

def main ():
	HOST = '192.168.43.223' # The server's hostname or IP address
	PORT = 2610        # The port used by the server
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((HOST, PORT))

	s.sendall (b'B1609548 2610');
	r = s.recv (255)

	for i in range(10):
		s.sendall (b'GET');
		__bytes = s.recv (255);
		input = str(__bytes, 'utf-8');
		print (input)

		A, B = map (int, input.split ())
		s.sendall (b'ANSWERa');
		verdict = s.recv(255);
		print (verdict)		

	s.close ()

if __name__=="__main__":
	main ()
