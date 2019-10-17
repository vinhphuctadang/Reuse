import java.util.HashMap;

// An experiment of sending an HTTP request with POST method

public class Main {
	static String targetURL = "http://daisybeaver.000webhostapp.com";
	
	public static void main (String[] args) {
		HashMap<String, String> params = new HashMap<>();
		params.put("apikey", "hello");
		params.put("method", "add");
		params.put("fbid", "phuc");
		params.put("score", "100000");
			
		try {
			System.out.print(HttpApiRequest.syncRequest(targetURL, params));
		} catch (Exception e) {
			System.out.print ("Hindered: "+e);
		}
	}
}
