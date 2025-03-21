#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <openssl/bio.h>

using namespace std;

class Node {
public:
    Node();
    Node(const string &inIp, unsigned int inId, unsigned int inPort);
    Node(const string &ip, unsigned int port);
    Node(const string &data);
    ~Node();

    bool equals(Node *node);
    string toString();
    vector<string> tokenize(const string& str, const string& delims);
    void setKey();

    //Getters & Setters

    unsigned int getId() {
        return nid;
    }

    void setId(unsigned int inId) {
        nid = inId;
    }

    string getIdString() {
        return nidS;
    }

    void setIdString(const string &s) {
        nidS = string(s);
    }

    string getIp() {
        return ip;
    }

    void setIp(const std::string inIp) {
        ip = inIp;
    }

    unsigned int getNid() {
        return nid;
    }

    void setNid(unsigned int n) {
        nid = n;
    }

    int getPort() {
        return port;
    }

    void setPort(int inPort) {
        port = inPort;
    }

    string getSignature() {
        return signature;
    }

    void setSignature(string signature) {
        this->signature = signature;
    }

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, unsigned int version) {
        // save/load base class information
        ar & signature & ip & nid & nidS&port;
    }

    string serializeNode();
    Node* deserializeNode(string serializedNode);

private:
#define P_SINGLETON ProtocolSingleton::getInstance()

    string signature;
    string ip;
    unsigned int nid; //node id (nid), "id" is reserved keyword in Cocoa.
    string nidS;
    int port;
};
#endif
