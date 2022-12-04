#include <string>
#include <vector>
#include <regex>

#include "CommandProcess.h"

#include "SocketCommunication.h"

static std::vector<std::string> split(const std::string& input, const std::string& regex);
static int contains(std::string str, std::string substr);


void startClientCommandProcess(SOCKET server, int id)
{
  using namespace std;

  int run = 1;
  int commandId, commandSuccess;
  char receiveBuffer[1024];
  string received, reply;
  string command, commandArgs, success;
  static const regex commandRegex(R"([\w]+\$.+\$.+\r\n)");

  while (run)
  {
	if (receiveMessage(server, receiveBuffer, sizeof receiveBuffer) == 0)
	{
	  puts("Error while reading, quitting!");
	  exit(1);
	}
	received = std::string(receiveBuffer);
	
	printf("Received: %s\n", received.c_str());
	printf("Match: %i\n", regex_match(received, commandRegex));
	if (regex_match(received, commandRegex))
	{
	  vector<string> commandSplit = split(received, "\\$");
	  commandId = atoi(commandSplit[0].c_str());
	  command = commandSplit[1];
	  commandArgs = commandSplit[2].substr(0, commandSplit[2].length() - 2);

	  printf("Got command: id = %i, command = %s, args = %s\n", commandId, command.c_str(), commandArgs.c_str());
	  //TODO DO COMMAND
	  commandSuccess = 1;
	  reply = "C$" + to_string(id) +
		"$" + to_string(commandId) +
		"$" + (commandSuccess == 0 ? "failure" : "success");
	  sendMessage(server, reply.c_str());
	  printf("Send %s as a reply\n", reply.c_str());
	}
  }
}

static int contains(std::string str, std::string substr)
{
  return str.find(substr) != std::string::npos;
}


static std::vector<std::string> split(const std::string& input, const std::string& regex)
{
  // passing -1 as the submatch index parameter performs splitting
  std::regex re(regex);
  std::sregex_token_iterator
	first{ input.begin(), input.end(), re, -1 },
	last;
  return { first, last };
}