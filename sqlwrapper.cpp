#include "sqlwrapper.h"
#include <iostream>
#include <mysql++.h>

namespace sqlwrapper {

mysqlpp::Connection * sql_connect ()
{
   mysqlpp::Connection * conn = new mysqlpp::Connection(false);
   bool connected = conn->connect("enigma","aurum.fishsicles.net","enigma","password");
   
   if ( connected )
   {
      std::cout << "Database connection established" << std::endl;
      return conn;
   }
   else
   {
      std::cerr << "Database connection failed" << std::endl;
      return NULL;
   }
}

void sql_disconnect (mysqlpp::Connection * conn)
{
   delete conn;
}

std::string get (const std::string & username, mysqlpp::Connection * conn)
{
   mysqlpp::Query get = conn->query();
   get << "SELECT pubkey FROM keytable WHERE username='" << username << "'";
   
   mysqlpp::StoreQueryResult result = get.store();
   if ( get.result_empty() ) { return std::string(""); }
   
   mysqlpp::String pubkey = result[0]["pubkey"];
   std::string output;
   pubkey.to_string(output);
   return output;
}

void put (const std::string & username, const std::string & key, mysqlpp::Connection * conn)
{
   mysqlpp::Query put = conn->query();
   std::stringstream hexKey;
   for ( unsigned int i = 0; i < key.size(); ++i ) { hexKey << std::hex << (int)key[i]; }

   std::string keyDone = hexKey.str();
   put << "INSERT INTO keytable (username, pubkey) VALUES ('"
       << username << "', 0x" << keyDone << ") ON DUPLICATE KEY UPDATE pubkey=0x"
       << keyDone << ";";
   put.execute();
}

}
