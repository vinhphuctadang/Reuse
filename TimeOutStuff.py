from time import time
def stuff (__stuff, timeOut=1000):

	timeOut /= 1000.0
	now = time ()
	while timeOut and time ()-now<timeOut:
		state, result = __stuff ()
		if state:
			return result
	raise TimeoutError ('Stuff time out')

def testStuff ():
	return 0, 0

def main ():
	result = stuff (testStuff)

if __name__=='__main__':
	main ()