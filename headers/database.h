#ifndef _DATABASE
#define _DATABASE 1

#include <string>

#define INDIRECT_CLIENT 1
#define DIRECT_CLIENT 2
#define SERVER 3

using namespace std;

typedef struct client_d{
  char* name;
  struct client_d *next,*prev;
  char authorisatie;
  unsigned short port;
  unsigned long ip;
  int ref;
  char pingtimeout;
}client_d;

typedef struct client_i{
  char* name;
  struct client_i *next,*prev;
  struct server *to;
}client_i;

//miss is het handig om hier later ook nog een lijst met clients aan toe te voegen.

typedef struct server{
  struct server *next,*prev;
  unsigned long ip;
  unsigned short port;
  unsigned int ref;
  char pingtimeout;
}server;

class Database {
private:
	client_i * begin_clients_indirect;
	client_i * end_clients_indirect;
	client_d * begin_clients_direct;
	client_d * end_clients_direct;
	server * begin_servers;
	server * end_servers;
public:
	int conClients;	
	Database();
	void* create_new_entry(int type);
	int insert(void* entry, int type);
	int replace(void* entry, int type);
	void* return_list(int type);
	int look_up_name(char* name, void* entry);
	int look_up_ip(unsigned long ip);
	int delete_entry(void* entry, int type);
	int look_up_direct_connect(unsigned long ip, unsigned short port, client_d* output);
	
};
#endif
