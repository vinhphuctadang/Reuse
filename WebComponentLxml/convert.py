# import win32com.client

# # word = win32com.client.Dispatch('Word.Application')
# # doc = word.Documents.Add('E:\\Python\\HTMLparserbaitap.html')
# # doc.SaveAs('Bài tập.doc', FileFormat=0)
# # doc.Close()
# # word.Quit() 
import os

# os.system ('pandoc -s "bài tập.html" -o "bài tập.doc"')
root, dirs, files = next (os.walk('.'))

for file in files:
	if file.endswith ('.html'):
		part = os.path.splitext (file)
		print ('Converting: ', part[0])
		os.system ('pandoc -s "'+file+'" -o "' + part[0] +'.doc"')
		print ('--> CONVERTED')