#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>

#include "SocketCommunication.h"
#include "Login.h"
#include "CommandProcess.h"

int main(int argc, char* argv[])
{
  WSADATA wsa;
  SOCKET server;
  SOCKADDR_IN serverAddress;

  char receiveBuffer[2048];
  int id;

  //Connect to remote server
  if (connectToServer("127.0.0.1", 26780, &wsa, &server, &serverAddress))
  {
	if (login(server, "C", &id))
	  startClientCommandProcess(server, id);
	else
	  puts("login failed");
  }
  else
  {
	puts("Failed");
  }  

  closesocket(server);
  WSACleanup();
}