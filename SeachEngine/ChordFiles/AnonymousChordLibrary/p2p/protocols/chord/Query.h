#ifndef QUERY_H
#define QUERY_H

#include <mutex>
#include <string>
#include <list>
#include <vector>
#include "Query.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "Node.h"
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <random>  
#include <tomcrypt.h>
#include <openssl/evp.h>
#include "sha1.h"

using namespace std;

class Query {
public:
    Query();
    Query(int l,string hash);
    Query(int l);
    Query(int min, int max);
    virtual ~Query();

    string getPublicKey();
    
    string getOwnerIP(){
        return owner_ip;
    }
    
    string setOwnerIP(string ip){
        owner_ip=ip;
    }

    string getQueryHash() {
        return queryHash;
    }

    list<vector<Node *>> getAllSelectedFingerTables() {
        return selectedFingerTables;
    }

    vector<Node*> getSelectedNodes() {
        return selectedNodes;
    }

    void addSelectedNodes(Node* node) {
        selectedNodes.push_back(node);
    }

    void popFrontSelectedNodes() {
        selectedNodes.erase(selectedNodes.begin(), selectedNodes.begin() + 1);
    }

    int getL() {
        return l;
    }
    
    string getIV(){
        return iv;
    }

    //add a fingertable of a certain node
    bool addFingerTable(vector<Node *> fingerTable, Node* node);
    bool hasBeenSelected(Node* node);
    Node* selectNewNode(vector<Node*> fingerTable);

    vector<Node *> getLastFingerTableEntry() {
        return selectedFingerTables.back();
    }

private:
    #define P_SINGLETON ProtocolSingleton::getInstance()

    int random(int min, int max);
    int random();

    char* itoa(int num, char* str, int base);
    void reverse(char str[], int length);

    void generateKey();
        
    string queryHash;
    
    list<vector<Node *>> selectedFingerTables; //list with all fingertables | They must be digitally signed

    vector<Node*> selectedNodes; //all selected random nodes

    vector<Node*> allFingerTablesNodes; //associates a node with a finger table

    int l;
    
    string owner_ip;

    string query_key;
    string iv;
    
    SHA1 *sha1=new SHA1();
};
#endif /* QUERY_H */

