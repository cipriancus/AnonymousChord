#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <cctype>
#include <openssl/sha.h>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
extern int errno;
using namespace std;

int port;


int main ( int argc , char *argv [ ] ) {
	int sd;			
	struct sockaddr_in server;

	port = 5757;

	if ( ( sd = socket ( AF_INET , SOCK_STREAM , 0 ) ) == -1 )
	{
		perror ( "Eroare la socket().\n" );
		return errno;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
	server.sin_port = htons ( port );


	if ( connect ( sd , ( struct sockaddr * ) &server , sizeof ( struct sockaddr ) ) == -1 )
	{
		perror ( "[client]Eroare la connect().\n" );
		return errno;
	}

	while ( 1 ){
		char buf [ 1000 ];
		read ( 0 , buf , sizeof( buf ) ); 
                buf[strlen(buf)-1]='\0';

		if ( strcmp ( buf , "1" ) == 0 ){//put
			char key [ 1000 ];
                        read ( 0 , key , sizeof( buf ) ); 
                        key[strlen(key)-1]='\0';

                        char value [ 1000 ];
                        read ( 0 , value , sizeof( buf ) ); 
                        value[strlen(value)-1]='\0';

                        if ( write ( sd , "put" , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                         if ( write ( sd , &key , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                         if ( write ( sd ,&value , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
		}
		else if ( strcmp ( buf , "2" ) == 0 ){//remove
			char key [ 1000 ];
                        read ( 0 , key , sizeof( buf ) ); 
                        key[strlen(key)-1]='\0';

                        if ( write ( sd , "remove" , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                         if ( write ( sd , &key , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
		}
		else if ( strcmp ( buf , "3" ) == 0 ){//get
			char key [ 100 ]={NULL};
                        read ( 0 , key , sizeof( buf ) ); 
                        key[strlen(key)-1]='\0';

                        if ( write ( sd , "get" , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                        if ( write ( sd , &key , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                        
                        char value[100];
                        if ( read ( sd , &value , sizeof( value ) ) <= 0 )
                        {
                                perror ( "[client]Error at server read.\n" );
                                exit(0);
                        }
                        printf ( "%s \n" , value );

		}
		else
		if ( strcmp ( buf , "4" ) == 0 ){//exit
			if ( write ( sd , "exit" , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
			close ( sd );
			exit ( 1 );
		}else if ( strcmp ( buf , "5" ) == 0 ){//anon get
			char key [ 1000 ];
                        read ( 0 , key , sizeof( buf ) ); 
                        key[strlen(key)-1]='\0';

                        if ( write ( sd , "randomWalk" , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                         if ( write ( sd , &key , 100 ) <= 0 ){
				perror ( "[client]Eroare la write() spre server.\n" );
			}
                        
                        
                        char valoare[100];
                        if ( read ( sd , &valoare , sizeof( valoare ) ) <= 0 )
                        {
                                perror ( "[client]Error at server read.\n" );
                                exit(0);
                        }
                        printf ( "%s \n" , valoare );

		}
		
	}
}