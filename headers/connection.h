#include <message.h>
class Connection
{
	Connection(); //Constructor, Port must be set to 2001!
	Connection(int); //Constructor, set port to arg1
	void setPort(int);
	
	/*
		Also checks if the message is valid
		Blocking function
	*/	
	Message listen(void);

	void send(Message);
}
