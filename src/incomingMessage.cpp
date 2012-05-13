#include <string.h>

void Server::incomingMessage(Message  message) {

	int type, temp, temp1, size, ref;
	unsigned long temp_ip;
	unsigned short temp_port;
	char *buffer, *name, *str1, *str2;
	char cbuffer[100];
	void *entry, *list;

	type = message.getType();
	buffer = message.getMessage();
	ref = message.getReferenceNumber();
	message.getSender(&temp_ip, &temp_port);
	switch (type) {
		case 100:
			printf("100 - client -> server (Received)\n");
			//name = new string(buffer.substr(0, buffer.find_first_of(" ")));
			name = strtok(buffer, " ");
			if (database->look_up_name(name, (void*)entry) > 0) {
				printf("510 - server -> client (Sent) - Registratie mislukt\n");
				message.setType(510);
				message.setReferenceNumber(0);
				message.setMessage("Name already exists");
				connection->send(message, temp_ip, temp_port);
				break;
			}
			printf("500 - server -> client (Sent) - Registratie gelukt\n");
			/*
			entry.directlyconnected = 1;
			entry.isClient = 1;
			database->conClients++;
			database->insertReplaceWithIp(entry);
			*/
			entry = database->create_new_entry(DIRECT_CLIENT);
			((client_d*)entry)->name = name;
			((client_d*)entry)->ip   = temp_ip;
			((client_d*)entry)->port = temp_port;
			database->insert(entry, DIRECT_CLIENT);
			message.setType(500);
			message.setMessage("");
			message.setRecipients(name, ONE);
			connection->send(message);
			printf("110 - server -> All but client (Sent) - user added\n");
			message.setType(110);
			message.setRecipients(name, ALLBUTONECLIENT);
			sprintf(cbuffer, "%6d%6d%s", 1, 1, name);
			message.setMessage(buffer);
			connection->send(message);
			
			printf("110 - server -> client (Sending) - user list\n");
			list = database->return_list(DIRECT_CLIENT);
			while(((client_d*)list)->next){
			  message.setReferenceNumber((((client_d*)list)->ref)++);
			  connection->send(message, ((client_d*)list)->ip, ((client_d*)list)->port);
			  list = ((client_d*)list)->next;
			}
			list = database->return_list(SERVER);
			while(((server*)list)->next){
			  message.setReferenceNumber((((server*)list)->ref)++);
			  connection->send(message, ((server*)list)->ip, ((server*)list)->port);
			  list = ((server*)list)->next;
			}
			/*
			entries = database->allEntries(&size);
			for (int i = 0 ; i < size ; i++) {
					sprintf(cbuffer, "%6d%6d%s", i+1, size, (*entries[i].name).c_str());
					buffer = cbuffer;
					message.setMessage(buffer);
					message.setRecipients(*entry.name, ONE);
					connection->send(message);
			}*/			
			break;			
		case 110:
			printf("110 - server -> server (Received)\n");
			if (strlen(buffer) > 12) {
				//entry.name = new string (buffer.substr(12));
				for(int i=0; i<12; i++) name[i] = buffer[i];
				message.getSender(&temp_ip, &temp_port);
				entry = database->create_new_entry(INDIRECT_CLIENT);
				((client_d*)entry)->name = name;
				((client_d*)entry)->ip   = temp_ip;
				((client_d*)entry)->port = temp_port;
				database->insert(entry, INDIRECT_CLIENT);
			}
			if ( parentip == temp_ip && parentport == temp_port ) {
				printf("110 - server -> server (Sending to clients and non-parent servers)\n");
				message.setRecipients(&parentname, ALLBUTONESERVER);
				connection->send(message);
				strncpy(str1,  buffer,     6);
				strncpy(str2, &buffer[6], 6);
				if(!strcmp(str1, str2)){
				//if (!buffer.substr(0, 6).compare(buffer.substr(6, 6))) {
					printf("110 - server -> parent server (Sending)\n");
					
					//entries = database->allEntries(&size);
					temp = 0, temp1 = 0;
					/*
					for (int i = 0 ; i < size ; i++) {
						if (!(entries[i].ip == parentip && entries[i].port == parentport))
							temp ++;
					}
					if (temp == 0) {*/
					if(database->look_up_ip(temp_ip) == 0){
						sprintf(cbuffer, "%6d%6d", 0, 0);
						buffer = cbuffer;
						message.setMessage(buffer);
						message.setRecipients(&parentname, ONE);
						connection->send(message);
					}
					entry = database->return_list(DIRECT_CLIENT);
					///MOET IK NOG DOEN
					/*
					for (int i = 0 ; i < size ; i++) {
						if (!(entries[i].ip == parentip && entries[i].port == parentport)) {
							if (entries[i].isClient) {
								sprintf(cbuffer, "%6d%6d%s", ++temp1, temp, (*entries[i].name).c_str());
								buffer = cbuffer;
								message.setMessage(buffer);
								message.setRecipients(parentname, ONE);
								connection->send(message);
							}
						}
					}*/
				}
			}
			break;
		case 120:
			printf("120 - client -> server (Received)\n");
			if (! database->look_up_direct(temp_ip, temp_port, (client_d*)entry)) {
				break;	
			}
			str1 = ((client_d*)entry)->name;
                        strcat(str1, " ");
                        strcat(str1, buffer);
			message.setMessage(str1);
			message.setType(130);
			message.setRecipients("#all", ALL);
			connection->send(message);
			database->delete_entry(entry, DIRECT_CLIENT);	
			printf("130 - server -> All (Sent)\n");
		case 130:
			printf("130 - server -> server (Received)\n");
			if (!database->look_up_ip(temp_ip));
				break;
			name = strtok(buffer," ");
			if (database->look_up_name(name, (void*)entry) {
				message.setRecipients(name, ALLBUTONESERVER);
				connection->send(message);
				printf("130 - server -> All (Sent)\n");
				database->delete_entry((void*)entry, INDIRECT_CLIENT);
			}
			break;
		case 140:
			//printf("140 - ping -> 150 - pong\n"); Clutters the logs
			message.setType(150);
			message.setReferenceNumber(csref++);
			connection->send(message, csip, htons(2001));
			break;
		case 160:
			printf("160 - client -> server (Received)\n");
			if (!database->look_up_direct(temp_ip, temp_port, &entry))
				break;
			if (database->look_up_name(buffer, (void*)entry)) {
				printf("530 - server -> client (Sent)\n");
				message.setType(530);
				message.setRecipients(((client_d*)entry)->name, ONE);
				message.setMessage("Name already taken");
				connection->send(message);
				break;
			}
			buffer = buffer + " " + (*client_d)entry->name;
			//buffer.insert(0, " ").insert(0, *entry.name);
			entry = database->create_new_entry(DIRECT_CLIENT);
			((client_d*)entry)->name = message.getMessage();
			/*
			entry.name = message.getMessage();
			entry.directlyconnected = 1;
			entry.isClient = 1;
			database->insertReplaceWithIp(entry);
			*/
			printf("520 - server -> client (Sent)\n");
			message.setType(520);
			message.setRecipients(((*client_d)entry)->name, ONE);
			message.setMessage("");
			connection->send(message);
			printf("170 - server -> All (Sent)\n");
			message.setType(170);
			message.setRecipients(((*client_d)entry)->name, ALL);
			message.setMessage(buffer);
			connection->send(message);
			break;
		case 170:
			printf("170 - server -> server (Received)\n");
			if (!database->look_up_direct(temp_ip, temp_port, (void*)entry))
				break;
			if (!database->look_up_name(strtok(buffer, " "), (void*)entry))
				break;
			((client_d*)entry)->name = strtok(NULL, " \n");
			((client_d*)entry)->ref  = 0;
			message.setRecipients(((client_d*)entry)->name, ALLBUTONESERVER);
			connection->send(message);
			printf("170 - server -> All (Sent)\n");
			break;
		case 200:
			printf("200 - client -> server (Received)\n");
			message.setType(300);
			if (! database->look_up_direct(temp_ip, temp_port, (void*)entry)) {
				break;	
			}
			buffer = buffer + " " + ((client_d*)entry)->name;
			//buffer.insert(0, " ").insert(0, *entry.name);
			message.setMessage(buffer);
			temp = strtok(buffer, " ")+1;
			//temp = buffer.find_first_of(' ')+1;
			///kijken wat dit betekent
			if (buffer.substr(temp, buffer.find_first_of (' ', temp+1)-temp) == "#all") {
				printf("300 -  server -> ALL (Sending)\n");
				message.setRecipients("#all", ALL);
				connection->send(message);
				break;
			}
			printf("300 -  server -> server/client (Sending)\n");
			temp = strtok(buffer, " ")+1;
			//temp = buffer.find_first_of(' ')+1;
			///weer zoiets vaags
			message.setRecipients(buffer.substr(temp, buffer.find_first_of(' ', temp)-temp), ONE);
			connection->send(message);
			message.setRecipients(((client_d*)entry)->name, ONE);
			connection->send(message);
			break;				
		case 600:
			printf("600 - server -> server (Received)\n");
			
			//Insert Server child into database
			entry = database->create_new_entry(SERVER);
			((server*)entry)->name = "";
			((server*)entry)->ip   = temp_ip;
			((server*)entry)->port = temp_port;
			database->insert((void*)entry, SERVER);
			/*
			entry.name = new string(buffer);
			entry.directlyconnected = 1;
			entry.isClient = 0;
			database->insertReplaceWithIp(entry);
			*/
			
			printf("110 - server -> server (Sending to new child)\n");
			///MOET NOG VERANDERD WORDEN
			entries = database->allEntries(&size);
			message.setType(110);
			message.setRecipients(*entry.name, ONE);
			for (int i = 0 ; i < size ; i++) {
				if (entries[i].isClient) {
					sprintf(cbuffer, "%6d%6d%s", i+1, size, (*entries[i].name).c_str());
					buffer = cbuffer;
					message.setMessage(buffer);
					connection->send(message);
				}
			}
			break;
		case 602:
			printf("602 - Control server -> server (Received)\n");
			if (buffer.compare("none") == 0) {
				printf("602 - No parent received (Now root of network)\n");
				break;
			}
			
			//Insert future parent into database
			inet_pton(AF_INET, strtok(buffer, ":"), &temp_ip);
			//inet_pton(AF_INET,buffer.substr(0, (temp = buffer.find_first_of(':'))).c_str(), &entry.ip);
			parentip = temp_ip;
			entry.port = htons(atoi( buffer.substr( temp+1, buffer.find_first_of(':',temp+1) - temp -1 ).c_str() ));
			parentport = entry.port;
			temp = buffer.find_first_of(':',temp+1);
			entry.name = new string (buffer.substr(temp+1, string::npos));
			parentname = *entry.name;
			entry.directlyconnected = 1;
			entry.isClient = 0;
			database->insert(entry);
							
			printf("600 - Server -> Server: Trying to register in network (Sending)");
			fflush(stdout);
			message.setType(600);
			message.setMessage(ident);
			message.setRecipients(*entry.name, ONE);
			connection->send(message);
			break;

		default:
			break;		
	}
}	
