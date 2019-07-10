import socket

MSG_HEADER_SIZE = 4

class MsgComm (socket.socket):  
	
	def sendMsg (self, msg):
		header = len (msg)
		self.send (header.to_bytes (MSG_HEADER_SIZE, byteorder='little'))
		self.send (bytes(msg))

	def recvMsg (self, bufferSize=1024, maxSize=1000000000):
		header =  self.recv (MSG_HEADER_SIZE);
		size = int.from_bytes (header, byteorder='big', signed=False);
		if (size > maxSize):
			raise ValueError ('Overflow size of %d' % size)
		if (len(header) == MSG_HEADER_SIZE):

			collection = b'';

			while size:
				msg = self.recv (min (bufferSize, size))
				size -= min (bufferSize, size, len (msg))
				collection += msg
			return collection
		


def main ():
	HOST = '127.0.0.1'  # The server's hostname or IP address
	PORT = 16472        # The port used by the server
	s = MsgComm (socket.AF_INET, socket.SOCK_STREAM)
	s.connect((HOST, PORT))
	print ('Connected')
	
	data = s.recvMsg ()
	print('Received:', data)

if __name__=="__main__":
	main ()	