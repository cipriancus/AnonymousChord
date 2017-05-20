/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CertificationAuthority.cpp
 * Author: ciprian
 * 
 * Created on February 25, 2017, 4:42 AM
 */
#include"CertificationAuthority.h"

CertificationAuthority::CertificationAuthority(const string &ip, int port,
		const string &overlayIntifier,
		const string &rootDirectory) {
    myNode=new ChordNode(ip,port,overlayIntifier,rootDirectory);
}


CertificationAuthority::~CertificationAuthority() {
    myNode->~ChordNode();
    allNodeSignatures.clear();
}

string CertificationAuthority::signNode(string ip, int port,int nid) {
    std::ostringstream oss;
    oss << ip << port;
    string id = string(myNode->getHexSHA1(oss.str()));
       
    if(allNodeSignatures.count(id)==0){
      allNodeSignatures[id]=new Node(ip,nid,port);   
    }
    
    return id;
}

std::map<string,Node*> CertificationAuthority::getAllNodeSignatures() {
    return allNodeSignatures;
}

bool CertificationAuthority::verifyNode(string ip, int port,int nid,string signature) { 
    if(allNodeSignatures.count(signature)==0){
        return false;
    }else{
        return (allNodeSignatures[signature]->getNid()==nid&&
                allNodeSignatures[signature]->getIp().compare(ip)==0&&
                allNodeSignatures[signature]->getPort()==port);
    }
}

void CertificationAuthority::operator +(Node* node){
    if(allNodeSignatures.count(node->getSignature())==0){
      allNodeSignatures[node->getSignature()]=node;   
    }
}
