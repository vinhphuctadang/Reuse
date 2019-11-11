# Yêu cầu: đã cài đặt công cụ cURL
import json
import requests 
import os

TOKEN = 'procon30_example_token'
HOST = "http://127.0.0.1:55531"; # Đặt server thật vào đây
CURRENT_MATCH_URL = "/matches/1"; # Đổi số hiệu trận đấu hiện tại ở đây
'''
	httpGET: Tạo một http request GET lên server
	hiện tại do ta chỉ quan tâm đến server chấm thi nên HOST được sử dụng trong hàm này và URL là đường dẫn TƯƠNG ĐỐI đến chỗ cần lấy
'''
def httpGET (URL, PARAMS):	
	# api-endpoint 
	# sending get request and saving the response as response object 
	requestString = 'curl --silent -H "Authorization: %s" %s%s'%(TOKEN, HOST, URL);
	# print (requestString)
	result = os.popen (requestString).read ();
	return json.loads (result);

'''
	httpPOST: Tạo một http request POST lên server
	tương tự, hiện tại do ta chỉ quan tâm đến server chấm thi nên HOST được sử dụng trong hàm này và URL là đường dẫn TƯƠNG ĐỐI đến chỗ cần lấy
'''

def httpPOST (URL, PARAMS):	
	# api-endpoint 
	# sending get request and saving the response as response object 
	header = 'curl --silent -H "Authorization: %s" -H Content-Type:application/json %s%s' % (TOKEN, HOST, URL);
	jsonRq = json.dumps (PARAMS);
	parsed = json.dumps (jsonRq);
	# print (parsed);
	request = ' -X POST -d %s' % parsed;
	requestString = header + request;
	# print (requestString)
	result = os.popen (requestString).read ();
	return json.loads (result);

def sendActions (lst):
	toSend = {"actions":lst}
	ans = httpPOST (CURRENT_MATCH_URL, toSend)
	return ans

def __action (agentID, action, dx=1, dy=0):
	toReturn = {
		"agentID" : agentID,
		"type" : action,
		"dx": dx,
		"dy": dy
	}
	return toReturn;
def getMatch ():
	result = httpGET (CURRENT_MATCH_URL, {})
	return result;
	# pass
def main ():
	acts = [
		__action (2, "move", dx=1, dy=0)
	]
	print (getMatch())

if __name__=="__main__":
	main ()
