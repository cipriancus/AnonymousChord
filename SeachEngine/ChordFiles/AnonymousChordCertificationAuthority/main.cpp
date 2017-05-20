#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <anonCh/ChordNode.h>
#include <pthread.h>
#include <anonCh/ProtocolSingleton.h>
#include <anonCh/CertificationAuthority.h>
#include <map>
#include <string.h>
#include <openssl/dsa.h>
#include <openssl/evp.h>

using namespace std;

unsigned char* sign_it( EVP_PKEY *key,const char* msg,size_t* slen){
    EVP_MD_CTX *mdctx = NULL;
    int ret = 0;
    
    void* sig = NULL;

    /* Create the Message Digest Context */
    if(!(mdctx = EVP_MD_CTX_create())) 
        goto err;

    /* Initialise the DigestSign operation - SHA-256 has been selected as the message digest function in this example */
     if(1 != EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, key)) 
         goto err;

     /* Call update with the message */
     if(1 != EVP_DigestSignUpdate(mdctx, msg, strlen(msg)))
         goto err;

     /* Finalise the DigestSign operation */
     /* First call EVP_DigestSignFinal with a NULL sig parameter to obtain the length of the
      * signature. Length is returned in slen */
     if(1 != EVP_DigestSignFinal(mdctx, NULL, slen))
         goto err;
    
     /* Allocate memory for the signature based on size in slen */
     if(!(*sig = OPENSSL_malloc(sizeof(unsigned char) * (*slen)))) 
         goto err;
    
     /* Obtain the signature */
     if(1 != EVP_DigestSignFinal(mdctx, *sig, slen)) 
         goto err;

     /* Success */
     ret = 1;

     err:
     if(ret != 1)
     {
       /* Do some error handling */
     }

     return sig;
     /* Clean up */
     if(*sig && !ret) 
         OPENSSL_free(*sig);
     
     if(mdctx) 
         EVP_MD_CTX_destroy(mdctx);
}

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
    
    //DSA* dsa=DSA_new();
    //DSA_generate_parameters_ex(dsa,2048,NULL,0,NULL,NULL, NULL);
    //int rc=DSA_generate_key(dsa);

    
    delete chord;
    return 0;
}