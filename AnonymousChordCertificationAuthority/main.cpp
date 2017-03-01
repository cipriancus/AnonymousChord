#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <anonCh/ChordNode.h>
#include <pthread.h>
#include <anonCh/ProtocolSingleton.h>
#include <anonCh/CertificationAuthority.h>
#include <map>

using namespace std;

// This application receives args, "ip", "port", "overlay identifier (unique string)", "root directory)"
int main(int argc, char * const argv[]) 
{
    //certification authority is the first node
    string backBone[] = {"127.0.0.1",};
    
    CertificationAuthority *chord = P_SINGLETON->initCA(std::string(argv[1]), atoi(argv[2]), std::string("AnonymousChord"), std::string(argv[3]));
   
    Node* node=new Node(backBone[0],8000);
    
    chord->getChordNode()->join(node);
    
    while(1){
        int a;
        cin>>a;
        
        if(a==0)
            break;
    }
    
    delete chord;
    return 0;
}