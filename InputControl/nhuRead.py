
sequence = []
f = open ('sequence.txt', 'r');
for line in f.read ().split ('\n'):
	sequence.append (tuple (line.split (',')))
# for line in sequence:
# 	print (line)
f.close ()