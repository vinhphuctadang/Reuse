import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Map;

public class HttpApiRequest {
	
	public static String syncRequest (String URL, Map<String, String> params) throws Exception{		
		String result = "";
		result = sendRq (URL, toURLParameters(params));
		return result;
	}
	
	private static String toURLParameters (
			Map<String, String> params) {
		StringBuilder result = new StringBuilder ();
		int cnt = 0;
		for (Map.Entry<String, String> e: params.entrySet()) {
			if (cnt > 0) 
				result.append("&");
			result.append(e.toString());
			cnt ++;
		}
		return result.toString();
	}
	
	// solution found at https://stackoverflow.com/questions/1359689/how-to-send-http-request-in-java
	private static String sendRq (String targetURL, String urlParameters) throws Exception{
		URL url = new URL(targetURL);
	    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
	    connection.setRequestMethod("POST");
	    connection.setRequestProperty("Content-Type", 
	        "application/x-www-form-urlencoded");
	    	
	    connection.setRequestProperty("Content-Length", 
	        Integer.toString(urlParameters.getBytes().length));
	    connection.setRequestProperty("Content-Language", "en-US");  

	    connection.setUseCaches(false);
	    connection.setDoOutput(true);
	    //Send request
	    DataOutputStream wr = new DataOutputStream (
	        connection.getOutputStream());
	    wr.writeBytes(urlParameters);
	    wr.close();

	    //Get Response  
	    InputStream is = connection.getInputStream();
	    
	    BufferedReader rd = new BufferedReader(new InputStreamReader(is));
	    StringBuilder response = new StringBuilder(); // or StringBuffer if Java version 5+
	    String line;
	    while ((line = rd.readLine()) != null) {
	      response.append(line);
	      response.append('\r');
	    }
	    rd.close();
	    return response.toString();
	}
}
