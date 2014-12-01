#include "opcodes.hpp"
#include "enigmaserver.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

int logError (string msg)
{
   time_t t = time(0);
   struct tm * now = localtime(&t);
   cerr << "ERROR [" << (now->tm_year + 1900) << '-' << (now->tm_mon + 1)
        << (now->tm_mday) << "  " << (now->tm_hour) << ":" << (now->tm_min)
        << "]: " << msg;
}

int handlePut (int sock)
{
   // read name
   int nameSize;
   read(sock, &nameSize, sizeof(int)); // get size of username in bytes
   nameSize = ntohl(nameSize); // convert to host order
   char * name = new char[nameSize];
   read(sock, name, nameSize); // read username into string
   
   int keySize;
   read(sock, &keySize, sizeof(int));
   keySize = ntohl(keySize); // convert to host order
   char * key = new char[nameSize];
   read(sock, key, keySize);
   
   errors errcode = errors::noError;
   // TODO: change this line to the appropriate put-to-database code
   // errcode = putToDatabase(name, key);
   
   // completed successfully; no other data expected back
   write(sock,errcode,sizeof(errors));
   
   delete name;
   delete key;
}

int handleGet (int sock)
{
   // read name
   int nameSize;
   read(sock, &nameSize, sizeof(int)); // get size of username in bytes
   nameSize = ntohl(nameSize); // convert to host order
   char * name = new char[nameSize];
   read(sock, name, nameSize); // read username into string
   
   errors errcode = errors::noError;
   // TODO: change this line to the appropriate get-from-database code
   string key/* = getFromDatabase(name,&errcode)*/;
   
   // send error code
   write(sock,errcode,sizeof(errors));
   // if successful, send data to client
   if ( errcode == errors::noError )
   {
      write(sock, htonl((int) key.size()), sizeof(int));
      write(sock, key.data(), key.size());
   }
   
   delete name;
}

int main (int argc, const char * argv[])
{
   int listener, conn, length;
   struct sockaddr_in s1, s2;
   
   listener = socket(AF_INET,SOCK_STREAM,0);
   
   bzero((char *) &s1, sizeof(s1));
   s1.sin_family = (short) AF_INET;
   s1.sin_addr.s_addr = htonl(INADDR_ANY);
   s1.sin_port = htons(23232);
   
   length = sizeof(s1);
   bind(listener, (struct sockaddr *) &s1, length);
   getsockname(listener, (struct sockaddr *) &s1, &length);
   listen(listener, 1);
   
   cout << "enigma keyserver running on port " << ntohs(s1.sin_port) << endl;
   
   length = sizeof(s2);
   while (1)
   {
      opcodes operation;
      pid_t child;
      
      conn = accept(listener,(struct sockaddr *) &s2, &length);
      child = fork();
      if ( child != 0 ) // main thread
      {
         // check error
         if ( child < 0 )
         {
            write(conn,errors::serverError,sizeof(errors));
            logError("Could not fork child process.");
         }
         // close the connection in main thread
         close(conn);
         // wait for next connection
         continue;
      }
      
      // child only
      close(listener);
      
      while ( read(conn,&operation,sizeof(opcodes)) )
      {
         switch (operation)
         {
         case opcode::put:
            handlePut(conn);
            break;
         case opcode::get:
            handleGet(conn); 
            break;
         default: // bad opcode
            write(conn,errors::badOpcode,sizeof(errors));
            logError("Bad operation code from client.");
            return 1;
         }
      }
      return 0;
   }
   // wait for children to finish
   while (wait() > 0) {}
   return 0;
}