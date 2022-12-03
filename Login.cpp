#include <fstream>
#include <vector>
#include <regex>
#include <string>

#include "Communication.h"
#include "Login.h"

static int fileAvailable(const char* fileName);
static int readFromFile(const char* fileName, int* id);
static int saveIdToFile(const char* fileName, int idToWrite);

static std::vector<std::string> split(const std::string& input, const std::string& regex);
static int contains(std::string str, std::string substr);

static int generateLoginId();

int login(SOCKET server, std::string root)
{
  int idToSend;
  int generate = 1;
  char received[1024];
  std::string command, reply;

  puts("login started...\n");

  if (fileAvailable(USER_ID_FILE_NAME)) //try to login with id from file
  {
	if (readFromFile(USER_ID_FILE_NAME, &idToSend))
	{
	  printf("Id read from file: %i, started logging in...\n", idToSend);
	  command = root + "$" + std::to_string(idToSend);
	  sendMessage(server, command.c_str());
	  receiveMessage(server, received, sizeof received);
	  reply = std::string(received);
	  if (contains(reply, "LOGIN$INVALID_ID$ONLINE"))
	  {
		printf("Id: %i is allready online WTF, quitting...\n", idToSend);
		return 0;
	  }
	  else if (contains(reply, "LOGIN$INVALID_ID$FREE"))
	  {
		printf("Id: %i is free WTF, quitting...\n", idToSend);
		return 0;
	  }
	  else if (contains(reply, "LOGIN$CONNECT$"))
	  {
		printf("Connected with id %i as %s\n", idToSend, root == "C" ? "Client" : "Admin");
		generate = false;
	  }
	  else
	  {
		puts("Something strange happened, quitting...\n");
		return 0;
	  }
	}
	else
	  puts("Id file exists, but reading id failed, generating new id..."), generate = 1;
  }
  
  while (generate) //log in using new id
  {
	idToSend = generateLoginId();
	printf("Generated id: %i, logging in...\n", -idToSend);
	command = root + "$" + std::to_string(-idToSend);
	sendMessage(server, command.c_str());

	receiveMessage(server, received, sizeof received);
	reply = std::string(received);

	if (contains(reply, "LOGIN$INVALID_ID$EXISTS"))
	  printf("Id: %i is allready registered, trying another one...\n", idToSend);
	else if (contains(reply, "LOGIN$INVALID_ID$ONLINE"))
	{
	  printf("Id: %i is allready online WTF, quitting...\n", idToSend);
	  return 0;
	}
	else if (contains(reply, "LOGIN$CONNECT$"))
	{
	  printf("Connected with id %i as %s\n", idToSend, root == "C" ? "Client" : "Admin");
	  generate = false;
	}
	else
	{
	  puts("Something strange happened, quitting...\n");
	  return 0;
	}
  }

  if (saveIdToFile(USER_ID_FILE_NAME, idToSend))
	puts("Generated id saved to file\n");
  else
	puts("Failed to save generated id to file\n");

  puts("Login done");

  return 1;
}

static int contains(std::string str, std::string substr)
{
  return str.find(substr) != std::string::npos;
}

static int readFromFile(const char* fileName, int* id)
{
  char line[32];
  std::ifstream reader(fileName);

  if (!reader.is_open())
	return 0;

  reader.getline(line, sizeof line);
  reader.close();

  std::string str(line);
  if (str.empty())
	return 0;

  *id = atoi(line);
  return 1;
}

static int saveIdToFile(const char *fileName, int idToWrite)
{
  if (fileAvailable(fileName))
  {
	std::ofstream output(fileName);
	output << idToWrite << '\n';
	output.close();
	return 1;
  }
  return 0;
}


static std::vector<std::string> split(const std::string& input, const std::string& regex) {
  // passing -1 as the submatch index parameter performs splitting
  std::regex re(regex);
  std::sregex_token_iterator
	first{ input.begin(), input.end(), re, -1 },
	last;
  return { first, last };
}

static int generateLoginId()
{
  return rand() % 32767 + 1; //from 1 to 32768
}

static int fileAvailable(const char* fileName)
{
  std::ifstream reader(fileName);
  if (reader.is_open())
  {
	reader.close();
	return 1;
  }

  std::ofstream writer(fileName);
  int available = writer.is_open();

  reader.close();
  return available;
}