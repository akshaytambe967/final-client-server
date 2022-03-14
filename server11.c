#include "header.h"

#define PORT 54321
#define Client_Message 1000
#define Message 1000
#define MAX 1000
#define Max_Clients 5

typedef struct client
{
  int sock;
  struct sockaddr_in client_addr;
  char client_path[MAX];
}NODE;

void *connection_handler(void *sock_desc);

int main()
{
    struct sockaddr_in server_addr;
    int n=Max_Clients,limit=0,option=1;

   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd == -1)
    {
        printf("Socket creation failed");
        return 1;
    }
    printf("Socket created successfully.......\n");
   
  
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,(char *)&option,sizeof(option)) < 0)
    {
      perror("Unable to reuse port!!!!");
      close(sockfd);
      exit(0);
    }
  
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT); 
    
   if(bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr))<0)
    {
      perror("Binding fail!!!!!!!");
      close(sockfd);
      return 1;
    }
    printf("Binding Done Sucessfully...\n");
     
    listen(sockfd, n);

    int clientLen = sizeof(struct sockaddr_in);
      
        
    printf("Waiting for clients.....\n");
   
   for(int i=1;i<=n;i++)
   {
               
   if(limit == n)
   printf("Server has reached clients limit...\n");

   NODE *ptr = (NODE *)malloc(MAX);

   ptr->sock = accept(sockfd,(struct sockaddr *)&ptr->client_addr,(socklen_t*)&clientLen);
   printf("Connection accepted for Client[%d]\n",i);
   
   strlcat(ptr->client_path,get_current_dir_name(),MAX);
            
   pthread_t * pthreads[n];

   pthread_t tid[i];
              
   pthreads[i] = &tid[i];

   if(pthread_create(pthreads[i], NULL, connection_handler, (void *)ptr) < 0)
    {
		printf("\nThread Creation Fail!!!!!!\n");
    close(sockfd);
    return -1;           
    }
             
      limit++;
                
    }
    pthread_exit(NULL);

   close(sockfd);
   
}
         


void *connection_handler(void *sock_desc)
{
   NODE *ptr = (NODE *)sock_desc;  
   char client_message[Client_Message] = {0};
   char server_message[Message] = {0};
   char message[Message] = {0};
   char cwd[PATH_MAX];
 
   while(1)
    {
      memset(client_message, '\0', sizeof client_message);
      memset(server_message, '\0', Message);
             
      if(recv(ptr->sock, client_message, sizeof(client_message), 0) < 0)
      {
       printf("Could not receive");            
      }

      printf("Client Message : %s\n",client_message);
        
              
    if(strcmp("pwd",client_message) == 0)
      {
       memset(server_message, '\0', Message);
       memset(cwd,'\0',MAX);
       
       strlcpy(cwd,ptr->client_path,MAX);
     
       strlcpy(server_message,cwd,sizeof(server_message));
       send(ptr->sock,server_message,strlen(server_message),0);

      }
        
 
    else if(strcmp("ls",client_message) == 0)
      {     
        memset(server_message, '\0', sizeof server_message);
  

        struct stat t_stat;
        struct dirent *entry;

        DIR *dir = opendir(ptr->client_path);

        if (dir == NULL)
        {
        printf("\nDirectory does not exist\n");
        close(ptr->sock);
        return 0;
         }

      
       while ((entry = readdir(dir)) != NULL)
        {
         stat(entry->d_name, &t_stat);
         time_t ltime;
         char bf[MAX];
         time(&ltime);
	
           if( S_ISDIR(t_stat.st_mode) )
           {
           strlcat(server_message,"\nType: Directory ",sizeof(server_message));
           }
          else
           {
           strlcat(server_message,"\nType: File ",sizeof(server_message));
           }
            strlcat(server_message," File_Name: ",sizeof(server_message));
            strlcat(server_message,entry->d_name,sizeof(server_message));
     
            if(ctime_r(&ltime, bf) != NULL)
            {
              strlcat(server_message," File_Creation_Date: ",sizeof(server_message));
            }
           strlcat(server_message,ctime_r(&ltime, bf),sizeof(server_message));

         }
        
        send(ptr->sock,server_message,strlen(server_message),0);

      }      
  
     else if(strcmp("cd",client_message) == 0) 
      {
        memset(server_message, '\0', sizeof server_message);
        recv(ptr->sock, message, Message, 0);

        printf("Directory Name From Client is -: %s\n",message);

        memset(ptr->client_path,'\0',MAX); 
        strlcat(ptr->client_path,message,MAX);

       DIR *dirptr=opendir(message);
      
      if(dirptr==NULL)
       {
	     printf("\nDirectory does not exist\n");
       strlcat(server_message,"Directory Not Found!!",Message);
       send(ptr->sock,server_message,strlen(server_message),0);
       
       strlcpy(ptr->client_path,get_current_dir_name(),MAX);

       memset(server_message, '\0',Message);
        memset(message, '\0',Message);

       }
      else 
       {
	     closedir(dirptr);
       memset(server_message, '\0', sizeof server_message);
       bzero(server_message,Message);
       
       strlcat(server_message,"Directory Changes Sucessfully...",sizeof(server_message));
       send(ptr->sock,server_message,strlen(server_message),0);

       memset(message, '\0', sizeof message);
       }
      
     
     } 
       
     else if(strcmp("bye",client_message) == 0) 
      {
           printf("Disconnected from client.\n");
           close(ptr->sock);  
           return NULL;
       }
     else
      {
      printf("Wrong Input From Client");
      close(ptr->sock);
      return NULL;
      }

   }
   
   free(ptr);
}
