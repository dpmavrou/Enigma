#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/PartSource.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>

/**
 * File: email.cpp
 * Author: Leopold Eckert
 * This file is supposed to login to a SMTP server and send encrypted files as attachments.
 */

using namespace Poco::Net;

/**
 * Function: openConnection
 * This function opens a login to a provided SMTP server, using a provided object.
 * Parameters:
 *    hostname: string containing the host.
 *    user: string containing the username.
 *    pass: string containing the password.
 *    client: pointer to a SecureSMTPClientSession object. After the function is called,
 *       it will contain the session to the hostname.
 */
void openConnection(std::string hostname, std::string user, std::string pass, SecureSMTPClientSession *client) {
	client->login(hostname);
	if(!client->startTLS()) {
		//Fail here
		return;
	}
	client->login(hostname, Poco::Net::SMTPClientSession::AUTH_LOGIN, user, pass);
}// end of function: openConnection

/**
 * Function: openConnection
 * This function opens a logins to a provided SMTP server by creating a new object.
 * Parameters:
 *    hostname: string containing the host.
 *    user: string containing the username.
 *    pass: string containing the password.
 * Return:
 *    A SecureSMTPClientSession logged into the desired host.
 */
SecureSMTPClientSession openConnection(std::string hostname, std::string user, std::string pass) {
	SecureSMTPClientSession client(hostname);
	client.login();
	if(!client.startTLS()) {
		//Fail here
		return client;
	}
	client.login(hostname, Poco::Net::SMTPClientSession::AUTH_LOGIN, user, pass);
	return client;
}// end of function: openConnection

/**
 * Function: sendFile
 * This function sends a file as an attachment to someone.
 * Parameters:
 *    path: path to file.
 *    recipientString: email address of the recipient, as a string.
 *    client: pointer to a client session with which to send the email.
 */
void sendFile(std::string path, std::string recipientString, SecureSMTPClientSession *client) {
	FilePartSource attachment(path);
	MailMessage mail;
	mail.addAttachment("Encrypted File", &attachment);
	MailRecipient recipient(MailRecipient::PRIMARY_RECIPIENT, recipientString);
	mail.addRecipient(recipient);
	client->sendMessage(mail);
}// end of function: sendFile
// end of file: email.cpp
