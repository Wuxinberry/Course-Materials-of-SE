package socket;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class HttpRequest {
	String method;
	String path;
	
	public void ParseHttp(OutputStream output, InputStream input) throws Exception {
		
		StringBuffer request = new StringBuffer(2048);
		int len;
		byte[] buffer = new byte[2048];
		len = input.read(buffer);
		
		for (int j = 0; j < len; j++) {
			request.append((char)buffer[j]);
		}
		String requestCnt = request.toString();
		System.out.print(requestCnt);
		
		this.method = requestCnt.split(" ")[0];
		this.path = requestCnt.split(" ")[1];
		System.out.println("Method: " + method);
		System.out.println("Path: " + path);
		
		String http = "HTTP/1.1 ";
		String status;
		String info;
		String contentTypeHead = "Content-Type: ";
		String contentType;
		String contentLengthHead = "Content-Length: ";
		String contentLength;
		String content;
		String end = "\r\n";
		if (this.method.equals("GET")) {
			// transform to absolute path
			
			// find file
			File file = new File("." + path);
			
			if(!file.exists()) {
				status = "404";
				contentType = "text/html\r\n";
				info = "File Not Found\r\n";
				content = "<h1>404 ERROR: File Not Found</h1>";
				contentLength = "34\r\n";
				// set response
				String response = http + status + info
						+ contentTypeHead + contentType
						+ contentLengthHead + contentLength
						+ end
						+ content;
				output.write(response.getBytes());
			} else {
				// set status
				status = "200\r\n";
				// get file type
				if (path.split("\\.")[1].equals("txt")) {
					contentType = "text/plain\r\n";
				} else if (path.split("\\.")[1].equals("jpg")){
					contentType = "image/jpg\r\n";
				} else {
					contentType = "text/html\r\n";
				}
				// put in buffer
				output.write((http + status
						+ contentTypeHead + contentType
						+ contentLengthHead + file.length()
						+ "\r\n" + "\r\n").getBytes());
				// read file
				FileInputStream finput = new FileInputStream(file);
				byte[] fbuffer = new byte[1024];
				int curLength = finput.read(fbuffer, 0, 1024);
				while (curLength != -1) {
					output.write(fbuffer);
					curLength = finput.read(fbuffer, 0, 1024);
				}
				finput.close();
			}
			
		} else { // POST
			if (path.equals("/html/dopost")) {
				// set status
				status = "200\r\n";
				contentType = "text/html;charset=utf-8\r\n";
				String[] tmpstrArr = requestCnt.split("\n");
				String postContent = tmpstrArr[tmpstrArr.length-1];
				String[] fields = postContent.split("&");
				String login = "", psword = "";
				for(String str: fields) { // get input
					String key = str.split("=")[0];
					String val = str.split("=")[1];
					if (key.equals("login")) {
						login = val;
					} else if (key.equals("pass")) {
						psword = val;
					}
				}
				// check login 
				String retHtml = "<html><body>LOGIN FALIED</body></html>";
				if (login.equals("3190103044") && psword.equals("3044")) {
					retHtml = "<html><body>LOGIN SUCCEED</body></html>";
				}
				output.write((http + status
						+ contentTypeHead + contentType
						+ contentLengthHead + retHtml.length() + "\r\n"
						+ "\r\n"
						+ retHtml).getBytes());
			} else {
				status = "404";
				contentType = "text/html\r\n";
				info = "File Not Found\r\n";
				contentLength = "0\r\n";
				output.write((http + status + info
						+ contentTypeHead + contentType
						+ contentLengthHead + contentLength
						+ "\r\n").getBytes());
			}
		}
		return ;
	}
	
}
