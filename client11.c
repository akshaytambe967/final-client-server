#include "header.h"

#define ServerPort 54321
#define hostid "127.0.0.1"
#define ToServer 1000  
#define FromServer 1000 

int SocketConnect(int hSocket)
{
    int connect_value;
    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr(hostid);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);
    connect_value = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
    return connect_value;
}


int SocketSend(int hSocket,char* Rqst,short lenRqst)
{
    int send_value;
    send_value = send(hSocket, Rqst, lenRqst, 0);
    return send_value;
}


int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{
    int receive_value;
    receive_value=recv(hSocket, Rsp, RvcSize, 0);
    return receive_value;
}


int main()
{
    int hSocket;

    char SendToServer[ToServer] = {0};
    char server_reply[FromServer] = {0};
    char server_reply1[FromServer] = {0};
    char SendToServer1[ToServer] = {0};
     

  hSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(hSocket == -1)
    {
        printf("Could not create socket\n");
        return 1;
    }

   printf("Socket is created\n");
   
  
    if (SocketConnect(hSocket) < 0)
    {
        perror("connect failed.\n");
        return 1;
    }

printf("Sucessfully conected with server\n");
  
do{

    printf("\n1)ls\t2)pwd\t3)cd\t4)BYE\n\nEnter Command Name As Input-:  ");
    scanf("%s",SendToServer);

    SocketSend(hSocket, SendToServer, strlen(SendToServer));
  memset(server_reply,'\0',FromServer);
  memset(server_reply1,'\0',FromServer);
    if(strcmp(SendToServer,"cd") == 0)
      {
          printf("\nEnter Directory name where you want to enter-: ");
          scanf("%s",SendToServer1);

          SocketSend(hSocket, SendToServer1, strlen(SendToServer1));

          SocketReceive(hSocket, server_reply, FromServer);
   /*          if(server_reply != NULL)
             {
              printf("%s",server_reply1);
              printf("\n");

            }
             else
             {
              printf("\nDirectory Not Found...Plz Enter Correct Directory...");
              break;
            }*/

          printf("\n%s\n",server_reply);
          memset(server_reply,'\0',FromServer);

     }

    else if(strcmp(SendToServer,"pwd") == 0)
      {
    
       SocketReceive(hSocket, server_reply, FromServer);
  
       printf("\nCurrent Working Directory is : %s\n\n",server_reply);

      }
    else if(strcmp(SendToServer,"bye") == 0)
      {
       close(hSocket);
       exit(0);
      }
    else if(strcmp(SendToServer,"ls") == 0)
     {

         SocketReceive(hSocket, server_reply1, FromServer);
 
         printf("%s",server_reply1);
         printf("\n"); 
     }

  else
     {
     printf("!!!!!Plz Enter Valid Command As Input!!!!\n");
    close(hSocket);
    exit(0);
    }
  }while(SendToServer != NULL);

 close(hSocket);
    
    return 0;
}
