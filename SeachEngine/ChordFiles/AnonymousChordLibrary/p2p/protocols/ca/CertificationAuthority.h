#ifndef CERTIFICATIONAUTHORITY_H
#define CERTIFICATIONAUTHORITY_H

#include "ChordNode.h"

class CertificationAuthority{
public:
    CertificationAuthority(const string &ip, int port,
		const string &overlayIntifier,
		const string &rootDirectory);
    virtual ~CertificationAuthority();
    
    std::map<string,Node*> getAllNodeSignatures();
    string signNode(string ip, int port,int nid);
    bool verifyNode(string ip, int port,int nid,string signature);
    void operator+(Node* node);
    ChordNode* getChordNode(){return myNode;}
private:
      //map containing all the nodes and their associated digital signatures
    std::map<string,Node*> allNodeSignatures;
    ChordNode *myNode;
};

#endif 

