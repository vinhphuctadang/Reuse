import socket
from random import randrange as rd
from threading import Thread 

def __gcd (x,y):
	if y == 0:
		return x;
	return __gcd (y, x%y)

def middleware (string):

	splits = string.split ()
	if (splits[0] == 'B1609548' and splits[1] == '2610'):
		return 'B1609548'
	raise ValueError ("Authorization failed")

def serve (socket):


	print('Client service thread created for', socket)

	A = -1
	B = -1
	count = 0
	correct = 0
	limit = 10

	input = socket.recv (255)
	temp = str(input, 'utf-8')

	try:
		mssv = middleware (temp)
		socket.sendall (b'CHAP NHAN')
		print ('MSSV', mssv, 'dang ket noi')
	except:
		socket.sendall (b'DANG NHAP SAI, 0 DIEM')
		socket.close ()
		return

	
	try: 
		while count <= limit:

			input = socket.recv (255)
			if not input:
				break;

			temp = str(input, 'utf-8')
			splits = temp.split ()
			if (len (splits) == 0):
				continue

			print ("received:", temp)

			strinp = splits[0]

			if strinp == "GET":
				if A == -1 or B == -1:				
					count += 1

					if (count > limit):
						socket.sendall (b"HET");
						break;				
					A = 3#rd (1, 1000000000)
					B = 6#rd (1, 1000000000)

				toSend = str(A) + " " + str(B);
				socket.send (bytes (toSend, encoding='utf8'))
				pass
			elif strinp == "ANSWER": 
				if A == -1 or B == -1:
					socket.sendall (b"LOI: CHUA GUI YEU CAU")
					continue
				if (len(splits) < 2):
					socket.sendall (b"LOI: KHONG NHAN DUOC DAP AN")

				ans = 0
				try:
					ans = int (splits[1])
				except:
					socket.sendall (b"LOI: DAP AN PHAI LA SO")							
					continue
				gcd = __gcd (A, B)

				if (gcd == ans):
					socket.sendall (b"DANH GIA: DUNG")
					correct += 1
				else:
					socket.sendall (b"DANH GIA: SAI")
				A = -1
				B = -1
				pass
			else:
				socket.sendall (bytes ("LOI: KHONG BIET DANG GUI GI", encoding='utf8'))
		socket.close ();
	except Exception as e:
		pass

	print ("Danh gia", mssv, ":", correct ,'/', limit)

def main ():
	HOST = 'localhost'  # local host
	PORT = 2610 # listen port, use as big as posible
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((HOST, PORT))
	s.listen()
	print ('Start listening...')
	while True:
		conn, addr = s.accept()
		thread = Thread (target = serve, args = (conn,))
		thread.start ()

if __name__=="__main__":
	main ()
