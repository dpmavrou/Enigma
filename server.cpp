#include "opcodes.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>

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
   write(sock,&errcode,sizeof(errors));
   
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
   write(sock,&errcode,sizeof(errors));
   // if successful, send data to client
   if ( errcode == errors::noError )
   {
      int keysize = htonl(key.size());
      write(sock, &keysize, sizeof(int));
      write(sock, key.data(), key.size());
   }
   
   delete name;
}

int main (int argc, const char * argv[])
{
   int listener, conn;
   struct sockaddr_in s1, s2;
   
   listener = socket(AF_INET,SOCK_STREAM,0);
   
   memset((char *) &s1, 0, sizeof(s1));
   s1.sin_family = (short) AF_INET;
   s1.sin_addr.s_addr = htonl(INADDR_ANY);
   s1.sin_port = htons(23232);
   
   socklen_t length = sizeof(s1);
   bind(listener, (struct sockaddr *) &s1, length);
   getsockname(listener, (struct sockaddr *) &s1, &length);
   listen(listener, 1);
   
   cout << "enigma keyserver running on port " << ntohs(s1.sin_port) << endl;
   
   length = sizeof(s2);
   while (1)
   {
      opcodes operation;
      pid_t child;
      errors errcode;
      
      conn = accept(listener,(struct sockaddr *) &s2, &length);
      child = fork();
      if ( child != 0 ) // main thread
      {
         // check error
         if ( child < 0 )
         {
            errcode = errors::serverError;
            write(conn,&errcode,sizeof(errors));
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
         case opcodes::put:
            handlePut(conn);
            break;
         case opcodes::get:
            handleGet(conn); 
            break;
         default: // bad opcode
            errcode = errors::badOpcode;
            write(conn,&errcode,sizeof(errors));
            logError("Bad operation code from client.");
            return 1;
         }
      }
      return 0;
   }
   // wait for children to finish
   pid_t pid;
   while (pid = waitpid(-1, NULL, 0)) { if (errno == ECHILD) { break; } }
   return 0;
}
