/*
 *  chord_callbacks.cpp
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010

 *
 */

#include "chord_callbacks.h"
#include "ProtocolSingleton.h"
#include "mongoose.h"
#include "http_operations.h"
#include "sha1.h"
#include "HELPER.h"
#include <assert.h>
/*
 *	/getpred callback, handles a get_predecessor peer request.
 */
void call_chord_getpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	string result;

	if (P_SINGLETON->getChordNode() != NULL) {
		result = P_SINGLETON->getChordNode()->getPredecessor()->toString();
	} else {

		assert(P_SINGLETON->getChordNode() == NULL);
	}

	//Display answer as a webpage to the peer requesting predecessor
	mg_printf(conn, "%s", result.c_str());

}

/*
 *	/findsucc callback, handles a find_successor peer request.
 */
void call_chord_findsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *id = NULL;

	//assert(mg_get_var(conn, "overlay_id") != NULL); <== NEEDS TO BE DONE BEFORE!!! XD
	assert((id = mg_get_var(conn, "id")) != NULL);

	result = P_SINGLETON->getChordNode()->findSuccessor(atoi(id))->toString();

	mg_printf(conn, result.c_str());

	//Release the allocated memory for id variable.
	mg_free(id);
}

/*
 *	/notif callback, handles a notification peer request.
 */
void call_chord_notif(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char *n = NULL;
	//assert(mg_get_var(conn, "overlay_id") != NULL); <== NEEDS TO BE DONE BEFORE!!! XD
	assert((n = mg_get_var(conn, "node")) != NULL);

	Node *node = new Node(n);
	P_SINGLETON->getChordNode()->notify(node);

	//	//Release the allocated memory for id variable.
	mg_free(n);
}

/*
 *	/join callback, handles a join chord peer request.
 */
void call_chord_join(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data)

{
	
}

/*
 *	/put callback, handles a put peer request.
 */
void call_chord_put(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char *key = NULL;
	char *value = NULL;

	assert((key = mg_get_var(conn, "key")) != NULL);
	assert((value = mg_get_var(conn, "value")) != NULL);

	P_SINGLETON->getChordNode()->put(key, value);

	//Release the allocated memory for id variable.
	mg_free(key);
	mg_free(value);
}

/*
 *	/get callback, handles a get peer request.
 */
void call_chord_get(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *key = NULL;

	assert((key = mg_get_var(conn, "key")) != NULL);

	result = P_SINGLETON->getChordNode()->get(key);
	mg_printf(conn, result.c_str());

	//Release the allocated memory for id variable.
	mg_free(key);
}

//calls for a digital signature
void call_chord_signod(struct mg_connection *conn,const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *ip  = NULL;
	char *port = NULL;
	char *nid  = NULL;

        
	assert((ip = mg_get_var(conn, "ip")) != NULL);
        assert((port = mg_get_var(conn, "port")) != NULL);
	assert((nid = mg_get_var(conn, "nid")) != NULL);
        assert(P_SINGLETON->getMode().compare("CA")==0);
          
	result = P_SINGLETON->getCA()->signNode(string(ip),atoi(port),atoi(nid));
        
	mg_printf(conn, result.c_str());

	//Release the allocated memory for id variable.
	mg_free(ip);
        mg_free(port);
	mg_free(nid);

}
//cals for a node signature verification
void call_chord_sigverify(struct mg_connection *conn,const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *ip  = NULL;
	char *port = NULL;
	char *nid  = NULL;
	char *signature  = NULL;

        
	assert((ip = mg_get_var(conn, "ip")) != NULL);
        assert((port = mg_get_var(conn, "port")) != NULL);
	assert((nid = mg_get_var(conn, "nid")) != NULL);
        assert((signature = mg_get_var(conn, "signature")) != NULL);
        assert(P_SINGLETON->getMode().compare("CA")==0);
          
	if(P_SINGLETON->getCA()->verifyNode(string(ip),atoi(port),atoi(nid),signature)==1){
            result=string("true");
        }else{
            result=string("false");
        }
            
        mg_printf(conn, result.c_str());

	//Release the allocated memory for id variable.
	mg_free(ip);
        mg_free(port);
	mg_free(nid);

}

void call_chord_closestnotetokey(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *id = NULL;

	assert((id = mg_get_var(conn, "id")) != NULL);
        
        result=P_SINGLETON->getChordNode()->getDataOrPrecedingNode(id);
	
	mg_printf(conn, result.c_str());

	mg_free(id);
}

void call_chord_getfingertable(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data){    
 
    time_t time=P_SINGLETON->getChordNode()->getStartTime();
    long int start_time=static_cast<int>(time);
   
    mg_printf(conn,HELPER::serializeLongInt(start_time).c_str());
}

void call_chord_getOnlineTime(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data){    
 
    ChordNode *node=P_SINGLETON->getChordNode();

    const char *p=HELPER::serializeLongInt(P_SINGLETON->getChordNode()->getStartTime()).c_str();
    
    mg_printf(conn, p);
}

void call_chord_getNewNode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	string result;
	char *hashch = NULL;
        char *failednodech = NULL;
        
	assert((hashch = mg_get_var(conn, "hash")) != NULL);
	assert((failednodech = mg_get_var(conn, "failed_node")) != NULL);

        string hash=string(hashch);
        

        Query *query;
        
        //my query 
	if(P_SINGLETON->getChordNode()->getQueryForHash(hash,query)==true){
            //Get Failed Node
            Node *failed_node=new Node();
            failed_node=failed_node->deserializeNode(string(failednodech));
        
            int times=0;
            string response;
            int noOfSeconds=2;
            map<string,string> query_params;
            do{
                //test if nodes respond
                response=P_SINGLETON->getChordNode()->send_request_with_timeout(failed_node,GETONLINETIME,noOfSeconds,query_params);
                times++;
            }while(response.size()!=0 &times<3);
            
            if(times<3){//node is on
                long int interogration_time=HELPER::deserializeLongInt(mg_get_var(conn,"time_of_fail"));//time node was declared dead time_of_fail
                long int node_on_time=HELPER::deserializeLongInt(response);//time node went online
                
                if(interogration_time>node_on_time){
                    //report to CA
                    response=string("abort");
                    mg_printf(conn, response.c_str());
                }
                
                //truth has been told, the node was offline
                //hashes are the same
                //select new nodes
                Node *node=query->selectNewNode(P_SINGLETON->getChordNode()->getFingerTable());
                string serializedNode=node->serializeNode();
                mg_printf(conn, serializedNode.c_str());
            }
        }else{
            //nu i a meu, dam mai departe
            vector<Node*> query=P_SINGLETON->getChordNode()->getPassedQueryForHash(hash);
            if(query.size()>0){
                //am gasit
                Node *predecesor=query[0];
                Node *succesor=query[1];
                
                if(succesor->getIp().compare(string(returnIP(conn)))==0){//succesorul e corect
                    string response;
                    int times=0;        
                    map<string,string> query_params;
                    query_params["hash"]=hash;
                    int noOfSeconds=2;

                    do{
                        //test if nodes respond
                        response=P_SINGLETON->getChordNode()->send_request_with_timeout(predecesor,GETNNEWNODE,noOfSeconds,query_params);
                        times++;
                    }while(response.size()!=0 &times<3);
                  
                    //it failed, return a failure
                    if(times==3){
                        response=string("failed");
                    }
                    
                    mg_printf(conn, response.c_str());
                }else{
                   string response=string("failed");
                   mg_printf(conn, response.c_str());
                }    
            }
        }
}


void call_chord_contact(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data){
    
  char *tablech = NULL;
  char *hashch = NULL;
  char *lch = NULL;
  char *keych = NULL;
  char *enumcommandch = NULL;
  char *last_nodech = NULL;

  tablech  = mg_get_var(conn, "table");
  assert((hashch = mg_get_var(conn, "hash")) != NULL);
  assert((lch = mg_get_var(conn, "l")) != NULL);
  assert((keych = mg_get_var(conn, "id")) != NULL);
  assert((enumcommandch = mg_get_var(conn, "enumeration_command")) != NULL);
  assert((last_nodech = mg_get_var(conn, "last_node")) != NULL);

  string l=string(lch);
  string tablestr=string(tablech);
  string hash=string(hashch);
  string key=string(keych);
  string enumcommand=string(enumcommandch);
  string last_node=string(last_nodech);
  /*
  ADAUG INFO despre pred si succ 
   */
  vector<Node*> predSucc;
  assert((predSucc[0] = P_SINGLETON->getChordNode()->getNodeForIP(string(returnIP(conn)))) != NULL);
  predSucc[1]=P_SINGLETON->getChordNode()->getThisNode();
  P_SINGLETON->getChordNode()->addPassedQuery(hash,predSucc);
  
  ChordNode *tempNode=P_SINGLETON->getChordNode()->deserialize(tablestr);
  
  vector<Node*> table=tempNode->getFingerTable();
  
  if(table.size()!=0){//I am not final note in first step
    Node* node=table.front();
    table.erase(table.begin(),table.begin()+1); 
    int noOfSeconds=2;

    if(table.size()!=0){//nodul selectat nu e ultimul
        tempNode=new ChordNode();
        tempNode->setFingerTable(table);

        map<string,string> query_params;
        query_params["table"]=tempNode->serialize(tempNode);
        query_params["hash"]=hash;
        query_params["l"]=l;
        query_params["id"]=key;
        query_params["enumeration_command"]=enumcommand;//13=RANDOMWALKCONTACT
        query_params["last_node"]=last_node;//13=RANDOMWALKCONTACT

            string response;
            int times=0;        
            int noOfSeconds=2;

            do{
                response=P_SINGLETON->getChordNode()->send_request_with_timeout(node,RANDOMWALKCONTACT,noOfSeconds,query_params);
                times++;
            }while(response.size()!=0 &times<3);
        
            if(times==3||response.compare("failed")==0){
                //failed
                string serializedNode=node->serializeNode();//give the failed node for inspection
                query_params["failed_node"]=serializedNode;//ar trebui criptat cu cheia publica a Query ului ca sa nu fie fazut de toti
                query_params["time_of_fail"]=HELPER::serializeLongInt(time(nullptr));
                do{
                    response=P_SINGLETON->getChordNode()->send_request_with_timeout(predSucc[0],GETNNEWNODE,noOfSeconds,query_params);
                    times++;
                }while(response.size()!=0 &times<3);
                
                if(times==3){
                    response=string("failed");
                }              
            }
        mg_printf(conn, response.c_str());
    }else{
        //e ultimul nod selectat
        //ii cer sa initieze ceea ce scrie in enumeration_command
        Node *contact=new Node();
        contact=contact->deserializeNode(last_node);
        
        map<string,string> query_params;
        query_params["table"]=tempNode->serialize(tempNode);
        query_params["hash"]=hash;
        query_params["l"]=l;
        query_params["id"]=key;
        
        char *buffer;
        string response=P_SINGLETON->getChordNode()->send_request_with_timeout(contact,HELPER::getEnumForInt(atoi(enumcommand.c_str())),noOfSeconds,query_params);
        mg_printf(conn, response.c_str());
    } 
  }
}

void call_chord_getkey(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data){
  
    char *tablech = NULL;
    char *hashch = NULL;
    char *lch = NULL;
    char *keych = NULL;

    tablech  = mg_get_var(conn, "table");
    assert((hashch = mg_get_var(conn, "hash")) != NULL);
    assert((lch = mg_get_var(conn, "l")) != NULL);
    assert((keych = mg_get_var(conn, "id")) != NULL);
    
    string response;
    string l=string(lch);
    string tablestr=string(tablech);
    string hash=string(hashch);
    string key=string(keych);
    
    vector<Node*> predSucc;
    assert((predSucc[0] = P_SINGLETON->getChordNode()->getNodeForIP(string(returnIP(conn)))) != NULL);
    predSucc[1]=P_SINGLETON->getChordNode()->getThisNode();
    P_SINGLETON->getChordNode()->addPassedQuery(hash,predSucc);
    
    vector<Node*> interogatedNodes;
    
    do{
        Query *temporaryQuery=new Query(atoi(l.c_str()));
        
        P_SINGLETON->getChordNode()->phaseOne(temporaryQuery);//select randomwalk nodes
                
        char *buffer;
        
        ChordNode *tempNode=new ChordNode();
        tempNode->setFingerTable(temporaryQuery->getSelectedNodes());
     
        Node* node=temporaryQuery->getSelectedNodes().front();
        
        map<string,string> queryParams;
        queryParams["table"]=tempNode->serialize(tempNode);
        queryParams["hash"]=temporaryQuery->getQueryHash();
        queryParams["l"]=string(P_SINGLETON->getChordNode()->itoa(temporaryQuery->getL(),buffer,10));
        queryParams["id"]=key;//SHOULD BE CRIPTED WITH LAST NODE KEY
        queryParams["enumeration_command"]=string(P_SINGLETON->getChordNode()->itoa(17,buffer,10));//CLOSESTNODETOKEY
        queryParams["last_node"]=interogatedNodes.back()->serializeNode();
        //queryParams["public_key"] for last node to crypt the selected nodes
        
        int times=0;        
        int noOfSeconds=2;

        do{
            response=P_SINGLETON->getChordNode()->send_request_with_timeout(node,RANDOMWALKCONTACT,noOfSeconds,queryParams);
            times++;
         }while(response.size()!=0 &times<3);
        
                if(times==3||response.compare("failed")==0){
                    //failed
                    string serializedNode=node->serializeNode();//give the failed node for inspection
                    queryParams["failed_node"]=serializedNode;//ar trebui criptat cu cheia publica a Query ului ca sa nu fie fazut de toti
                    queryParams["time_of_fail"]=HELPER::serializeLongInt(time(nullptr));
                    do{
                        response=P_SINGLETON->getChordNode()->send_request_with_timeout(predSucc[0],GETNNEWNODE,noOfSeconds,queryParams);
                        times++;
                    }while(response.size()!=0 &times<3);

                    if(times==3){
                        response=string("failed");
                    }       
                }
            //responce has A NODE
            Node *response_node=new Node();
            response_node=response_node->deserializeNode(response);
            
            if(response_node->getIp().size()!=0){//if it really had a node
                interogatedNodes.push_back(response_node);
            }else{
                //I have a value for the key
                break;
            }
    }while(1);
    mg_printf(conn, response.c_str());
}

/*
 *	/get callback, handles a get peer request.
 */
void call_chord_removekey(struct mg_connection *conn,
					const struct mg_request_info *request_info, void *user_data) {
	char *key = NULL;

	assert((key = mg_get_var(conn, "key")) != NULL);

	P_SINGLETON->getChordNode()->removekey(key);

	//Release the allocated memory for id variable.
	mg_free(key);
}

/*
 *	/setsucc callback, handles a set successor peer request.
 */
void call_chord_setsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char *n = NULL;
	assert((n = mg_get_var(conn, "successor")) != NULL);

	Node *node = new Node(n);
	P_SINGLETON->getChordNode()->setSuccessor(node);

	//	//Release the allocated memory for id variable.
	mg_free(n);
}

/*
 *	/setpred callback, handles a set predecessor peer request.
 */
void call_chord_setpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char *n = NULL;
	//assert(mg_get_var(conn, "overlay_id") != NULL); <== NEEDS TO BE DONE BEFORE!!! XD
	assert((n = mg_get_var(conn, "predecessor")) != NULL);

	Node *node = new Node(n);
	P_SINGLETON->getChordNode()->setPredecessor(node);

	//Release the allocated memory for id variable.
	mg_free(n);
}

/*
 * /hash callback, handles a hash value request
 */
void call_chord_hash(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data) {
	char *key = NULL;

	assert((key = mg_get_var(conn, "key")) != NULL);
	string result = P_SINGLETON->getChordNode()->getHexSHA1(key);
	mg_printf(conn, result.c_str());

	// Release the allocated memory for id variable.
	mg_free(key);
}

/*
 * /shutdown callback, handles a shutDown value request
 */
void call_chord_shutdown(struct mg_connection *conn,
					 const struct mg_request_info *request_info, void *user_data) {
//	P_SINGLETON->getChordNode()->shutDown();
}
