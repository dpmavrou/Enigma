#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H
#include <string>

namespace mysqlpp { class Connection; }

namespace sqlwrapper {

mysqlpp::Connection * sql_connect ();
void sql_disconnect (mysqlpp::Connection * conn);
std::string get (const std::string & username, mysqlpp::Connection * conn);
void put (const std::string & username, const std::string & key, mysqlpp::Connection * conn);

}

#endif
