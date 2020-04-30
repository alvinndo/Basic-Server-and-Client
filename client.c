// Alvin Do
// Studio 24 - Client

#include <sys/types.h> //these two for binding(), socket(), listen(), and accept()
#include <sys/socket.h>
#include <sys/un.h> //AF_UNIX stuff
#include <stdio.h> //these two for.. you know
#include <unistd.h>
#include <errno.h> //these two for error handling
#include <stdlib.h>
#include <string.h> //memset
#include <signal.h> //catching keystrokes

#define PATHNAME "randompath"

int main( int argc, char* argv[] )
{

/**
 * 1 - Same socket stuff
 */
	int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( sockfd == -1 )
	{
		perror("Couldn't do the socket stuff man..\n");
		exit(-1);
	}
/**
 * 2 - Connect()
 */
	struct sockaddr_un my_addr;
	memset( &my_addr, 0, sizeof( struct sockaddr_un ) );

	my_addr.sun_family = AF_UNIX;
	strncpy( my_addr.sun_path, PATHNAME, sizeof( my_addr.sun_path )-1 );

	int connect_ret_val = connect( sockfd, (struct sockaddr*) &my_addr, sizeof( struct sockaddr_un ) );
	if( connect_ret_val == -1 )
	{
		perror("Cannot connect() :( ");
		exit(-1);
	}

	printf("Connected to server successfully!\n");
/**
 * Reading and Writing
 */
	FILE* sockStream = fdopen( sockfd, "w+" );

	fprintf( sockStream, "quit");

return 0;
}

