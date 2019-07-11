import pynput
from pynput.mouse import Button, Controller
mouse = pynput.mouse.Controller()
from pynput.keyboard import Key, Controller
keyboard = pynput.keyboard.Controller()

# # Press and release space
# keyboard.press(Key.space)
# keyboard.release(Key.space)

def act (f): # acting the given message
	if (f[0] == 'm'):  # Mouse control
		if (f[1] == 'move'):
			print ('Move mouse to (%s, %s)' % (f[2], f[3]))
			mouse.position = (int (f[2]), int (f[3]))
		elif (f[1] == 'press'):
			print ('Mouse press at (%s, %s)' % (f[2], f[3]))
			mouse.position = (int (f[2]), int (f[3]))
			mouse.press(Button.left)# 
		elif (f[1] == 'release'):
			print ('Mouse relase at (%s, %s)' % (f[2], f[3]))
			mouse.position = (int (f[2]), int (f[3]))
			mouse.release(Button.left)# 

	elif (f[0] == 'k'):
		if f[1] == 'press':
			print ('Key pressed %s' % f[2])
			keyboard.press(f[2])
		elif f[1] == 's_press':
			print ('Key pressed %s' % f[2])
			if (f[2] == 'Key.enter'):
				keyboard.press(Key.enter)
		elif f[1] == 'release':
			if f[2] == 'Key.enter':
				print ('Key release %s' % f[2])
				keyboard.release(Key.enter)
			elif len (f[2]) == 1:	
				print ('Key release %s' % f[2])
				keyboard.release(f[2])

import nhuRead as read
for action in read.sequence:
	act (action)
# Read pointer position
# act (('m','move','10','20'))
# act (('m','click', '10', '20'))
# act (('k', 'press', 'Key.enter'))
# act (('k', 'press', 'b'))
# act (('k', 'release', 'a'))
