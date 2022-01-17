package socket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/*
 * 服务器端创建ServerSocket，循环调用accept()等待客户端连接。
 * 客户端创建一个socket并请求和服务器端连接。
 * 服务器端接受客户端请求，创建socket与该客户建立专线连接。
 * 建立连接的两个socket在一个单独的线程上对话。
 * 服务器端继续等待新的连接。
 * */
public class Server {
	static int PORT = 3044;
	
	@SuppressWarnings({ "resource", "static-access" })
	public static void main(String[] args) throws Exception {
		// create socket, bind port
		ServerSocket serverSocket = new ServerSocket(PORT);
		
		// keep listening
		System.out.println("waiting ...");
		
		while (true) {
			Socket socket = serverSocket.accept();
			// display info
			System.out.println("Client: "
			+ socket.getInetAddress().getLocalHost()
			+ " connected to server");
			
			new Thread(()->{
				// get input
				try {
					HttpRequest httpRequest = new HttpRequest();
					httpRequest.ParseHttp(socket.getOutputStream(), socket.getInputStream());
					socket.close();
				} catch(Exception e) {
					e.printStackTrace();
				}
			}).start();
		}
		
		
	}
}
