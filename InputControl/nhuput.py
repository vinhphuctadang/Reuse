from pynput import mouse, keyboard
seq = []

def on_move(x, y):
	print('Pointer moved to {0}'.format((x, y)))
	seq.append (('m', 'move', x, y))
def on_click(x, y, button, pressed):
	print('{0} at {1}'.format(
		'Pressed' if pressed else 'Released', (x, y)))
	if pressed:
		seq.append (('m', 'press', x, y))
	else:
		seq.append (('m', 'release', x, y))

def on_scroll(x, y, dx, dy):
	print('Scrolled {0} at {1}'.format(
		'down' if dy < 0 else 'up',
		(x, y)))
	seq.append (('m', 'scroll', x, y, dx, dy))

# Collect events until released
listener = mouse.Listener(
		on_move=on_move,
		on_click=on_click,
		on_scroll=on_scroll) 
listener.start ()

def on_press(key):
	try:
		# print (str(key)[1])
		print('alphanumeric key {0} pressed'.format(
			key.char))
		seq.append (('k', 'press', key.char))
	except AttributeError:
		print('special key {0} pressed'.format(
			key))
		seq.append (('k', 's_press', key))

def on_release(key):
	print('{0} released'.format(
		key))
	if key == keyboard.Key.esc:
		# Stop listener
		return False
	try:
		seq.append (('k', 'release', key.char))
	except AttributeError:
		seq.append (('k', 'release', key))

# Collect events until released
with keyboard.Listener(
		on_press=on_press,
		on_release=on_release) as listener:
	listener.join()

f = open ('sequence.txt', "w");
for motion in seq:
	for i in range (0,len(motion)):
		f.write(str (motion [i]))
		if i<len(motion)-1:
			f.write (',')
	f.write('\n')
f.close ()