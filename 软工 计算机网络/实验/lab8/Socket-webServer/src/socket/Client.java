package socket;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class Client {
	static int PORT = 3044;
	static String HOST = "127.0.0.1";
	
	public static void main(String[] args) throws Exception {
		// create socket
		// build connection with server
		Socket socket = new Socket(HOST, PORT);
		
		OutputStream outputStream = socket.getOutputStream();
		String message = "\r\rHello";
		socket.getOutputStream().write(message.getBytes("UTF-8"));
		socket.shutdownOutput();
		
		InputStream inputStream = socket.getInputStream();
		int length;
		byte[] buffer = new byte[1024];
		StringBuilder strb = new StringBuilder();
		length = inputStream.read(buffer);
		while (length != -1) {
			strb.append(new String(buffer, 0, length, "UTF-8"));
			length = inputStream.read(buffer);
		}
		System.out.println("Get message from server\n[message]: " + strb);
		
		// shutdown
		inputStream.close();
		outputStream.close();
		socket.close();
	}
}
