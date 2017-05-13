#include <iostream>
#include <string>
#include <stdlib.h>
#include <anonCh/ChordNode.h>
#include <pthread.h>
#include <anonCh/ProtocolSingleton.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;

void handle_sigchld ( int sig ) {
	while ( waitpid ( ( -1 ) , 0 , WNOHANG ) > 0 );
	
}

void child ( int client, char * const argv[] );


int main(int argc, char * const argv[]) 
{   
    	int port=5757;
	struct sockaddr_in server;	
	struct sockaddr_in from;
	int sd;

	int optval = 1;

	if ( ( sd = socket ( AF_INET , SOCK_STREAM , 0 ) ) == -1 )
	{
		perror ( "[AnonCH.Server]Socket error.\n" );
		return errno;
	}
	setsockopt ( sd , SOL_SOCKET , SO_REUSEADDR , &optval , sizeof( optval ) );

	bzero ( &server , sizeof ( server ) );
	bzero ( &from , sizeof ( from ) );

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl ( INADDR_ANY );
	server.sin_port = htons ( port );

	if ( bind ( sd , ( struct sockaddr * ) &server , sizeof ( struct sockaddr ) ) == -1 )
	{
		perror ( "[AnonCH.Server]Binding error.\n" );
		return errno;
	}

	if ( listen ( sd , NULL) == -1 )
	{
		perror ( "[AnonCH.Server]Error at listen().\n" );
		return errno;
	}

	while ( 1 )
	{
		int client;
		socklen_t length = sizeof( from );

		printf ( "[AnonCH.Server]Waiting at port:  %d...\n" , port );
		fflush ( stdout );

		client = accept ( sd , ( struct sockaddr * ) &from , &length );

		if ( client < 0 )
		{
			perror ( "[AnonCH.Server]Error at accept.\n" );
			continue;
		}
                
		int copil_fct;
		if ( ( copil_fct = fork ( ) ) == 0 ){
			child ( client, argv );

		}
		if ( signal ( SIGCHLD , handle_sigchld ) == SIG_ERR) {
			perror ( 0 );
			exit ( 1 );
			printf ( "[server]Mesajul a fost trasmis cu succes.\n" );
		}
	}
}


void child ( int client, char * const argv[] ){
    
    Node *node = NULL;
    ChordNode *chord = NULL;
    // This application receives args, "ip", "port", "overlay identifier (unique string)", "root directory)"
    chord = P_SINGLETON->initChordNode(std::string(argv[1]), atoi(argv[2]), std::string("AnonymousChord"), std::string(argv[3]));        
    node = new Node(std::string(argv[4]), 8000);
    chord->join(node);
        
	while ( 1 ){     
                char  comanda[100];
                char key[100];

		if ( read ( client , &comanda , sizeof( comanda ) ) <= 0 )
		{
			perror ( "[AnonCH.Server]Error at client read.\n" );
			exit(0);
		}
                
                if ( read ( client , &key , sizeof( key ) ) <= 0 )
		{
			perror ( "[AnonCH.Server]Error at client read.\n" );
			exit(0);
		}

		if ( strcmp ( comanda , "get" ) == 0 ){
                    string value=chord->get(string(key));
                    
                    char value_c[100];
                    strcpy(value_c,value.c_str());
                    
                    if ( write ( client , &value_c , sizeof( value_c ) <= 0 ))
			{
				perror ( "[AnonCH.Server]Error at client write.\n" );
				return;
			}
		}
		else
		if ( strcmp ( comanda , "put" ) == 0 ){
                    char value[100];
                    if ( read ( client , &value , sizeof( value ) ) <= 0 )
                    {
                            perror ( "[AnonCH.Server]Error at client read.\n" );
                            exit(0);
                    }

                    chord->put(key,string(value));
		}
		else
                if ( strcmp ( comanda , "remove" ) == 0 ){
			chord->removekey(key);
		}
		else
                if ( strcmp ( comanda , "randomWalk" ) == 0 ){
		    string value=chord->randomWalk(string(key));
                    
                    char value_c[100];
                    strcpy(value_c,value.c_str());
                    
                    if ( write ( client , &value_c , sizeof( value_c ) <= 0 ))
			{
				perror ( "[AnonCH.Server]Error at client write.\n" );
				return;
			}
		}
		else
		if ( strcmp ( comanda , "exit" ) == 0 ){
                        delete node;
                        delete chord;
			close ( client );
			exit(0);
		}
	}                                 
    delete node;
    delete chord;
}


/*
 * 
 * cout<<"Give search key ";
    
    string key;
    cin >> key;

    sleep(5);
    cout<<"Searched value is "<<node->randomWalk(key)<<endl;       
 * string backBone[] = {
			// first node, CA
            "192.168.0.105",
	};
//                    if(node->getSignatureFromCA(chord)!=true){
//                        exit(1);
//                    }//take signature from CA
//                    cout<<node->verifyNodeSignature(node->getThisNode(),node->getThisNode()->getSignature(),chord);
		}
                
 
//		char entry[256];
//		string key;
//		string value;
//		
//		while (1) { // sleep...
//			cout << "\n0) Print status\n" << 
//					"1) Put\n" << 
//					"2) Get\n" <<
//					"3) Remove\n" << 
//					"4) Exit\n\n";
//			cout << "---> ";
//			cin >> entry;
//			int chx = atoi(entry);
//
//			switch (chx) {
//    			case 0:
//    				cout << "\n" << node->printStatus();
//    				break;
//    			case 1:
//    				cout << "Key = ";
//    				cin >> key;
//    				cout << "Value = ";
//    				cin >> value;
//    				node->put(key, value);
//    				break;
//    			case 2:
//    				cout << "Key = ";
//    				cin >> key;
//    				cout << "\n" << node->get(key) << "------> found!" << endl;
//    				break;
//    			case 3:
//    				cout << "Key = ";
//    				cin >> key;
//    				node->removekey(key);
//    				break;
//    			case 4:
//    				node->shutDown();
//    			default:
//				break;
//			}         
//	    }*/