#include <server.h>
#include <cstdio>
#include <cstdlib>

extern "C" {
#include <arpa/inet.h>
}

using namespace std;

Server::Server(unsigned short _port, unsigned long _csip, string _ident)
{
	port = _port;
	csip = _csip;
	ident = _ident;
	connection = new Connection (port);
	database = new Database();
	manager = new Manager();
	connection->setDatabase(database);
}
	// Must be format (username password);
void Server::addManager(string data)
{
	manager->setPassword(data.substr(0, data.find_first_of (' ')));
	manager->setName(data.substr(data.find_first_of (' ')+1, string::npos));
}

	//Main control loop
	//Returns when the server should exit
void Server::start(void)
{
	printf("Starting up: Our Tag is %s\n", ident.c_str());
	Message message;
	string buffer;
	char cbuffer[100];
	int type, temp, temp1, size, ref;
	//unsigned long ip;
	//unsigned short _port;
	entry_t entry, * entries;
	
	message.setType(601);
	message.setReferenceNumber(0);
	message.setMessage(ident);
	
	connection->send(message, csip, htons(2001));
	printf("601 - Server -> Control server: Request for parent Server\n");
			
	while(1) {
		message = connection->listen();
		type = message.getType();
		buffer = message.getMessage();
		ref = message.getReferenceNumber();
		switch (type) {
			case 100:
				printf("100 - client -> server (Received)\n");
				message.getSender(&entry.ip, &entry.port);
				entry.name = new string(buffer.substr(0, buffer.find_first_of(" ")));
				if (database->lookup(*entry.name, NULL)) {
					printf("510 - server -> client (Sent) - Registratie mislukt\n");
					message.setType(510);
					message.setReferenceNumber(0);
					message.setMessage("Name already exists");
					connection->send(message, entry.ip, entry.port);
					break;
				}
				//Must also send 110 
				printf("500 - server -> client (Sent) - Registratie gelukt\n");
				database->insert(entry);
				message.setType(500);
				message.setMessage("");
				message.setRecipients(*entry.name, ONE);
				connection->send(message);
				
				printf("110 - server -> All but client (Sent) - user added\n");
				message.setType(110);
				message.setRecipients(*entry.name, ALLBUTONE);
				sprintf(cbuffer, "%6d%6d%s", 1, 1, (*entry.name).c_str());
				buffer = cbuffer;
				message.setMessage(buffer);
				connection->send(message);
				
				printf("110 - server -> client (Sending) - user list\n");
				entries = database->allEntries(&size);
				for (int i = 0 ; i < size ; i++) {
						sprintf(cbuffer, "%6d%6d%s", i+1, size, (*entries[i].name).c_str());
						buffer = cbuffer;
						message.setMessage(buffer);
						message.setRecipients(*entry.name, ONE);
						connection->send(message);
				}			
				break;			
			case 110:
				printf("110 - server -> server (Received)\n");
				if (buffer.length() > 12) {
					entry.name = new string (buffer.substr(12));
					message.getSender(&entry.ip, &entry.port);
				
					database->insertReplace(entry);
				}
				if ( parentip == entry.ip && parentport == entry.port ) {
					printf("110 - server -> server (Sending to clients and non-parent servers)\n");
					message.setRecipients(parentname, ALLBUTONEADRESS);
					connection->send(message);
					
					if (!buffer.substr(0, 6).compare(buffer.substr(6, 6))) {
						printf("110 - server -> parent server (Sending)\n");
						entries = database->allEntries(&size);
						temp = 0, temp1 = 0;
						
						for (int i = 0 ; i < size ; i++) {
							if (!(entries[i].ip == parentip && entries[i].port == parentport))
								temp ++;
						}
						if (temp == 0) {
							sprintf(cbuffer, "%6d%6d", 0, 0);
							buffer = cbuffer;
							message.setMessage(buffer);
							message.setRecipients(parentname, ONE);
							connection->send(message);
						}
						for (int i = 0 ; i < size ; i++) {
							if (!(entries[i].ip == parentip && entries[i].port == parentport)) {
								sprintf(cbuffer, "%6d%6d%s", ++temp1, temp, (*entries[i].name).c_str());
								buffer = cbuffer;
								message.setMessage(buffer);
								message.setRecipients(parentname, ONE);
								connection->send(message);
							}
						}
					}
				}
				break;
			case 140:
				//printf("140 - ping -> 150 - pong\n"); Clutters the logs
				message.setType(150);
				message.setReferenceNumber(0);
				message.getSender(&entry.ip, &entry.port);
				connection->send(message, entry.ip, entry.port);
				break;
			case 200:
				printf("200 - client -> server (Received)\n");
				message.setType(300);
				
				if (buffer.substr(0, buffer.find_first_of (' ')) == "#all") {
					printf("300 -  server -> ALL (Sending)\n");
					message.setRecipients("#all", ALL);
					connection->send(message);
					break;
				}
				printf("300 -  server -> server/client (Sending)\n");
				message.setRecipients(buffer.substr(0, buffer.find_first_of (' ')), ONE);
				connection->send(message);
				break;				
			case 600:
				printf("600 - server -> server (Received)\n");
				
				//Insert Server child into database
				message.getSender(&entry.ip, &entry.port);
				entry.name = new string(buffer);
				database->insertReplace(entry);
				
				printf("110 - server -> server (Sending to new child)\n");
				entries = database->allEntries(&size);
				message.setType(110);
				message.setRecipients(*entry.name, ONE);
				for (int i = 0 ; i < size ; i++) {
					sprintf(cbuffer, "%6d%6d%s", i+1, size, (*entries[i].name).c_str());
					buffer = cbuffer;
					message.setMessage(buffer);
					connection->send(message);
				}
				break;
			case 602:
				printf("602 - Control server -> server (Received)\n");
				if (buffer.compare("none") == 0) {
					printf("602 - No parent received (Now root of network)\n");
					break;
				}
				
				//Insert future parent into database
				inet_pton(AF_INET,buffer.substr(0, (temp = buffer.find_first_of(':'))).c_str(), &entry.ip);
				parentip = entry.ip;
				entry.port = atoi( buffer.substr( temp+1, buffer.find_first_of(':',temp+1) ).c_str() );
				parentport = entry.port;
				temp = buffer.find_first_of(':',temp+1);
				entry.name = new string (buffer.substr(temp+1, string::npos));
				parentname = *entry.name;

				database->insert(entry);
								
				printf("600 - Server -> Server: Trying to register in network (Sending)");
				message.setType(600);
				message.setMessage(ident);
				message.setRecipients(*entry.name, ONE);
				connection->send(message);
				break;

			default:
				break;		
		}	
	}
}
	
void Server::stop(void){}
	

