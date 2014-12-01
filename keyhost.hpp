#ifndef KEYCLIENT_HPP
#define KEYCLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class keyhost
{
   keyhost(const std::string & hostname, const std::string & port = defaultPort)
      { getaddrinfo(hostname.data(), port.data(), nullptr, &host); }
public:
   ~keyhost () { freeaddrinfo(info); }
   
   void putToServer (std::string username, ICryptosystem * key);
   void getFromServer (std::string username, ICryptosystem * key);
private:
   const static std::string defaultPort = "23232";
   struct addrinfo * host;
};

#endif