#include "keyhost.hpp"
#include "icryptosystem.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

void keyhost::putToServer (std::string username, ICryptosystem * key)
{
   struct addrinfo * host;
   getaddrinfo(hostname_.data(), port_.data(), nullptr, &host);
   int sock = socket(AF_INET, SOCK_STREAM, 0);
   
   // check connection
   if ( connect(sock, host->ai_addr, host->ai_addrlen) )
   {
      std::cerr << "Error connecting to host." << std::endl;
      return;
   }
   
   // write opcode
   write(sock, opcodes::put, sizeof(opcodes));
   // write username
   write(sock, htonl((int) username.size()), sizeof(int));
   write(sock, username.data(), username.size());
   // write public key
   std::string keyData = key->public_key_to_string();
   write(sock, htonl((int) keyData.size()), sizeof(int));
   write(sock, keyData.data(), keyData.size());
   
   // get error code
   errors errcode;
   read(sock, &errcode, sizeof(errors));
   if ( errcode != errors::noError )
   {
      std::cerr << "Server sent error code " << errcode << std::endl;
   }
   
   close(sock);
}

void keyhost::getFromServer (std::string username, ICryptosystem * key)
{
   struct addrinfo * host;
   getaddrinfo(hostname_.data(), port_.data(), nullptr, &host);
   int sock = socket(AF_INET, SOCK_STREAM, 0);
   
   // check connection
   if ( connect(sock, host->ai_addr, host->ai_addrlen) )
   {
      std::cerr << "Error connecting to host." << std::endl;
      return;
   }
   
   // write opcode
   write(sock, opcodes::get, sizeof(opcodes));
   // write username
   write(sock, htonl((int) username.size()), sizeof(int));
   write(sock, username.data(), username.size());
   
   // read error code
   errors errcode;
   read(sock, &errcode, sizeof(errors));
   // check error
   if ( errcode != errors::noError )
      std::cerr << "Server returned error code " << (int) errcode << std::endl;
      close(sock);
      return;
   }
   
   // read key
   int keySize;
   read(sock, &keySize, sizeof(int));
   keySize = ntohl(keySize);
   char * keyData = new char[keySize];
   read(sock, keyData, keySize);
   
   // store key data in ICryptosystem object
   key->public_key_from_string(std::string(keyData));
   
   close(sock);
   delete keyData;
}