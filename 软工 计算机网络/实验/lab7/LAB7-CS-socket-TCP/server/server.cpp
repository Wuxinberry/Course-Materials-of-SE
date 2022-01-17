#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <thread>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 3044
#define MAXMSG 1024


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
	char MSG[MAXMSG];
};

struct CLIENT {
	int port;
	string ip;
};

map<int, CLIENT> clientInfo; //  id(key): client(value)
map<int, struct sockaddr_in> clientList; // id(key): socket
map <int, bool> flags; // status of link
sockaddr_in serverAddr;


void handleReceive(sockaddr_in addr, SOCKET client) {
	char tmpClient[255];
	inet_ntop(AF_INET, &serverAddr.sin_addr, tmpClient, 16);
	clientList[client] = serverAddr;
	clientInfo[client].ip = tmpClient;
	clientInfo[client].port = serverAddr.sin_port;

	// display information 
	cout << "CLIENT: " << endl;
	cout << "* ID: " << client << endl;
	cout << "* IP: " << clientInfo[client].ip << endl;
	cout << "* PORT: " << clientInfo[client].port << endl;

	flags[client] = true;

	while (flags[client]) {
		// receive packages
		char buffer[sizeof(MESSAGE)];
		int receive_signal = recv(client, buffer, sizeof(buffer), 0);
		if (receive_signal == -1) {
			flags[client] = false;
			clientInfo.erase(client);
			cout << "[ERROR]: Receive Failure: " << errno << endl;
			break;
		}
		cout << "Connection Request from Client: " << client << " Received" << endl;

		// parse package
		MESSAGE recvMsg, sendMsg;
		memcpy(&recvMsg, buffer, sizeof(MESSAGE));
		memset(sendMsg.MSG, 0, sizeof(sendMsg.MSG));
		// display reveived package
		cout << "Received: " << endl;
		sendMsg.DESTINATION = client;
		sendMsg.OPTION = recvMsg.OPTION;

		time_t cc;
		string cur_time;
		string curLinks;
		string text;
		int send_signal;

		switch (recvMsg.OPTION) {
		case SHUTDOWN:
			flags[client] = 0;
			clientInfo.erase(client);
			cout << "Shutdown connection with " << client << endl;
			break;
		case GETTIME:
			cout << "Send server time to " << client << endl;
		    cc = time(0);
			cur_time = ctime(&cc);
			strcpy_s(recvMsg.MSG, cur_time.c_str());
			break;
		case GETNAME: 
			cout << "Send server name to " << client << endl;
			char server_name[255];
			gethostname(server_name, 255);
			strcpy_s(recvMsg.MSG, server_name);
			break;
		case GETLIST:
			cout << "Send active links to " << client << endl;
			curLinks = "\tID\tIP\tPort\n";
			for (auto i : clientInfo)
				curLinks = curLinks + "\t" + to_string(i.first) + "\t" + i.second.ip + "\t" + to_string(i.second.port) + "\n";
			strcpy_s(recvMsg.MSG, curLinks.c_str());
			break;
		case SEND:
			cout << "Send message from " << client << "to " << recvMsg.DESTINATION << endl;
			text = recvMsg.MSG;
			text = "message from [" + to_string(client) + "]: " + text;
			memset(sendMsg.MSG, 0, sizeof(sendMsg.MSG));
			strcpy_s(sendMsg.MSG, text.c_str());
			sendMsg.DESTINATION = recvMsg.DESTINATION;
			
			cout << "Start sending ..." << endl;
			send_signal = send(sendMsg.DESTINATION, (char*)&sendMsg, sizeof(sendMsg), 0);
			if (send_signal < 0) {
				cout << "[ERROR]: Send failure: " << errno << endl;
				strcpy_s(recvMsg.MSG, "Sending Failed");
			}
			break;
		default:
			cout << "wrong input." << endl;

		}

		cout << "Send: " << endl;
		cout << "* Type: " << REQUEST_STRING[sendMsg.OPTION] << endl;
		cout << "* Destination: " << sendMsg.DESTINATION<< endl;
		cout << "* Message: " << sendMsg.MSG << endl;
		cout << endl;

		if (recvMsg.OPTION >= 1 && recvMsg.OPTION <= 7) {
			sendMsg.DESTINATION = client;
			cout << "Replying ..." << endl;
			text = recvMsg.MSG;
			text = "Reply from server: \n" + text + "\nYour request has been processed";
			strcpy_s(sendMsg.MSG, text.c_str());
			send_signal = send(client, (char*)&sendMsg, sizeof(sendMsg), 0);
			if (send_signal < 0) {
				cout << "[ERROR]: Send failure: " << errno << endl;
			}
			else {
				cout << "Done." << endl;
			}
		}
		if (recvMsg.OPTION == GETTIME) {
			for (int i = 0; i < 99; i++) {
				cout << "Send server time to " << client << endl;
				cc = time(0);
				cur_time = ctime(&cc);
				strcpy_s(recvMsg.MSG, cur_time.c_str());

				sendMsg.DESTINATION = client;
				cout << "Replying ..." << endl;
				text = recvMsg.MSG;
				text = "Reply from server: \n" + text + "\nYour request has been processed";
				strcpy_s(sendMsg.MSG, text.c_str());
				send_signal = send(client, (char*)&sendMsg, sizeof(sendMsg), 0);
				if (send_signal < 0) {
					cout << "[ERROR]: Send failure: " << errno << endl;
				}
				else {
					cout << "Done." << endl;
				}
			}
		}
	}
	return;
}


int main() {
	ios::sync_with_stdio(false); // speed up
	cout << "Initializing server ..." << endl;

	/* Initialization
	 * set WinSock version
	 * init DLL
	 */
	WSADATA wsaData;
	int wsastart_signal = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsastart_signal != 0) {
		cout << "[ERROR]: DLL initialization failure: " << errno << endl;
		return 0;
	}

	/* Create Socket*/
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* Bind Socket */
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
	int bind_signal = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
	if (bind_signal == -1) {
		cout << "[ERROR]: Bind Failure: " << errno << endl;
		return 0;
	}

	int listen_signal = listen(serverSocket, 20);
	if (listen_signal == SOCKET_ERROR) {
		cout << "[ERROR]: Listen failure: " << errno << endl;
		return 0;
	}

	cout << "Server Successfully Initialized." << endl;

	int nSize = sizeof(SOCKADDR);

	while (1) {
		SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&serverAddr, &nSize);
		if (clientSocket == -1) {
			cout << "[ERROR]: Receiving Failure: " << errno << endl;
			return 0;
		}

		thread clientThread(handleReceive, serverAddr, clientSocket);
		clientThread.detach();
	}

	return 0;
}
