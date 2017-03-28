/*
 *  ChordNode.cpp
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#include "ChordNode.h"
#include "Stabilization.h"
#include "CheckPred.h"
#include "FixFinger.h"
#include "TransportHTTP.h"
#include "callbacks.h"
#include "sha1.h"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <assert.h>
#include <math.h>

static const std::string PERSISTENCE_DIR = ".chord/data";
static const std::string PERSISTENCE_DIR_STRING = ".chord/data/%s";

ChordNode::ChordNode(){
    
}

/* Constructor */
ChordNode::ChordNode(const string &ip, int port, const string &overlayIdentifier, const string &rootDirectory) {
        
        startTime=time(nullptr);
    
	// Define the address space size
	spacesize = 9;

	// Create the id
	std::ostringstream oss;
	oss << ip << port;
	int id = getIntSHA1(oss.str());

	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id > pow(2, spacesize)) && !(id < 0));

	//create our stabilizer threads instance.
	stableThread = new Stabilization(this);

	//Initialize the transport layer.
	transport = new TransportHTTP(port, rootDirectory);

	//set the overlay identifier.
	this->overlayIdentifier = overlayIdentifier;

	// not yet notified
	notified = false;

	//Call our parent's initializer
	initialise(ip, id, port);

	//We start-up our stabilizer thread.
	checkStable();
}

/* Destructor */
ChordNode::~ChordNode() {
	stableThread->kill();
	delete stableThread;
	delete transport;
}

/* Override from AbstractChord */
void ChordNode::notify(Node *n) {
	Node *pred = predecessor;
	((AbstractChord *) this)->notify(n);
	// If the predecessor as changed, update the DHT table
	if (pred != predecessor) {
		notified = true;
	}
}

/* Custom chord stabilize */
void ChordNode::stabilize() {
	((AbstractChord *) this)->stabilize();
	// If the predecessor as changed, update the DHT table
	if (notified && predecessor->getId() != thisNode->getId()) {
		struct dirent *dirEntry;
		char path[256];
		DIR *dir = opendir(PERSISTENCE_DIR.c_str());
		if(dir != NULL){
			while(dirEntry=readdir(dir)) {
				if(dirEntry->d_type != DT_DIR){
					int hFilename = getIntSHA1(dirEntry->d_name);
					if (!insideRange(hFilename, predecessor->getId(), thisNode->getId())){
						Request *request = new Request(this->getIdentifier(), PUT);
						request->addArg("key", dirEntry->d_name);
						request->addArg("value", openData(dirEntry->d_name));
						sendRequest(request, predecessor);
						sprintf(path, PERSISTENCE_DIR_STRING.c_str(), dirEntry->d_name);
						if( remove( path ) != 0 ) {
							perror( "Error deleting file" );
						}
					}
				}
			}
		}
		notified = false;
	}
}

/* find and replace string tools */
string find_and_replace(string str, const string find, string replace) {
	size_t j;
	string source = str;
	for (; (j = source.find(find)) != string::npos;) {
		source.replace(j, find.length(), replace);
	}
	return source;
}

Query* ChordNode::searchForQueryByHash(string hash){
    vector<Query*>::iterator it;
    for(it=allQueries.begin();it!=allQueries.end();it++){
        if((*it)->getQueryHash().compare(hash)==0){
            return (*it);
        }
    }
    return NULL;
}

/* serialize a postit before to store it into the DHT */
string ChordNode::serializeData(string data) {
	data = find_and_replace(data, " ", "\\_");
	data = find_and_replace(data, "\t", "\\t");
	data = find_and_replace(data, "\n", "\\n");
	return data;
}

/* reconstruct a serialized postit */
string ChordNode::unserializeData(string data) {
	data = find_and_replace(data, "\\_", " ");
	data = find_and_replace(data, "\\t", "\t");
	data = find_and_replace(data, "\\n", "\n");
	return data;
}

/* save the data value in a text file */
void ChordNode::saveData(string filename, string value){
	char path[256];
	sprintf(path, PERSISTENCE_DIR_STRING.c_str(), filename.c_str());
	mkdir(".chord/", 0777);
	mkdir(".chord/data", 0777);
	ofstream data(path, ios::out);
	data << unserializeData(value);
}

/* return the content of the file */
string ChordNode::openData(string filename){
	string line, data="";
	char path[256];
	sprintf(path, PERSISTENCE_DIR_STRING.c_str(), filename.c_str());
	ifstream myfile(path);
	if (myfile.is_open()) {
		while (! myfile.eof()) {
			getline(myfile,line);
			data.append(line.c_str());
			if(! myfile.eof()){
				data.append("\n");
			}
		}
		myfile.close();
	}
	else return "null";
	return data;
}

string ChordNode::serialize(ChordNode* node) {
    std::ostringstream ofs;
    boost::archive::text_oarchive oa(ofs);   
    ofs.flush();
    oa<<node;
    return ofs.str();
}

ChordNode* ChordNode::deserialize(string data){
    ChordNode *tempNode;
    std::istringstream ifs(data);
    boost::archive::text_iarchive ia(ifs);
    ia>>tempNode;
    return tempNode;
}


/* DHT Put */
void ChordNode::put(string key, string value) {
	// Convert the key in a hash integer
	int hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		//		table[key] = value;
		saveData(key, value);
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), PUT);
		request->addArg("key", key);
		request->addArg("value", value);
		// Send the Put request
		sendRequest(request, responsible);
	}
}

/* DHT Get */
string ChordNode::get(string key) {
	// Convert the key in a hash integer
	int hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		return openData(key);
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), GET);
		request->addArg("key", key);
		// Send the Put request
		return sendRequest(request, responsible);
	}
}

/* DHT Remove */
void ChordNode::removekey(string key) {
	// Convert the key in a hash integer
	int hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getId() + 1, thisNode->getId())) {
		// I'm responsible for this key
		char path[256];
		sprintf(path, PERSISTENCE_DIR_STRING.c_str(), key.c_str());
		if( remove( path ) != 0 ) {
			perror( "Error deleting file" );
		}
	} else {
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), REMOVEKEY);
		request->addArg("key", key);
		// Send the Put request
		sendRequest(request, responsible);
	}
}

/* Convert a string into an integer in [0, spacesize] */
unsigned int ChordNode::getIntSHA1(string str) {
	SHA1 *sha1 = new SHA1();
	sha1->addBytes( str.c_str(), strlen( str.c_str() ));
	unsigned char* digest = sha1->getDigest();
	unsigned int res = sha1->shaToInteger(digest, 20, pow(2, spacesize));
	delete sha1;
	free( digest );
	return res;
}

/* Convert a string into a string (using SHA1) */
char *ChordNode::getHexSHA1(string str) {
	SHA1 *sha1 = new SHA1();
	sha1->addBytes( str.c_str(), strlen( str.c_str() ));
	unsigned char *digest = sha1->getDigest();
	char *h = sha1->hexPrinter(digest, 20);
	delete sha1;
	free( digest );
	return h;
}

/* Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE" */
string ChordNode::sendRequest(Request *request, Node* destination) {
	char *response = transport->sendRequest(request, destination);
	// response received
	if (response) {
		stringstream ss;
		ss << response;
		free(response); // we must free the initial char* response, to avoid leaks.
		return ss.str();
	} else {
		// Fix the broken pointers of the node
		fixBrokenPointers(destination);
		// time to fix the chord
		sleep(1);
		// The node is completely disconnected of the backbone
		if (isAlone()) { // there is only one response possible
			return getThisNode()->toString();
		}
		// try again the request with a new destination
		return sendRequest(request, findSuccessor(destination->getId()));
	}
}

/* Fix broken pointers algorithm */
void ChordNode::fixBrokenPointers(Node *node) {
	for (int i = 0; i < fingerTable.size() - 1; i++) {
		if (fingerTable[i]->getId() == node->getId()) {
			fingerTable[i] = new Node(thisNode->toString());
		} 
	}
	if (predecessor->getId() == node->getId()) {
		predecessor = new Node(thisNode->toString());
	} 
	if (successor->getId() == node->getId()) {
		successor = new Node(thisNode->toString());
	}
}

/* return true if the node is completely disconnected of the chord */
bool ChordNode::isAlone(){
	for (int i = 0; i < fingerTable.size() - 1; i++) {
		if (fingerTable[i]->getId() != thisNode->getId()){
			return false;
		}
	}
	return predecessor->getId() == thisNode->getId() && successor->getId() == thisNode->getId();
}

/* Starts up the "stabilizer thread" for this peer. */
void ChordNode::checkStable() {
	stableThread->start();
}

bool ChordNode::getQueryForHash(string hash, Query *query) {
    vector<Query*>::iterator it;
    for(it=allQueries.begin();it!=allQueries.end();it++){
        if((*it)->getQueryHash().compare(hash)==0)
            break;
    }    
    if(it==allQueries.end()){
        return false;
    }else{
        query=(*it);
        return true;
    }
}

Node* ChordNode::getNodeForIP(string ip) {
    vector<Node*>::iterator it;
    for(it=fingerTable.begin();it!=fingerTable.end();it++){
        if((*it)->getIp().compare(ip)==0)
            return (*it);
    }
    return NULL;
}


vector<Node*> ChordNode::getPassedQueryForHash(string hash) {
    map<string,vector<Node*>>::iterator it;
    vector<Node*> returnVec;
    for(it=passedQueries.begin();it!=passedQueries.end();it++){
        if((*it).first.compare(hash)==0){
            return (*it).second;
        }
    }
    return returnVec;
}

void ChordNode::addPassedQuery(string hash, vector<Node*> predSucc) {
    passedQueries[hash]=predSucc;
}



/* Stop the stabilization, distribute the key and shutDown the peer */
void ChordNode::shutDown() {
	// kill the stabilization Threads
	stableThread->kill();

	// notify predecessor
	Request *request = new Request(this->getIdentifier(), SETSUCC);
	request->addArg("successor", successor->toString());
	sendRequest(request, predecessor);

	// notify successor
	request = new Request(this->getIdentifier(), SETPRED);
	request->addArg("predecessor", predecessor->toString());
	sendRequest(request, successor);

	// if the node is not alone
	if(successor->getId() != getThisNode()->getId()){
		// give the part of the DHT to the successor
		struct dirent *dirEntry;
		DIR *dir = opendir(PERSISTENCE_DIR.c_str());
		char path[256];
		while(dirEntry=readdir(dir)) {
			if(dirEntry->d_type != DT_DIR){
				request = new Request(this->getIdentifier(), PUT);
				request->addArg("key", dirEntry->d_name);
				request->addArg("value", openData(dirEntry->d_name));
				sendRequest(request, successor);
				sprintf(path, PERSISTENCE_DIR_STRING.c_str(), dirEntry->d_name);
				if( remove( path ) != 0 ) {
					perror( "Error deleting file" );
				}
			}
		}
	}

	// leave
	cout << "bye bye...\n";
	sleep(1);
	exit(0);
}


/* A utility function to reverse a string  */
void ChordNode::reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* ChordNode::itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}
 

void *contact_node(void *args){
      int oldtype;

    contact *contactNodes=(contact*)args;
        
    ChordNode *node=contactNodes->node;
    Node *selectedNode=contactNodes->selectedNode;
    map<string,string> queryParams=contactNodes->queryParams;

    /* allow the thread to be killed at any time */
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
        
    Request *request = new Request(node->getIdentifier(), contactNodes->tr);
    
        if(queryParams.size()>0){
            map<string,string>::iterator it;
            for(it=queryParams.begin();it!=queryParams.end();it++){
                  request->addArg((*it).first,(*it).second);
            }
        }
    
    string response=node->sendRequest(request, selectedNode);//the serialized finger table from the contacted node
   
    pthread_cond_signal(&node->done);
   
    char *responseCh=(char*)malloc(response.size()*sizeof(char));
    strcpy(responseCh,response.c_str());
    return (void*)responseCh;
}

string ChordNode::send_request_with_timeout(Node *selectedNode,transportCode tr,int noOfSecondsToWait,map<string,string> &queryParams){
       struct timespec max_wait;
       memset(&max_wait, 0, sizeof(max_wait));        
       max_wait.tv_sec = noOfSecondsToWait;    
    
       struct timespec abs_time;
       pthread_t tid;
       int err;
        
       pthread_mutex_lock(&calculating);

        /* pthread cond_timedwait expects an absolute time to wait until */
        clock_gettime(CLOCK_REALTIME, &abs_time);
        abs_time.tv_sec += max_wait.tv_sec;
        abs_time.tv_nsec += max_wait.tv_nsec;
        
        sleep(random(0,2));//wait a no of sec for attack prevention
             
        contact contactNodes;
        contactNodes.node=this;
        contactNodes.selectedNode=selectedNode;
        contactNodes.tr=tr;
        contactNodes.queryParams=queryParams;
        
        pthread_create(&tid, NULL, contact_node,(void*)&contactNodes);

        err = pthread_cond_timedwait(&done, &calculating, &abs_time);
       
        char * temp;

        if (!err){
            pthread_mutex_unlock(&calculating);
            pthread_join(tid, (void**)&temp);
            return string(temp);
        }
        pthread_cancel(tid);
        pthread_mutex_unlock(&calculating);
        return string("");
}


/*
 * Insert dummy query
 * 
 */
void ChordNode::randomWalk(string key){   
    Query *query=new Query(1,1);
    allQueries.push_back(query);
    query->addFingerTable(fingerTable,thisNode);
    char *buffer;

    //Faze one
    for(int iterator=0;iterator<query->getL();iterator){
        vector<Node *> currentTable=query->getLastFingerTableEntry();
        
        string serializedTable;
        
        Node *selectedNode;
                
        do{
            int randomNode=random(0,fingerTable.size()-1);//select a random node from current finger table
        
            selectedNode=currentTable[randomNode];//select the node
            
            //check if it wasn't selected before
            if(query->hasBeenSelected(selectedNode)==false){
                map<string,string> queryParams;      
                serializedTable = send_request_with_timeout(selectedNode,GETFINGERTABLE,4,queryParams);
            }
        }while(serializedTable.size()==0);
        
        //if duplicate
        if(query->addFingerTable(deserialize(serializedTable)->getFingerTable(),selectedNode)==true){
            iterator++;
        }   
    }//at the end in selectedNodes we will have the chain of nodes
    
    //Faze two
        
    Node* A=query->getSelectedNodes().front();//get first selected nod of first part of random walk
    
    ChordNode *tempNode=new ChordNode();
    tempNode->setFingerTable(query->getSelectedNodes());//Crypt nodes in getSelectedNodes with all the node keys
    
    map<string,string> queryParams;
    queryParams["table"]=tempNode->serialize(tempNode);
    queryParams["hash"]=query->getQueryHash();
    queryParams["l"]=string(itoa(query->getL(),buffer,10));
    queryParams["key"]=key;//SHOULD BE CRIPTED WITH LAST NODE KEY

    //queryParams["public_key"] for last node to crypt the selected nodes

    
    string response = send_request_with_timeout(A,RANDOMWALKCONTACT,10,queryParams);   
    //inside response are NODES or fail
    
    
    cout<<response.c_str();
}