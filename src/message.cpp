#include <message.h>

using namespace std;
 
Message::Message(void){}

void Message::setType(int){}
int Message::getType(void){return 1;}

void Message::setMessage(string){}
string Message::getMessage(void){return data;}

int Message::length(void){return 1;}

void Message::setReferenceNumber(int){}
int Message::getReferenceNumber(void){return 1;}
	

void Message::setRecipients(string){}
string Message::getRecipients(void){return data;}

unsigned long Message::getSenderIp(void){return 1;}
unsigned short Message::getSenderPort(void){return 1;}

void Message::setSender(unsigned long, unsigned short){}
