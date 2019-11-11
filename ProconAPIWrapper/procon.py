# Yêu cầu: đã cài đặt công cụ cURL
import json
import requests 
import os
'''
	httpGET: Tạo một http request GET lên server
'''
HOST = "http://127.0.0.1:51145/";

def httpGET (URL, PARAMS):	
	# api-endpoint 
	# sending get request and saving the response as response object 
	requestString = 'curl --silent -H "Authorization: procon30_example_token" %s%s'%(HOST, URL);
	result = os.popen (requestString).read ();
	return json.loads (result);

def httpPOST (URL, PARAMS):	
	# api-endpoint 
	# sending get request and saving the response as response object 
	header = 'curl --silent -H "Authorization: procon30_example_token" -H Content-Type:application/json %s%s' % (HOST, URL);
	jsonRq = json.dumps (PARAMS);
	parsed = json.dumps (jsonRq);
	# print (parsed);
	request = ' -X POST -d %s' % parsed;
	requestString = header + request;
	# print (requestString)
	result = os.popen (requestString).read ();
	return json.loads (result);

def sendActions (URL, lst):
	toSend = {"actions":lst}
	ans = httpPOST (URL, toSend)
	return ans

def __action (agentID, action, dx=1, dy=0):
	toReturn = {

		"agentID" : agentID,
		"type" : action,
		"dx": dx,
		"dy": dy
	}
	return toReturn;
def __agent (agentID):
	pass

def main ():
	URL = '/matches/1/action'	
	lst = [
		__action (2, "move", dx=1, dy=1),
		__action (3, "move", dx=-1, dy=-1),
	];

	print (sendActions (URL, lst))

	URL = '/matches/1';
	print (httpGET (URL, {}))

if __name__=="__main__":
	main ()
