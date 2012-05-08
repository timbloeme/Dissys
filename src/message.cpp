#include <message.h>
#include <string.h>

using namespace std;
 
Message::Message(void){
	mtype = NONE;
	mdata = "";
	msize = 0;
	mrefnum = 0;
	mip = 0;
	mport = 0;
	mrecipient = "";
	mrecvtype = 0;
}

void Message::setType(int type)
{
	mtype = type;
}
int Message::getType(void)
{
	return mtype;
}

void Message::setMessage(char* data)
{
	string temp;
	mdata = data;
	temp = (string)data;
	
	if (string::npos != 
		temp.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/?"))
		//Print warning here, be robust, keep the wheels turning!
	
	if (strlen(mdata)+6 > 200)
		/**Als we de data caloccen of maloccen dan kunnen we realloc doen */
		//mdata.resize(194);
	msize = strlen(mdata)+6;
}

char* Message::getMessage(void)
{
	return mdata;
}

int Message::length(void)
{
return msize;
}

void Message::setReferenceNumber(int refnum)
{
	mrefnum = refnum;
}

int Message::getReferenceNumber(void)
{
	return mrefnum;
}
	
void Message::setRecipients(char* recipient, int type)
{
	mrecipient = recipient;
	mrecvtype = type;
}

char* Message::getRecipients(int * recvtype)
{
	* recvtype = mrecvtype;
	return mrecipient;
}

void Message::getSender(unsigned long * ip, unsigned short * port )
{
	* ip = mip;
	* port = mport;
}

void Message::setSender(unsigned long ip, unsigned short port)
{
	mip = ip;
	mport = port;
}
