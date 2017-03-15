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
#include "sha1.h";
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



void *contact_node(void *args){
    int oldtype;

   contact *contactNodes=(contact*)args;
        
    ChordNode *node=contactNodes->node;
    Node *selectedNode=contactNodes->selectedNode;
    
    /* allow the thread to be killed at any time */
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
        
    Request *request = new Request(node->getIdentifier(), GETFINGERTABLE);
    string response=node->sendRequest(request, selectedNode);//the serialized finger table from the contacted node
    
    pthread_cond_signal(&node->done);
   
    char *responseCh=(char*)malloc(response.size()*sizeof(char));
    strcpy(responseCh,response.c_str());
    return (void*)responseCh;
}

string ChordNode::getFingerTableFromPeer(list<Node*> *selectedNodes,vector<Node*> *currentTable){
        struct timespec max_wait;
        memset(&max_wait, 0, sizeof(max_wait));        
        max_wait.tv_sec = 2;    
    
       struct timespec abs_time;
       pthread_t tid;
       int err;
        
       pthread_mutex_lock(&calculating);

        /* pthread cond_timedwait expects an absolute time to wait until */
        clock_gettime(CLOCK_REALTIME, &abs_time);
        abs_time.tv_sec += max_wait.tv_sec;
        abs_time.tv_nsec += max_wait.tv_nsec;
        
        sleep(random(0,2));//wait a no of sec for attack prevention
        
        int randomNode=random(0,fingerTable.size()-1);//select a random node from current finger table
        
        Node *selectedNode=(*currentTable)[randomNode];//select the node
        selectedNodes->push_back(selectedNode);
         
        
        contact contactNodes;
        contactNodes.node=this;
        contactNodes.selectedNode=selectedNode;
        pthread_create(&tid, NULL, contact_node,(void*)&contactNodes);

        err = pthread_cond_timedwait(&done, &calculating, &abs_time);
       
        char * temp;
        pthread_join(tid, (void**)&temp);

       
        if (!err){
                pthread_mutex_unlock(&calculating);
                
        }

        return string(temp);
}

void ChordNode::randomWalk(){
    //int l=random(fingerTable.size()/2,fingerTable.size());//l-hop random walk
    int l=1;
    
    /**
     * At the end, this node will inspect the 
     * given tables and report to the CA if anything is wrong
     */
    list<nodesVector> allGivenNodes;//list with all fingertables | They must be digitally signed
    list<Node*> selectedNodes;//all selected random nodes
    allGivenNodes.push_back(fingerTable);//my table is first
    
    //Faze one
    for(int iterator=0;iterator<l;iterator++){
        vector<Node *> currentTable=allGivenNodes.back();
         
         
        string serializedTable = getFingerTableFromPeer(&selectedNodes,&currentTable);

        while(serializedTable.size()==0){
            serializedTable = getFingerTableFromPeer(&selectedNodes,&currentTable);
        }
        
        //must deserialize finger table
        ChordNode *tempNode;
        
        std::istringstream ifs(serializedTable);
        boost::archive::text_iarchive ia(ifs);
        ia>>tempNode;
        allGivenNodes.push_back(tempNode->getFingerTable());  
    }//la ultimul pas voi avea ultimul nod din prima parte, cum ar fi B, de pe exemplu
    
    //ar trebui facut ping sa vedem daca e in viata si daca nu, selectam altul

    
    //Faze two
    /*
    B alege un nod C1 dupa functia de la I si ii cere fingertabel-ul.
    B alege un nod D1 dupa functia de la I si ii cere sa caute o informatie.
    B primeste E1.
    Se repeta pasii de la 1 la 3 pana cand se ajunge la informatia cautata.
    */
    
    
}
