#include <string>
using namespace std;

typedef struct entry {
	string name;
	int ip;
	int port;	
}


class Database {
	
	Database();
	
	void insert(entry);
	entry lookup(string);
	void delete_(string);

}
