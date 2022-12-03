#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>

#include "Communication.h"
#include "Login.h"

int main(int argc, char* argv[])
{
  WSADATA wsa;
  SOCKET server;
  SOCKADDR_IN serverAddress;

  char receiveBuffer[2048];

  //Connect to remote server
  if (connectToServer("127.0.0.1", 26780, &wsa, &server, &serverAddress))
  {
	login(server, "C");/*
	if (sendMessage(server, "C$10"))
	  puts("Message sent");
	else
	  puts("Sending failed");*/
	if (receiveMessage(server, receiveBuffer, sizeof receiveBuffer))
	  puts(receiveBuffer);
	else
	  puts("Receiving failed");
  }
  else
  {
	puts("Failed");
  }  

  closesocket(server);
  WSACleanup();
}