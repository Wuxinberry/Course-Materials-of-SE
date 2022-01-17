/*
 * Creator & Editor: Wu Xinbei
 * Student Number: 3190103044
 */

#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include <signal.h>
#pragma comment (lib, "ws2_32.lib")  //╪сть ws2_32.dll

#define MAX_MSG 1024
#define PORT 3044
using namespace std;

enum REQUEST_TYPE {
	CONNECT,
	SHUTDOWN,
	GETTIME,
	GETNAME,
	GETLIST,
	SEND,
	EXIT
};
string REQUEST_STRING[] = {
	"CONNECT",
	"SHUTDOWN",
	"GETTIME",
	"GETNAME",
	"GETLIST",
	"SEND",
	"EXIT"
};

struct MESSAGE {
	int DESTINATION;
	REQUEST_TYPE OPTION;
	char MSG[MAX_MSG];
};

/*
 * Name: showMenu()
 * Function: print actions
 */
void showMenu() {
	cout << "---------------------------------------------" << endl;
	cout << "Actions: " << endl;
	cout << "1. Build Connection" << endl;
	cout << "2. Shutdown Connection" << endl;
	cout << "3. Get Time" << endl;
	cout << "4. Get Name" << endl;
	cout << "5. Get Link List" << endl;
	cout << "6. Send Message" << endl;
	cout << "7. EXIT" << endl;
}

/*
 * Name: receiveThread
 * params: SOCKET client
 * Fuction: receive and parse message sent by server
 */
int package_num = 0; // record package number
void receiveThread(SOCKET client) {
	char buffer[sizeof(MESSAGE)] = { 0 };
	MESSAGE msg; // store message received
	// receive message
	while (1) {
		int receive_sign = recv(client, buffer, sizeof(buffer), 0);
		if (receive_sign == -1) { // error sign
			// print error information
			cout << "[ERROR]: RECEIVE FAILURE: " + errno << endl;
			exit(0);
		}
		// succeed to receive message
		package_num++;  // update information
		memcpy(&msg, buffer, sizeof(buffer)); // copy received message from buffer
		// display message
		cout << "------------------------------------------------" << endl;
		cout << "Package No. " << package_num << endl;
		cout << "Receive Message: " << endl;
		cout << "* Type: " << REQUEST_STRING[msg.OPTION] << endl;
		cout << "* Destination: " << msg.DESTINATION << endl;
		cout << "* Message: " << msg.MSG << endl;
		cout << "------------ end of message -------------" << endl;

		if (msg.OPTION == SHUTDOWN) { // deal with shutdown option
			cout << "Shutdown ..." << endl;
			exit(0);
		}

		showMenu();
	}
}


sockaddr_in clientAddr;
SOCKET client;
int connect_signal;

/*
 * Name: signalHandle
 * Function: handle crtl+C signal, shutdown client
 */
void signalHandle(int signum) {
	if (signum == SIGINT) {
		cout << "Shuting down client ..." << endl;
		// send shutdown message
		MESSAGE msg = { -1, SHUTDOWN, "" }; // shutdown data package
		char buffer[sizeof(MESSAGE)];
		memcpy(buffer, &msg, sizeof(MESSAGE));
		int send_signal = send(client, buffer, sizeof(buffer), 0);
		if (send_signal == -1) {
			cout << "[ERROR]: Sending Failure: " + errno << endl;
		}
		else {
			exit(0);
		}
		send_signal = send(client, buffer, sizeof(buffer), 0); // try second time
		if (send_signal == -1) {
			cout << "[ERROR]: Sending Failure: " + errno << endl;
		}
		exit(0);
	}
}


int main() {
	// regist SIGNAL
	signal(SIGINT, signalHandle);

	/* Initialization
	 * set WinSock version
	 * init DLL
	 */
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 2, 2 is the newest version

	/* Create Socket */
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == -1) { // if failed, display error information
		cout << "[ERROR]: Socket Creating Failure: " + errno << endl;
		return 0;
	}
	/* Bind Socket*/
	memset(&clientAddr, 0, sizeof(clientAddr)); // set 0 for each byte
	clientAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &clientAddr.sin_addr.s_addr);
	clientAddr.sin_port = htons(PORT);
	
	connect_signal = connect(client, (sockaddr*)&clientAddr, sizeof(clientAddr));
	if (connect_signal == -1) {
		cout << "[ERROR]: Connection Failure: " + errno << endl;
		return 0;
	}
	cout << "Successfully Connected." << endl;

	/* Create sub-thread, keep listenning to client*/
	thread msgListener(receiveThread, client);
	msgListener.detach(); // start

	showMenu();

	while (1) {
		MESSAGE msg = { -1, CONNECT, "" };

		int option, dest_num;
		string input;

		cin >> option; // get action 
		switch (option)  {
		case 1:
			msg.OPTION = CONNECT;
			break;
		case 2:
			msg.OPTION = SHUTDOWN;
			break;
		case 3:
			msg.OPTION = GETTIME;
			break;
		case 4:
			msg.OPTION = GETNAME;
			break;
		case 5:
			msg.OPTION = GETLIST;
			break;
		case 6:
			msg.OPTION = SEND;
			cout << "To [destination]: " << endl;
			cin >> dest_num;
			msg.DESTINATION = dest_num;
			cout << "Enter the message to be sent" << endl;
			cout << "[message]: ";
			cin >> input;
			strcpy_s(msg.MSG, input.c_str());
			break;
		case 7:
			msg.OPTION = EXIT;
			cout << "Exiting ..." << endl;
			return 0;
		default:
			cout << "Wrong Input" << endl;
			break;
		}
		if (option < 1 || option > 7) { // invalid option
			continue;
		}
		// display message
		cout << "Message is sending ..." << endl;
		cout << "* Type: " << REQUEST_STRING[msg.OPTION] << endl;
		cout << "* Destination: " << msg.DESTINATION << endl;
		cout << "* Message: " << msg.MSG << endl;

		// send message
		char buffer[sizeof(MESSAGE)];
		memcpy(buffer, &msg, sizeof(MESSAGE));
		int send_signal = send(client, buffer, sizeof(buffer), 0);
		if (send_signal == -1) {
			cout << "[ERROR]: Sending Failure: " << errno << endl;
		}
	}

	return 0;
}
