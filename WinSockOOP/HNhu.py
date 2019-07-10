import MsgComm
import json

class HNhu (MsgComm.MsgComm): 

	def __init__ (self,HOST='127.0.0.1',PORT=16472, autoConnect = True):

		MsgComm.MsgComm.__init__ (self, HOST=HOST, PORT=PORT)
		if autoConnect:
			MsgComm.MsgComm.connect (self)			

	def request (self, jmsg):
		jmsg ['type'] = 'request'
		self.sendMsg (json.dumps (jmsg))
		result = self.recvMsg () # sync, threading for async :)
		return json.loads (result)

	def response (self, jmsg_src, jmsg):
		jmsg ["source"] = jmsg_src ["source"];
		jmsg ["type"] = "response"
		self.sendMsg (json.dumps (jmsg))

	


def main ():
	HOST = '127.0.0.1'  # The server's hostname or IP address
	PORT = 16472        # The port used by the server
	nhu = HNhu ()
	result = nhu.request ({'function':'kernel', 'request':'get', 'variable':'vars'})
	print (result)

if __name__=="__main__":
	main ()	