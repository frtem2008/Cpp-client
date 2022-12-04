#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "SocketCommunication.h"

int connectToServer(const char* address, int port, WSADATA* wsa, SOCKET* server, SOCKADDR_IN* serverAddress)
{
  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), wsa) != 0)
  {
	printf("Failed. Error Code : %d", WSAGetLastError());
	return 0;
  }

  printf("Initialised.\n");

  if ((*server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
	printf("Could not create socket : %d", WSAGetLastError());
	return 0;
  }

  printf("Socket created.\n");
  serverAddress->sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddress->sin_family = AF_INET;
  serverAddress->sin_port = htons(26780);

  if (connect(*server, (struct sockaddr*)serverAddress, sizeof(*serverAddress)) < 0)
  {
	puts("Connection error");
	return 0;
  }
  return 1;
}

int sendMessage(SOCKET s, const char* message)
{
  char* toSend;
  if (message[strlen(message) - 1] != '\n')
  {
	toSend = new char[strlen(message) + 2];
	strncpy(toSend, message, strlen(message));
	toSend[strlen(message)] = '\n';
	toSend[strlen(message) + 1] = '\0';
  }
  else
  {
	toSend = new char[strlen(message) + 1];
	strncpy(toSend, message, strlen(message));
	toSend[strlen(message)] = '\0';
  }

  if (send(s, toSend, strlen(toSend), 0) < 0)
	return 0;

  return 1;
}

int receiveMessage(SOCKET s, char* receiveTo, int maxReceiveLength)
{
  int recv_size;

  recv_size = recv(s, receiveTo, maxReceiveLength, 0);

  if (recv_size == SOCKET_ERROR || recv_size == 0)
	return 0;
  printf("RECV SIZE: %i\n", recv_size);

  receiveTo[recv_size] = '\0';
  return 1;
}

