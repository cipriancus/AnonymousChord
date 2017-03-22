/*
 *  ChordNode.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 *	This class is the implementation of the "Chord" class,
 *	it inherits from the Chord abstract class and implements
 *	it's abstract methods.
 */

#ifndef CHORDNODE_H
#define CHORDNODE_H

#include "IOverlay.h"
#include "AbstractChord.h"
#include "TransportHTTP.h"
#include <string>
#include <map>
#include <vector>
#include "Query.h"

class Stabilization;

using namespace std;

struct contact{
    ChordNode *node;
    Node *selectedNode;
    transportCode tr;
    map<string,string> queryParams;
};

class ChordNode: public AbstractChord {
public:
        ChordNode();
	/* Constructor & Destructor */
	ChordNode(const string &ip, int port, const string &overlayIdentifier,
			const string &rootDirectory);
	~ChordNode();

	/* Setters */
	void setIdentifier(const string &iD) { overlayIdentifier = iD; }
	void setTransport(TransportHTTP *t)  { transport = t; }

	/* Action Methods */
	virtual string sendRequest(Request *request, Node* destination);
	virtual void fixBrokenPointers(Node* node);
	virtual bool isAlone();
	virtual void checkStable();
	virtual void shutDown();

	/* Override Methods */
	void notify(Node *n);
	void stabilize();

	/* IOverlay Pure METHODS */
	void   put(string key, string value);
	string get(string key);
	void   removekey(string key);


	/* data CRUD */
	void saveData(string filename, string value);
	string openData(string filename);
	string serializeData(string data);
	string unserializeData(string data);
        
	/* tools methods */
        Query* searchForQueryByHash(string hash);
	unsigned int   getIntSHA1(string key);	
        char *getHexSHA1(string str);	
        string getIdentifier() 			{ return overlayIdentifier; }	
        TransportHTTP* getTransport()   { return transport; }	
        std::vector<Node*> getFingerTable(){return fingerTable;}   
        Node* getThisNode() 			{ return thisNode; }    
        string serialize(ChordNode *node);   
        ChordNode* deserialize(string data);
        bool getQueryForHash(string hash,Query *query);
        
      
        void                    randomWalk();
      
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version){ar & fingerTable;}
       
        string send_request_with_timeout(Node *selectedNode,transportCode tr,int noOfSecondsToWait,map<string,string> &queryParams);

         
        
        pthread_mutex_t calculating = PTHREAD_MUTEX_INITIALIZER; 
        pthread_cond_t done = PTHREAD_COND_INITIALIZER;
private:
        char* itoa(int num,char* str,int base);
        void  reverse(char str[], int length);

    
        TransportHTTP* transport;

	/* One thread for the stabilization of each node */
	Stabilization* stableThread;
	/* The id of the overlay */
	string overlayIdentifier;
	/* node notification */
	bool notified;
	/* Part of the DHT */
	typedef std::pair<string, string> data;
	typedef std::map<string, string> dataMap;
	dataMap table;
        
        vector<Query*> allQueries;
        
       
};
#endif
