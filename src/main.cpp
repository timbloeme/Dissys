#include <iostream>
#include <string>
#include <server.h>
#include <cstdio>

extern "C" 
{
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}

using namespace std;
 
int main(void)
{   
    struct sockaddr_in sa;
    inet_pton(AF_INET, "192.168.16.101", &sa.sin_addr);
    string a = "S007S007123436789012345678901234";
    Server Server (2003, sa.sin_addr.s_addr, a);
    Server.start();
    return 0;     
}
