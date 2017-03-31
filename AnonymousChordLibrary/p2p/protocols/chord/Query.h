/* 
 * File:   Query.h
 * Author: ciprian
 *
 * Created on March 22, 2017, 1:32 AM
 */

#ifndef QUERY_H
#define QUERY_H

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
#include "sha1.h"
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <random>  


using namespace std;

class Query {
public:
    Query();
    Query(int l);
    Query(int min,int max);
    virtual ~Query();

    string getQueryHash(){return queryHash;}
    string setQueryHash(string hash){queryHash=hash;}
    list<vector<Node *>> getAllSelectedFingerTables(){return selectedFingerTables;}
    vector<Node*> getSelectedNodes(){return selectedNodes;}
    int           getL(){return l;}
    
    //add a fingertable of a certain node
    bool addFingerTable(vector<Node *> fingerTable,Node* node);
    bool hasBeenSelected(Node* node);
    Node* selectNewNode(vector<Node*> fingerTable);
    
    vector<Node *> getLastFingerTableEntry(){return selectedFingerTables.back();}

private:
    #define P_SINGLETON ProtocolSingleton::getInstance()

    int random(int min, int max);
    int random();
    
    char* itoa(int num, char* str, int base);
    void  reverse(char str[], int length);
    
    
     string queryHash;
     list<vector<Node *>> selectedFingerTables;//list with all fingertables | They must be digitally signed
     vector<Node*> selectedNodes;//all selected random nodes
     int l;
     
     string public_key;
     string private_key;
};

#endif /* QUERY_H */

