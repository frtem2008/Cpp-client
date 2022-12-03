#include <fstream>
#include <vector>
#include <regex>
#include <string>

#include "Communication.h"
#include "Login.h"

static int readFromFile(const char* fileName, int* id);
static std::vector<std::string> split(const std::string& input, const std::string& regex);
static int generateLoginId(); 
static int fileAvailable(const char* fileName);

int login(SOCKET server, std::string root)
{
  int idToSend;
  int generate = 0;

  puts("login started...\n");

  if (fileAvailable(USER_ID_FILE_NAME)) //try to login with id from file
    if (readFromFile(USER_ID_FILE_NAME, &idToSend))
    {
      printf("Id read from file: %i, started logging in...\n", idToSend);
      std::string command = root + "$" + std::to_string(idToSend);
      sendMessage(server, command.c_str());
    }
    else
      puts("Id file exists, but reading id failed, generating new id..."), generate = 1;
  
  while (generate) //log in using new id
  {
    
  }
  puts("Login done");
  
  return 0;
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

static int fileAvailable(const char *fileName)
{
  std::ifstream reader(fileName);
  int available = reader.is_open();

  reader.close();
  return available;
}