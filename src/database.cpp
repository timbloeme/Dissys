#include <cstdlib>
#include <cstdio>
#include <database.h>

using namespace std;


/** TO DO
 *  replace
 * 
 */


Database::Database(){
	begin_clients_indirect=NULL;
	end_clients_indirect=NULL;
	begin_clients_direct=NULL;
	end_clients_direct=NULL;
	begin_servers=NULL;
	end_servers=NULL;
}

void* create_new_entry(int type){
  switch(type){
    case INDIRECT_CLIENT:{
      client_i* output = calloc(1,sizeof(client_i));
      output->next=NULL;
      output->prev=NULL;
      output.authorisatie=0;
      output.socket=0;
    }break;
    
    case DIRECT_CLIENT:{
      client_d* output = calloc(1,sizeof(client_d));
      output->next=NULL;
      output->prev=NULL;
      output->to=NULL;
    }break;
    
    case SERVER:{
      server* output = calloc(1,sizeof(server));
      output->next=NULL;
      output->prev=NULL;
      output.ip=0;
      output.port=0;
      output.ref=0;
    }break;
    
    default:{
      return NULL;
    }break;
  }
  return output;
}

/*
 * checkt nu alleen op socket/ip miss ook wel handig om ook met ref te controlleren.
 * 
 */


int Database::insert(void* entry, int type){
  int i,error=0;
  void *temp;
  /*
   * 1 = indirectly connected clients
   * 2 = directly connected clients
   * 3 = indirectly connected servers
   * 4 = directly connected servers
   */
  switch(type){
    case INDIRECT_CLIENT:{
      if(end_clients_indirect==NULL){
	begin_clients_indirect = end_clients_direct = (client_i*)entry;
	return 1;
      }
      else{
	temp=begin_clients_indirect;
	while(temp->next != NULL){
	  if(strcmp((client_i*)temp.name,(client_i*)entry.name==0) return 0; 
	  temp=temp->next;
	}
	end_clients_indirect->next=(client_i*)entry;
	return 1;
      }
    }break;
    
    case DIRECT_CLIENT:{
      if(end_clients_direct==NULL){
	begin_clients_direct = end_clients_direct = (client_d*)entry;
	return 1;
      }
      else{
	temp=begin_clients_direct;
	while(temp->next != NULL){
	  if(strcmp((client_d*)temp.name,(client_d*)entry.name==0)return 0; 
	  temp=temp->next;
	}
	end_clients_direct->next=(client_i*)entry;
	return 1;
      }
    }break;
    
    case SERVER:{
      if(end_servers==NULL){
	begin_servers = end_servers = (server*)entry;
	return 1;
      }
      else{
	temp=begin_servers;
	while((server*)temp->next != NULL){
	  if(temp.ip == (server*)entry.ip) return 0;
	  temp=temp->next;
	}
	end_servers->next=(server*)entry;
	return 1;
      }
    }break;
    
    default:{
      return 0;
    }break;
  }
}

int delete_(void* entry, int type){
  void* temp;
  switch(type){
    case INDIRECT_CLIENT:{
      temp=begin_clients_indirect;
      while(temp->next != NULL){
        if(strcmp((client_i*)temp.name,(client_i*)entry.name==0){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 1;
	} 
        temp=temp->next;
      }  
      return 0;
    }break;
    
    case DIRECT_CLIENT:{
      temp=begin_clients_direct;
      while(temp->next != NULL){
        if(strcmp((client_d*)temp.name,(client_d*)entry.name==0){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 1;
	} 
        temp=temp->next;
      }  
      return 0;
    
    }break;
    
    case SERVER:{
      temp=begin_servers;
      while((server*)temp->next != NULL){
        if(temp.ip==(server*)entry.ip){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 1;
	} 
        temp=temp->next;
      }  
      return 0;
    
    }break;
    
    default:{
      return 0;
    }break;
  }

}


int replace(void* entry, int type){
  switch(type){
    case INDIRECT_CLIENT:{
      
    }break;
    case DIRECT_CLIENT:{
      
    }break;
    case SERVER:{
      
    }break;
    
    default:{
      return 0;
    }break;
  }
}

void* return_list(void* list, int type){
  switch(type){
    case INDIRECT_CLIENT:{
      return begin_clients_indirect;
    }break;
    case DIRECT_CLIENT:{
      return begin_clients_indirect;
    }break;
    case SERVER:{
      return servers;
    }break;
    
    default:{
      return 0;
    }break;
  }
}

int look_up_name(string* name){
  int output=0;
  void* temp=begin_clients_indirect;
  while(temp->next==NULL){
    if(strcmp((client_i*)temp.name,name==0)output++;
    temp=temp->next;
  }
  temp=begin_clients_direct;
  while(temp->next==NULL){
    if(strcmp((client_d*)temp.name,name==0)output++;
    temp=temp->next;
  }
  return output;
}

int look_up_ip(unsigned long ip){
  int output=0;
  server* temp=begin_servers;
  while(temp->next==NULL){
    if(temp.ip==ip)output++;
    temp=temp->next;
  }
  return output;
}