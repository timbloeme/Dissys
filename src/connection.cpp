#include <connection.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
extern "C" 
{
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

}

using namespace std;

Connection::Connection(unsigned short port = 2001)
{
	buffer = (char *)malloc(sizeof(char)*201);
	int yes = 1;
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(int));
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &sa.sin_addr);
	bind(sd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in));
}
	
void Connection::setDatabase(Database * db)
{
	database = db;
}

Message Connection::listen(void){
	Message message;
	string temp;
	struct sockaddr_in sender;
	socklen_t fromlen = sizeof(struct sockaddr_in);

	while (1) {
		uint16_t rlength = recvfrom(sd, buffer, sizeof(char)*200, 0, (struct sockaddr *)&sender, &fromlen);
		if (rlength < 6)
			continue;
		
		uint16_t length = ntohs(*(uint16_t *)buffer);
		uint16_t type = ntohs(*(uint16_t *)&buffer[2]);
		uint16_t refnum = ntohs(*(uint16_t *)&buffer[4]);

		if (rlength != length)
			continue;
		
		buffer[length] = '\0';
		char* t = &buffer[6];
		if (strlen(t) + 6 != length)
			continue;
		temp = (string)t;
		if (string::npos != 
		temp.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/? "))
			continue;
			
		message.setType(type);
		message.setReferenceNumber(refnum);
		message.setMessage(t);
		message.setSender(sender.sin_addr.s_addr, sender.sin_port);
		break;
	}
	return message;
}

void Connection::send(Message message)
{
	int sendtype, size, temp;
	char* recipient = message.getRecipients(&sendtype);
	void *entry;
	
	//Als de naam van degene die dit bericht heeft gestuurd niet bij mij voorkomt vertrouw ik het niet
	temp = database->look_up_name(recipient, (void*)entry);
	if(temp == 0) return;
	
	switch(sendtype) {
		case NONE:
			break;
		case ONE:
			if (temp == 1) {
				message.setReferenceNumber((((client_i*)entry)->to->ref)++);
				this->send(message, ((client_i*)entry)->to->ip, ((client_i*)entry)->to->port);
			}
			else if(temp == 2){
				message.setReferenceNumber((((client_d*)entry)->ref)++);
				this->send(message, ((client_d*)entry)->ip, ((client_d*)entry)->port);
			}
			break;
		case ALL:
			//INDIRECT CLIENT is niet nodig, aangezien die via een server berijkbaar zijn
			entry = database->return_list(DIRECT_CLIENT);
			while(((client_d*)entry)->next){
			  message.setReferenceNumber((((client_d*)entry)->ref)++);
			  this->send(message, ((client_d*)entry)->ip, ((client_d*)entry)->port);
			  entry = ((client_d*)entry)->next;
			}
			entry = database->return_list(SERVER);
			while(((server*)entry)->next){
			  message.setReferenceNumber((((server*)entry)->ref)++);
			  this->send(message, ((server*)entry)->ip, ((server*)entry)->port);
			  entry = ((server*)entry)->next;
			}
			/*entries  = database->allEntries(&size);
			for (int i = 0 ; i < size ; i++) {
				message.setReferenceNumber((*entries[i].ref)++);
				if (entries[i].directlyconnected)
					this->send(message, entries[i].ip, entries[i].port);
			}*/
			break;
		case ALLBUTONECLIENT:
			entry = database->return_list(DIRECT_CLIENT);
			while(((client_d*)entry)->next){
			  if(strcmp(((client_d*)entry)->name,recipient) != 0){
			    message.setReferenceNumber((((client_d*)entry)->ref)++);
			    this->send(message, ((client_d*)entry)->ip, ((client_d*)entry)->port);
			  }
			  entry = ((client_d*)entry)->next;
			}
			entry = database->return_list(SERVER);
			while(((server*)entry)->next){
			  message.setReferenceNumber((((server*)entry)->ref)++);
			  this->send(message, ((server*)entry)->ip, ((server*)entry)->port);
			  entry = ((server*)entry)->next;
			}
			/*entries  = database->allEntries(&size);
			for (int i = 0 ; i < size ; i++) {
				if (entries[i].name->compare(recipient) != 0) {
					message.setReferenceNumber((*entries[i].ref)++);
					if (entries[i].directlyconnected)
						this->send(message, entries[i].ip, entries[i].port);
				}
			}*/
			break;
		case ALLBUTONESERVER:
			entry = database->return_list(DIRECT_CLIENT);
			while(((client_d*)entry)->next){
			  message.setReferenceNumber((((client_d*)entry)->ref)++);
			  this->send(message, ((client_d*)entry)->ip, ((client_d*)entry)->port);
			  entry = ((client_d*)entry)->next;
			}
			entry = database->return_list(SERVER);
			while(((server*)entry)->next){
			  if((temp == 1 && ((server*)entry)->ip != ((client_i*)entry)->to->ip)
			      || (temp == 2 && ((server*)entry)->ip != ((client_d*)entry)->ip)){
			    message.setReferenceNumber((((server*)entry)->ref)++);
			    this->send(message, ((server*)entry)->ip, ((server*)entry)->port);
			    entry = ((server*)entry)->next;
			  }
			}
			/*if (database->lookup(recipient, &entry) != 0) {
				entries  = database->allEntries(&size);
				for (int i = 0 ; i < size ; i++) {
					if (!(entries[i].ip == entry.ip && entries[i].port == entry.port)) {
						message.setReferenceNumber((*entries[i].ref)++);
						if (entries[i].directlyconnected)
							this->send(message, entries[i].ip, entries[i].port);
					}
				}
			}*/
			break;
		default:
			break;
	}
}

void Connection::send(Message message, unsigned long ip, unsigned short port)
{
	struct sockaddr_in destination;
	destination.sin_addr.s_addr = ip;
	destination.sin_port = port;
	destination.sin_family = AF_INET;
	
	uint16_t length = message.length();
	uint16_t type = message.getType();
	uint16_t refnum = message.getReferenceNumber();

	buffer[0] = htons(length);
	buffer[1] = htons(length) >> 8;
	buffer[2] = htons(type);
	buffer[3] = htons(type) >> 8;
	buffer[4] = htons(refnum);
	buffer[5] = htons(refnum) >> 8;
	strcpy(&buffer[6], message.getMessage());
	sendto(sd, buffer, length, 0, (struct sockaddr *)&destination, sizeof(struct sockaddr_in));
}
