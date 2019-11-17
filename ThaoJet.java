import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;



import sun.management.resources.agent;

class Flight { // flight code are already stored
	
	private String flightID;
	private String dateDepart; // date that the flight departs
	private String placeStart, placeEnd;  // source and destination of the flight
	private int flightDuration; // flight duration
	private int vacantChair, occupiedChair; // vacantchair and occupied char;
	
	HashMap<String, HashSet<String>> records; // which <Agent> buy which ticket
	
	public Flight (String flightID, String dateDepart, String placeStart, String placeEnd, 
			int flightDuration,	int vacantChair, int occupiedChair) {
		this.flightID = flightID;
		this.dateDepart = dateDepart;
		this.placeStart = placeStart;
		this.placeEnd = placeEnd; 
		this.flightDuration = flightDuration;
		this.vacantChair = vacantChair;
		this.occupiedChair = occupiedChair;
		records = new HashMap <>();
	}
	
	public String getInfo () {
		return flightID+","+dateDepart+","+placeStart+","+placeEnd+","+flightDuration+","+vacantChair;
	}
	
	public String getVacantChair () {
		return vacantChair+"";
	}
	
	private String generateID (String agentName) {
		int cnt = 0;
		if (records.containsKey(agentName)) {
			cnt = records.get(agentName).size()+1;
		} else {
			cnt = 1;
		}
		return flightID+"-"+agentName+"-"+cnt;
	}
	
	public String book (String agentName) {
		if (vacantChair>0) {
			vacantChair--;
			occupiedChair++;			
			String id = generateID (agentName);
			if (records.containsKey(agentName)) {
				records.get(agentName).add(id);
			} else {
				records.put(agentName, new HashSet <>());
			}
			return id;
		} else {
			return "Het ghe";
		}
	}
	public String cancel (String agentName, String ticketID) {
		if (!records.containsKey(agentName))
			return "Khong ton tai ve";
		Set<String> tickets = records.get(agentName);
		if (!tickets.contains(ticketID))
			return "Khong ton tai ve";
		tickets.remove(ticketID);
		vacantChair++;
		occupiedChair--;
		return "Thanh cong";
	}
	
}


public class ThaoJet {
	
	private static final int port = 2610; 
	private Map<String, String> agents;
	private static Map<String, Flight> flights;
	
	private static Map<String, String> tickets = new HashMap <> ();// for programming trick only
	
	public Map<String, Flight> loadFlightInfo () {
		HashMap<String, Flight> flights = new HashMap<String, Flight>();		
		flights.put("ThaoJet1", 
				new Flight("ThaoJet1", "20-11-2019 20:00:00", "Can Tho", "Ha Noi", 140, 20, 0));
		flights.put("ThaoJet2", 
				new Flight("ThaoJet2", "2-12-2019 06:55:00", "Can Tho", "Da Nang", 90, 11, 0));
		flights.put("ThaoJet3", 
				new Flight("ThaoJet3", "7-12-2019 21:55:00", "Da Nang", "Can Tho", 90, 11, 0));		
		return flights;
	}
	
	public Map<String, String> loadAgentInfo () { // agent, password
		Map<String, String> agents = new HashMap <> ();
		agents.put ("b1609548", "1604");
		return agents;
	}
	
	public String middleware (Socket socket) throws Exception{
		Scanner scanner = new Scanner (socket.getInputStream());
		String authorizeString = scanner.nextLine();
		String params[] = authorizeString.split(" ");
		if (params.length < 2)
			return "";
		String userName = params[0], password = params[1];
		if (!agents.containsKey(userName) || !agents.get(userName).equals(password)) 
			return "";
		return userName;
	}
	
	
	public void reject (Socket socket) throws Exception{
		PrintWriter pw = new PrintWriter(socket.getOutputStream());
		pw.write("Dang nhap that bai");
		pw.flush ();
		
		pw.close();
	}
	
	public void doIO (String name, Scanner input, PrintWriter output) {
		String raw = input.nextLine ();
		System.out.println (name + " da gui '" + raw + "'");
		
		String params[] = raw.split(" ");
		String cmd = params [0];
		
		String toWrite = "";
		
		synchronized (tickets) {
			if (cmd.equals ("list")) {
				for(String key: flights.keySet()) {
					toWrite += flights.get(key).getInfo() + ";";
				}			
			} else if (cmd.equals("buy")) { // command buy <flightID>, returns ticketID
				
				if (params.length < 2) {
					toWrite = "Khong tim thay hang";
					
				} else {
					String flightID = params[1];
					if (!flights.containsKey(flightID))
						toWrite = "Khong tim thay hang";
					else {
						
						String ticket = flights.get(flightID).book(name);
						toWrite = ticket;
						if (!toWrite.equals("Het ghe")) {
							tickets.put(ticket, flightID);
						}
					}
				}
			} else if (cmd.equals("cancel")) {// command buy <ticketID>
				if (params.length < 2) {
					toWrite = "Khong tim thay ve";				
				} else {
					String ticketID = params[1];
					if (!tickets.containsKey(ticketID)) {
						toWrite = "Khong tim thay ve";
					} else {
						String flightID = tickets.get(ticketID);					
						tickets.remove(ticketID);
						toWrite = flights.get(flightID).cancel(name, ticketID);
					}
				}
			} else if (cmd.equals("bought")) {			
				for (String ticketID: tickets.keySet()) {
					if (ticketID.contains(name)) {
						toWrite += ticketID + " ";
					}
				}
			}
		}
		
		output.write(toWrite+"\n");
		output.flush();
	}
	
	public void startServing (String agentName, Socket socket) {
		
		final Socket destination = socket;
		final String name = agentName;
		Thread thread = new Thread () {
			public void run () {
				Scanner scanner = null;
				PrintWriter pw = null;
				try {
					scanner = new Scanner (destination.getInputStream());
					pw = new PrintWriter(destination.getOutputStream());
				} catch (IOException e) {
					System.out.println ("Loi khong mo duoc ket noi");
					return;
				}
				System.out.println (name + " da ket noi.");
				pw.write("Da ket noi thanh cong\n");
				pw.flush();
				
				while (destination.isConnected()) {
					try {
						doIO (name, scanner, pw);
					} catch (NoSuchElementException e) {
						break;
					}
				}
				System.out.println (name + " da NGAT ket noi.");
			}
			
		};
		thread.start();
	}
	
	public void run () {
		System.out.println ("[1] Tim nap du lieu ...");
		agents = loadAgentInfo();
		flights = loadFlightInfo();
		
		
		ServerSocket server = null;
		try {
			server = new ServerSocket(port);
		} catch (IOException e) {
			e.printStackTrace();
			System.out.print("---- Khong the tao lap ket noi, cong " + port + " da bi chiem, thoat chuong trinh. ----");
		}
        //keep listens indefinitely until receives 'exit' call or program terminates
        while(true){
            System.out.println("[2] Lang nghe ket noi");
            //creating socket and waiting for client connection
            try {
            	Socket socket = server.accept(); 
            	System.out.println ("Bat dau ket noi tai " + socket);
            	String loginUserName = middleware (socket);
            	if (!loginUserName.equals ("")) {            		            	
            		startServing (loginUserName, socket);
            	} else {
            		reject (socket);
            	}               
            } catch (Exception e) {
            	e.printStackTrace();
            	System.out.println ("Ngung phuc vu ket noi, loi "+e);
            }            
        }
	}
	
	public static void main (String[] args) {
		ThaoJet instance = new ThaoJet ();
		instance.run ();
	}
}
