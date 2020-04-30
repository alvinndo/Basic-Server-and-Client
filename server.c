// Alvin Do
// Studio 24 - Server

#include <sys/time.h> //for select()
#include <sys/types.h> //these two for binding(), socket(), listen(), and accept()
#include <sys/socket.h>
#include <sys/un.h> //AF_UNIX stuff
#include <stdio.h> //these two for.. you know
#include <unistd.h>
#include <errno.h> //these two for error handling
#include <stdlib.h>
#include <string.h> //memset and strncmp
#include <signal.h> //catching keystrokes

#define UNIX_PATH_MAX   108
#define PATHNAME	"randompath"
#define BUFFERSIZE	1024

void	INThandler(int);

int main( int argc, char* argv[] )
{
	signal(SIGINT, INThandler);
	printf("Starting server..\n");
	sleep(1);
/**
 * 1 - Creating Socket
 */
	int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( sockfd == -1 )
	{
		perror("Oops! Our highly trained monkey messed up the socket()");
		exit(-1);
	}
	else printf("Socket() finished. Establishing connection..\n");
	sleep(0.5);
/**
 * 2 - Binding socket
 */
	struct sockaddr_un my_addr;
	memset( &my_addr, 0, sizeof( struct sockaddr_un ) );

	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, PATHNAME, sizeof( my_addr.sun_path) -1 );

	int bind_ret_val = bind( sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr_un) );
	if( bind_ret_val == -1 )
	{
		perror("Oops! Our highly trained monkey messed up the bind()");
		exit(-1);
	}
	else printf("Binding() finished. Server is ready!\n");
/**
 * 3 - Listen Linda Listen for incomming connections (please tell me you got that reference)
 */
	int lis_ret_val = listen( sockfd, 5 );
	if( lis_ret_val == -1 )
	{
		perror("Oops! Our highly trained monkey messed up the listen()");
		exit(-1);
	}
	else printf("Waiting for someone to stop by..\n");
/**
 * 4 - Accept connections
 */
	char buffer[ BUFFERSIZE ];
	struct sockaddr_un peer_addr;
	socklen_t peer_addr_size = sizeof(struct sockaddr_un);

	int i, acceptfd, clients = 0, maxClients = 5;	
	for( i = 0; i < maxClients; i++ )
	{
		acceptfd = accept( sockfd, (struct sockaddr*) &peer_addr, &peer_addr_size ); 
		if( acceptfd == -1 )
		{
			perror("Oops! Our highly trained monkey messed up the accept()");
			exit(-1);
		}
		else
		{
			printf("!!! Ayyyyyy someone has connected !!!\n");
			clients++;
			if( clients == 1 ) printf("+-+-+There is %d client connected+-+-+\n", clients);
			else if( clients > 1 && clients != 5 ) printf("+-+-+There are %d clients connceted+-+-+\n", clients);
			else if( clients == 5) printf("+-+-+Max number of %d clients connected+-+-+\n", clients);
		}
        }

/**
 * 5 - Reading and Writing
 */
	FILE* clientSock = fdopen( acceptfd, "w+" );
	fgets( buffer, BUFFERSIZE, clientSock );

	printf("Client said: %s\n", buffer);
/**
 * 6 - Unlinking and terminating
 */
        if( strncmp( "quit", buffer, 4 ) == 0 )
        {       
                printf("Nooo you just said '%s'! Don't leave me :( \n");

		int ul_ret_val = unlink( PATHNAME );
        	if( ul_ret_val == -1 ) 
        	{
                	perror("Oops! We should fire our monkey. Can't do unlink()");
                	exit(-1);
        	}

        	printf("Closing server..\n");
        	close(sockfd);
        	close(acceptfd);
        	exit(0);
        }
/*
        int ul_ret_val = unlink( PATHNAME );
        if( ul_ret_val == -1 ) 
        {
                perror("Oops! We should fire our monkey. Can't do unlink()");
                exit(-1);
        }

        printf("Closing server..\n");
        close(sockfd);
	close(acceptfd);
        exit(0);
*/

return 0;
}

void INThandler(int sig)
{
	char  c;

	signal(sig, SIG_IGN);
	printf("\nAre you sure you want to quit? [y/n] ");

	c = getchar();
	if (c == 'y' || c == 'Y')
	{
        	int ul_ret_val = unlink( PATHNAME );
        	if( ul_ret_val == -1 )
        	{   
                	perror("Oops! We should fire our monkey. Can't do unlink()");
			exit(-1);
        	}
		printf("Closing server..\n");
		sleep(1);
		exit(0);
	}
	else
	{
		signal( SIGINT, INThandler );
		getchar();
	}
}
