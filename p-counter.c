/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */

/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */

//Authors: Evan Drake, Jude Thomas
//class: 212-EECE446-01-3080 (Lecture)
//class: 212-EECE446-05-3079 (Lab)
//semester: spring 2021
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT "80"
#define MAX_LINE 1000

// "GET /~kkredo/file.html HTTP/1.0\\r\\n\\r\\n"
// www.ecst.csuchico.edu
/*
 * Lookup a host IP address and connect to it using service. Arguments match the first two
 * arguments to getaddrinfo(3).
 *
 * Returns a connected socket descriptor or -1 on error. Caller is responsible for closing
 * the returned socket.
 */
int lookup_and_connect( const char *host, const char *service );

int main( int argc, char *argv[] ) {
	char *host = "www.ecst.csuchico.edu";	//host name
	char buf[MAX_LINE];
	int s;	//socket
	int len = 0; //length of data in bytes (1000 is largest chunk size)
	char msg[] = "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";
	int data = 0;	//stores size of data in bytes
	int newdata = 0;
	int tagcount = 0;
	int i = 0;
	int temp = 0;

	int n = 0;	//counts data that got sent
	int total = 0; //total number of bits sent
	int len_send = (int)sizeof(msg);	//length of messege to be sent
	int bleft = len_send;	//length of messege that has not sent

	if ( argc == 2 ) {
		len = atoi(argv[1]);
	}

	/* Lookup IP and connect to server */
	if ( ( s = lookup_and_connect( host, SERVER_PORT ) ) < 0 ) {
		exit( 1 );
	}

	/* Main loop: get and send lines of text */
	//while(1){
	while(total < len_send){						//this makes sure everything gets sent
		n = send(s, msg+total, bleft, 0);
		if(n == -1){break;}
		total += n;
		bleft -= n;
		if(bleft == 0){break;}
	}
	while(1){
		newdata += recv( s, buf, len, 0);
		if(newdata == 0){break;}
		if(newdata == -1){close(s);exit(1);}

		while(newdata != len){
			temp = newdata;
			newdata += recv(s, buf+newdata,len-newdata,0);
			if(newdata == 0){break;}
			if(newdata == -1){close(s);exit(1);}
			if(temp == newdata){break;}
		}

		for(i = 0;i < newdata-2;i+=1){
			if(buf[i] == '<' && buf[i+1] == 'p' && buf[i+2] == '>'){
				tagcount += 1;
			}

		}

		for(i = 0;i < (int)sizeof(buf);i+=1){
			buf[i] = ' ';
		}

		data += newdata;
		newdata = 0;
	}//while
	printf("%s", "Number of <p> tags: ");
	printf("%i\n", tagcount);
	printf("%s", "Number of bytes: ");
	printf("%i\n", data);

	close(s);

	host = NULL;

	return 0;
}

int lookup_and_connect( const char *host, const char *service ) {
	struct addrinfo hints;
	struct addrinfo *rp, *result;
	int s;

	/* Translate host name into peer's IP address */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ( ( s = getaddrinfo( host, service, &hints, &result ) ) != 0 ) {
		fprintf( stderr, "stream-talk-client: getaddrinfo: %s\n", gai_strerror( s ) );
		return -1;
	}

	/* Iterate through the address list and try to connect */
	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		if ( ( s = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
			continue;
		}

		if ( connect( s, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
			break;
		}

		close( s );
	}
	if ( rp == NULL ) {
		perror( "stream-talk-client: connect" );
		return -1;
	}
	freeaddrinfo( result );

	return s;
}


//end
