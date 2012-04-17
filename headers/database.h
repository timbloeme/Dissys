#ifndef _DATABASE
#define _DATABASE 1

#include <string>

#define INDIRECT_CLIENT 1
#define DIRECT_CLIENT 2
#define SERVER 3

using namespace std;

typedef struct{
  string* name;
  client_i *next,*prev;
  int authorisatie;
  unsigned short socket;
}client_d;

typedef struct{
  string* name;
  client_i *next,*prev;
  server *to;
}client_i;

//miss is het handig om hier later ook nog een lijst met clients aan toe te voegen.

typedef struct{
  server_i *next,*prev;
  unsigned long ip;
  unsigned short port;
  unsigned int ref;
}server;

class Database {
private:
	int nrentries, size;
	entry_t * entries;
public:
	int conClients;	
	Database();
	void* create_new_entry(int type)
	int insert(void* entry, int type);
	int replace(void* entry, int type);
<<<<<<< HEAD
	int return_list(void* list, int type);
	int look_up_name(string* name);
=======
	int return_list(void* entry, int type);
	int look_up_name(strin* koekje);
>>>>>>> db03f7290d99bce3a33fd5e520d3b866d138bb27
	int look_up_ip(unsigned long ip);
	void delete_(void* entry, int type);
	
};
#endif
