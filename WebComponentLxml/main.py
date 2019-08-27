from lxml import etree 
import requests

def download (page, savedName):


	page = requests.get(page)
	html = etree.HTML (page.text)
	written = etree.HTML (
		'''<html>
			<head>
				<meta charset="utf-8">
			</head>
		</html>
		'''
		)

	# solution below found on https://stackoverflow.com/questions/6123351/equivalent-to-innerhtml-when-using-lxml-html-to-parse-html/6123758
	# answerer: https://stackoverflow.com/users/180709/zopieux

	# header = html.xpath("//head") [0] # find a header
	lesson = html.xpath("//div[@class='gb-chapter']") # find gb-chaper
	# written.append (header) # append header into written HTML
	node = etree.Element ('body')
	node.append (lesson[0])
	written.append (node)

	f = open (savedName, 'w', encoding='utf-8')
	output = etree.tostring (written, encoding='unicode')
	# print (output)
	f.write (output)
	f.close ()


def getLinks (page):
	result = {}
	html = etree.HTML (page)
	# "//div[@class='gb-chapter']"

	chapters = html.xpath (".//div[@class='gb-chapter-list mt2']")
	for chap in chapters:
		nodes = chap.xpath ('.//a')
		for node in nodes:
			# print (node.get ('href'))
			span = node.xpath (".//span")[0].text
			# print (span)
			result ["https://giaibaitap.me/"+node.get ('href')] = span

	return result

def main ():

	#download ('https://giaibaitap.me/lop-5/giai-bai-1-2-3-trang-4-vo-bai-tap-toan-5-tap-1-a34131.html', 'output.html')
	f = open ('offline.txt', 'r', encoding='utf-8')
	page = f.read ()
	f.close ()

	lst = getLinks (page)
	for key in lst:
		print ('Downloading:',key)
		download (key, lst[key]+".html")
		print ('--> DOWNLOADED')


if __name__=='__main__':
	main ()