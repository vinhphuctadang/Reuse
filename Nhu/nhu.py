from shutil import copyfile
from json import loads
from os import path, chdir, mkdir

def customSplitExt (name):
	custom = path.splitext (name)
	if len (custom[0])>0 and custom[0][0] == '.':
		return ('', custom [0])
	return custom

def traverse (j,desination,dir='.',space=0,) :
	written = 0
	if 'content' not in j:
		raise ValueError ('Json wrong format: Container has no content key')
	for file in j['content']:
		if 'name' not in file:
			raise ValueError ('Json wrong format: %s' % str (file))

		tmp =  customSplitExt (file['name'])[1]
		# print (tmp);

		if (tmp == '.drive'):
			continue

		if 'content' not in file:

			print (' ' * space, "%-30s" % file['name'], sep ='', end='')
			try:
				# myzip.write (dir+"\\"+file['name']);
				copyfile (dir+"\\"+file['name'], desination + "\\" + file['name'])
				size = path.getsize (dir+"\\"+file['name'])
				written += size
				print ('... %d byte(s)' % size)
			except Exception as e:
				print ('... ERROR:', e)

			
		else:
			print (' ' * space, file['name']+"\\", sep ='')
			mkdir (desination+"\\"+file['name'])
			size = traverse (file, desination+"\\"+file['name'], dir+"\\"+file['name'], space+3)
			written += size

	return written
def getRoot ():
	abspath = path.abspath (__file__)
	# print (abspath)
	fname = path.split (abspath)[0]

	return fname	
def function (name='.drive'):
	try:
		f = open (name, 'r')
		j = loads (f.read ())
		f.close ()
	except Exception as e:
		print ('Error:',e)
		print ('Contact files creator or go to HNhu homepage for more details')
		return

	if ('module' not in j):
		print ('Invalid module name')
		return

	fname = path.basename (name)
	fBaseNameNoExt = customSplitExt (fname) [0]
	fdir  = name [0:len (name)-len (fname)]	
	if (fdir == ''):
		fdir = '.'
	chdir (fdir)
	print ('Copying files...')
	# print ('At %s:' % path.abspath (fdir))

	try:
		mkdir (getRoot () + "/" + j['module'])
	except Exception as e:
		print ('Error:',e)
		print ('You may need to delete the existed one ("%s") to reinstall this module, installation FAILED => stopped' % (getRoot () + "\\"+j['module']))

		return 
	traverse (j, getRoot ()+"/" + j['module'] , '.', 0)

def main ():
	
	# function ()
	import argparse
	import sys
	parser = argparse.ArgumentParser(description='Installer')
	parser.add_argument('--file', help='Specify which .drive file for installing', default = '.drive', type=str)
	try:
		args = parser.parse_args (sys.argv[1: ])
		function (args.file)
	except Exception as e:
		print ("Error found:", e)
		print ('You mean: nhu <a .drive file name> ?')

if __name__=='__main__':
	main ()