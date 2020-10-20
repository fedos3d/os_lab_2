


#include <stdio.h>

#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
  
// Function designed for chat between client and server. t
void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        // print buffer which contains the client contents 
        printf("From client: %s", buff); 
        //bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        //while ((buff[n++] = getchar()) != '\n') 
        //    ; 
  
        // and send that buffer to client 
        //write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("ps", buff, 2) == 0) {
			int kek = fork();
			if (kek != 0) {
				execv("a.out", NULL);
			}
		}
        if (strncmp("exit", buff, 4) == 0) { 
			//write(1, "Server has finished", 30);
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

int main(int argc, char **argv)
{
	//test pipe
	int fd[2];
			pipe(fd);
			int kek = fork();
			if (kek != 0) {
				int size;
				execv("a.out", NULL);
				close(fd[1]);
				read(fd[0], &size, sizeof(size));
				char bufdf[size];
				read(fd[0], bufdf, size);
				write(sockfd, buff, sizeof(buff)); 
				printf("%s", bufdf);
			}
				//
	//daemon creation logic
	//TODO: Insert daemon code here.
        pid_t pid;

		/* Fork off the parent process */
		pid = fork();
		if (pid < 0)
			exit(EXIT_FAILURE);

		/* Success: Let the parent terminate */
		if (pid > 0)
			exit(EXIT_SUCCESS);
		signal(SIGCHLD, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
		umask(0);

		/* Change the working directory to the root directory */
		/* or another appropriated directory */
		chdir("/");
		//tcp conversation logic
	int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // Function for chatting between client and server
  
    // After chatting close the socket
    close(sockfd); 
    while (1)
    {
        char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
		
        // read the message from client and copy it in buffer 
        // print buffer which contains the client contents 
        //printf("From client: %s", buff); 
        //bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        //while ((buff[n++] = getchar()) != '\n') 
        //    ; 
  
        // and send that buffer to client 
        //write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("ps", buff, 2) == 0) {
			int fd[2];
			pipe(fd);
			int kek = fork();
			if (kek != 0) {
				int size;
				execv("a.out", NULL);
				close(fd[1]);
				read(fd[0], &size, sizeof(size));
				char bufdf[size];
				read(fd[0], bufdf, size);
				write(sockfd, buff, sizeof(buff)); 
				printf("%s", bufdf);
			}
		}
        if (strncmp("exit", buff, 4) == 0) { 
			//write(1, "Server has finished", 30);
            printf("Server Exit...\n");
            exit(1); 
            break; 
        } 
    } 
        syslog (LOG_NOTICE, "First daemon started.");
        sleep (20);
    }
	close(sockfd); 
    syslog (LOG_NOTICE, "First daemon terminated.");

    return EXIT_SUCCESS;
	
	
	return 0;
}

