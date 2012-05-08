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
<<<<<<< HEAD
	begin_servers=NULL;
	end_servers=NULL;
=======
	begin_servers_direct=NULL;
	end_servers_direct=NULL;
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
}

void* create_new_entry(int type){
  switch(type){
<<<<<<< HEAD
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
    
=======
    case 1:{
      client_i* output = (client_i*)calloc(1,sizeof(client_i));
   	  return output;
    }break;
    
    case 2:{
      client_d* output = (client_d*)calloc(1,sizeof(client_d));
      return output;
    }break;
    
    case 3:{
      server* output = (server*)calloc(1,sizeof(server));
      return output;
    }break;
 
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
    default:{
      return NULL;
    }break;
  }
}

/*
 * checkt nu alleen op socket/ip miss ook wel handig om ook met ref te controlleren.
 * 
 */


int Database::insert(void* entry, int type){
  int i,error=0;
   /*
   * 1 = indirectly connected clients
   * 2 = directly connected clients
   * 3 = indirectly connected servers
   * 4 = directly connected servers
   */
  switch(type){
<<<<<<< HEAD
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
=======
    case 1:{
      client_i * temp;
      if(end_clients_indirect==NULL){
	      begin_clients_indirect = end_clients_indirect = (client_i*)entry;
	      return 0;
      }
      else{
	      temp=begin_clients_indirect;
	      while(temp->next != NULL){
//TODO    if(strcmp(temp->name,((client_i*)entry)->name)==0) return 1; 
	        temp=temp->next;
    	  }
	      end_clients_indirect->next=(client_i*)entry;
	      return 0;
      }
    }break;
    
    case 2:{
      client_d * temp;
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
      if(end_clients_direct==NULL){
	begin_clients_direct = end_clients_direct = (client_d*)entry;
	return 1;
      }
      else{
	temp=begin_clients_direct;
	while(temp->next != NULL){
<<<<<<< HEAD
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
=======
//TODO	  if(strcmp(temp.name,(client_d*)entry.name==0)return 1; 
	  temp=temp->next;
	}
	end_clients_direct->next=(client_d*)entry;
	return 0;
      }
    }break;

    case 3:{
      server * temp;
      if(end_servers_direct==NULL){
	begin_servers_direct = end_servers_direct = (server*)entry;
	return 0;
      }
      else{
	temp=begin_servers_direct;
	while(temp->next != NULL){
//TODO	  if(temp.socket == (server*)entry.socket) return 1;
	  temp=temp->next;
	}
	end_servers_direct->next=(server*)entry;
	return 0;
      }
    }break;
    
    default: {
      return 1;
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
    }break;
  }
}

<<<<<<< HEAD
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
=======
int Database::delete_(void* entry, int type){
  switch(type){
    case 1:{
            client_i * temp;
      temp=begin_clients_indirect;
      while(temp->next != NULL){
//TODO        if(strcmp(temp.name,(client_i*)entry.name==0){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 0;
//	} 
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
        temp=temp->next;
      }  
      return 0;
    }break;
    
<<<<<<< HEAD
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
=======
    case 2:{
      client_d * temp;
      temp=begin_clients_direct;
      while(temp->next != NULL){
//TODO        if(strcmp(temp.name,(client_d*)entry.name==0){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 0;
//	} 
        temp=temp->next;
      }  
      return 1;
    
    }break;
        
    case 3:{
            server * temp;
      temp=begin_servers_direct;
      while(temp->next != NULL){
//TODO        if(temp.socket==(server_d*)entry.socket){
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  delete temp;
	  return 0;
//	} 
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
        temp=temp->next;
      }  
      return 0;
    
    }break;
    
    default:{
<<<<<<< HEAD
      return 0;
=======
      return 1;
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
    }break;
  }

}

int Database::replace(void* entry, int type) {
return 0;
}
int Database::return_list(void* entry, int type) {
return 0;
}
int Database::look_up_name(string* koekje) {
return 0;
}
int Database::look_up_ip(unsigned long ip) {
return 0;
}


<<<<<<< HEAD
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
=======
//hieronder nog vervangen


/*
void Database::insertReplace(entry_t entry)
{
	entry_t entry1;
	if (this->lookup(*entry.name, &entry1)){
		this->delete_(*entry1.name);
	}	
	entry.ref = (unsigned int *)malloc(sizeof(int));
	*entry.ref = 0;
	if (size == nrentries)
		entries = (entry_t *)realloc(entries, sizeof(entry_t) * (size *= 2));
	entries[nrentries++] = entry;
}

void Database::insertReplaceWithIp(entry_t entry)
{
	entry_t entry1;
	if (this->lookup(*entry.name, &entry1) || this->lookup(entry.ip, entry.port, &entry1)){
		this->delete_(*entry1.name);
	}	
	entry.ref = (unsigned int *)malloc(sizeof(int));
	*entry.ref = 0;
	if (size == nrentries)
		entries = (entry_t *)realloc(entries, sizeof(entry_t) * (size *= 2));
	entries[nrentries++] = entry;
}

int Database::lookup(string name, entry_t * entry){
	for (int i = 0 ; i < nrentries ; i++) {
		if ((*entries[i].name).compare(name) == 0) {
			if (entry != NULL)
				*entry = entries[i];
			return 1;	
		}
	}
	return 0;
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
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

<<<<<<< HEAD
int look_up_ip(unsigned long ip){
  int output=0;
  server* temp=begin_servers;
  while(temp->next==NULL){
    if(temp.ip==ip)output++;
    temp=temp->next;
  }
  return output;
}
=======
entry_t * Database::allEntries(int * s)
{
	* s = nrentries;
	return entries;
}*/
>>>>>>> a1d2f0b051bd0c15db3c437bc256319740ba0667
