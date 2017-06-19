#include "Node.h"
#include "Request.h"
#include "ChordNode.h"
#include "ProtocolSingleton.h"
#include "Exception.h"
#include <stdio.h>
#include <stdlib.h>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>

Node::Node() {
    ip = "";
    nid = 0;
    port = 0;
    signature = "";
    nidS = "";

}

Node::Node(const string &inIp, unsigned int inId, unsigned int inPort) {
    ip = inIp;
    nid = inId;
    port = inPort;

    //Transform the nid into a string for later usage.
    char nidBuffer[255];
    sprintf(nidBuffer, "%d", nid);
    nidS = string(nidBuffer);
}


Node::Node(const string &inIp, unsigned int inPort) {
    ip = inIp;
    nid = 0;
    port = inPort;

    //Transform the nid into a string for later usage.
    char nidBuffer[255];
    sprintf(nidBuffer, "%d", nid);
    nidS = string(nidBuffer);  
}

Node::Node(const string &inData) {
    string inID, inIP, inPort;
    int at = inData.find("@");
    inID = inData.substr(0, at);
    int ddot = inData.find(":");
    inIP = inData.substr(at + 1, ddot - at - 1);
    inPort = inData.substr(ddot + 1);

    this->nidS = inID;
    this->nid = atoi(inID.c_str());
    this->ip = inIP;
    this->port = atoi(inPort.c_str());
}

Node::~Node() {

}

bool Node::equals(Node *aNode) {
    if (aNode != NULL)
        return ((aNode->getId() == nid) && (aNode->getPort() == port)
            && (aNode->getIp() == ip));

    return false;
}

/* format: <ID>@<IP>:<PORT> */
string Node::toString() {
    ostringstream ss;
    char portBuffer[255];
    sprintf(portBuffer, "%d", port);
    string portS(portBuffer);
    ss << nidS << "@" << ip << ":" << portS;
    return (ss.str());
}

string Node::serializeNode() {
    std::ostringstream ofs;
    boost::archive::text_oarchive oa(ofs);
    ofs.flush();
    oa << this;
    return ofs.str();
}

Node* Node::deserializeNode(string serializedNode) {
    Node *tempNode;
    std::istringstream ifs(serializedNode);
    boost::archive::text_iarchive ia(ifs);
    ia>>tempNode;
    return tempNode;
}

