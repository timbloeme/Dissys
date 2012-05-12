#include <cstdlib>
#include <cstdio>
#include <database.h>
#include <cstring>

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

void* Database::create_new_entry(int type){
  void* output;
  switch(type){
    case INDIRECT_CLIENT:{
      output = calloc(1,sizeof(client_i));
    }break;
    
    case DIRECT_CLIENT:{
      output = calloc(1,sizeof(client_d));
    }break;
    
    case SERVER:{
      output = calloc(1,sizeof(server));
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
  void* temp;
   /*
   * 1 = indirectly connected clients
   * 2 = directly connected clients
   * 3 = indirectly connected servers
   * 4 = directly connected servers
   */
  switch(type){
    case INDIRECT_CLIENT:{
      if(end_clients_indirect==NULL){
	begin_clients_indirect = end_clients_indirect = (client_i*)entry;
	return 1;
      }
      else{
	temp=begin_clients_indirect;
	while(((client_i*)temp)->next != NULL){
	  if(strcmp(((client_i*)temp)->name,((client_i*)entry)->name)==0) return 0; 
	  temp=((client_i*)temp)->next;
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
	while(((client_d*)temp)->next != NULL){
	  if(strcmp(((client_d*)temp)->name,((client_d*)entry)->name)==0)return 0; 
	  temp=((client_d*)temp)->next;
	}
	end_clients_direct->next=(client_d*)entry;
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
	while(((server*)temp)->next != NULL){
	  if(((server*)temp)->ip == ((server*)entry)->ip) return 0;
	  temp=((server*)temp)->next;
	}
	end_servers->next=(server*)entry;
	return 1;
      }
    }break;
    
    default:{
      return 0;
    }
  }
}

int Database::delete_entry(void* entry, int type){
  void* temp;
  switch(type){
    case INDIRECT_CLIENT:{
      temp=begin_clients_indirect;
      while(((client_i*)temp)->next != NULL){
        if(strcmp(((client_i*)temp)->name,((client_i*)entry)->name)==0){
	  ((client_i*)temp)->prev->next=((client_i*)temp)->next;
	  ((client_i*)temp)->next->prev=((client_i*)temp)->prev;
	  delete (client_i*)temp;
	  return 1;
	} 
        temp=((client_i*)temp)->next;
      }  
      return 0;
    }break;
    
    case DIRECT_CLIENT:{
      temp=begin_clients_direct;
      while(((client_d*)temp)->next != NULL){
        if(strcmp(((client_d*)temp)->name,((client_d*)entry)->name)==0){
	  ((client_d*)temp)->prev->next=((client_d*)temp)->next;
	  ((client_d*)temp)->next->prev=((client_d*)temp)->prev;
	  delete (client_d*)temp;
	  return 1;
	} 
        temp=((client_d*)temp)->next;
      }  
      return 0;
    
    }break;
    
    case SERVER:{
      temp=begin_servers;
      while(((server*)temp)->next != NULL){
        if(((server*)temp)->ip==((server*)entry)->ip){
	  ((server*)temp)->prev->next=((server*)temp)->next;
	  ((server*)temp)->next->prev=((server*)temp)->prev;
	  delete (server*)temp;
	  return 1;
	} 
        temp=((server*)temp)->next;
      }  
      return 0;
    
    }break;
    
    default:{
      return 0;
    }break;
  }

}


int Database::replace(void* entry, int type){
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
  return 0;
}

void* Database::return_list(int type){
  switch(type){
    case INDIRECT_CLIENT:{
      return begin_clients_indirect;
    }break;
    case DIRECT_CLIENT:{
      return begin_clients_direct;
    }break;
    case SERVER:{
      return begin_servers;
    }break;
    
    default:{
      return 0;
    }break;
  }
}

int Database::look_up_name(char* name, void* entry){
  int output=0;
  void* temp=begin_clients_indirect;
  while(((client_i*)temp)->next==NULL){
    if(strcmp(((client_i*)temp)->name,name)==0){
      output = 1;
      entry = temp;
    }
    temp=((client_i*)temp)->next;
  }
  temp=begin_clients_direct;
  while(((client_d*)temp)->next==NULL){
    if(strcmp(((client_d*)temp)->name,name)==0){
      output = 2;
      entry = temp;
    }
    temp=((client_d*)temp)->next;
  }
  return output;
}

int Database::look_up_ip(unsigned long ip, void *entry){
  int output=0;
  server* temp=begin_servers;
  while(temp->next==NULL){
    if(temp->ip==ip)output++;
    temp=temp->next;
  }
  return output;
}

