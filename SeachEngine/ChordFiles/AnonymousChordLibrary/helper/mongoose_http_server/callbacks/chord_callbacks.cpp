#include "chord_callbacks.h"
#include "ProtocolSingleton.h"
#include "mongoose.h"
#include "http_operations.h"
#include "sha1.h"
#include "HELPER.h"
#include "Exception.h"
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
    char *id = NULL;

    //assert(mg_get_var(conn, "overlay_id") != NULL); <== NEEDS TO BE DONE BEFORE!!! XD
    id = mg_get_var(conn, "id");
    if (id != NULL) {
        mg_printf(conn, P_SINGLETON->getChordNode()->findSuccessor(atoi(id))->toString().c_str());

        //Release the allocated memory for id variable.
        mg_free(id);
    }
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
        const struct mg_request_info *request_info, void *user_data) {

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
    char *key = NULL;

    assert((key = mg_get_var(conn, "key")) != NULL);

    mg_printf(conn, P_SINGLETON->getChordNode()->get(key).c_str());

    //Release the allocated memory for id variable.
    mg_free(key);
}

//calls for a digital signature

void call_chord_signod(struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data) {
    char *ip = NULL;
    char *port = NULL;
    char *nid = NULL;

    assert((ip = mg_get_var(conn, "ip")) != NULL);
    assert((port = mg_get_var(conn, "port")) != NULL);
    assert((nid = mg_get_var(conn, "nid")) != NULL);
    assert(P_SINGLETON->getMode().compare("CA") == 0);

    mg_printf(conn, P_SINGLETON->getCA()->signNode(string(ip), atoi(port), atoi(nid)).c_str());

    //Release the allocated memory for id variable.
    mg_free(ip);
    mg_free(port);
    mg_free(nid);

}
//cals for a node signature verification

void call_chord_sigverify(struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data) {
    string result;

    char *ip = NULL;
    char *port = NULL;
    char *nid = NULL;
    char *signature = NULL;


    assert((ip = mg_get_var(conn, "ip")) != NULL);
    assert((port = mg_get_var(conn, "port")) != NULL);
    assert((nid = mg_get_var(conn, "nid")) != NULL);
    assert((signature = mg_get_var(conn, "signature")) != NULL);
    assert(P_SINGLETON->getMode().compare("CA") == 0);

    if (P_SINGLETON->getCA()->verifyNode(string(ip), atoi(port), atoi(nid), signature) == 1) {
        result = string("true");
    } else {
        result = string("false");
    }

    mg_printf(conn, result.c_str());

    //Release the allocated memory for id variable.
    mg_free(ip);
    mg_free(port);
    mg_free(nid);
    mg_free(signature);
}

void call_chord_closestnotetokey(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {

    char *id = NULL;
    assert((id = mg_get_var(conn, "id")) != NULL);
    mg_printf(conn, P_SINGLETON->getChordNode()->getDataOrPrecedingNode(id).c_str());
    mg_free(id);
}

void call_chord_getfingertable(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {

    mg_printf(conn, P_SINGLETON->getChordNode()->serialize(P_SINGLETON->getChordNode()).c_str());
}

void call_chord_getOnlineTime(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {
    mg_printf(conn, HELPER::serializeLongInt(P_SINGLETON->getChordNode()->getStartTime()).c_str());
}

void call_chord_getNewNode(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {
    //	string result;
    //	char *hashch = NULL;
    //        char *failednodech = NULL;
    //        
    //	assert((hashch = mg_get_var(conn, "hash")) != NULL);
    //	assert((failednodech = mg_get_var(conn, "failed_node")) != NULL);
    //
    //        string hash=string(hashch);
    //        
    //
    //        Query *query;
    //        
    //        //my query 
    //	if(P_SINGLETON->getChordNode()->getQueryForHash(hash,query)==true){
    //            //Get Failed Node
    //            Node *failed_node=new Node();
    //            failed_node=failed_node->deserializeNode(string(failednodech));
    //        
    //            int times=0;
    //            string response;
    //            int noOfSeconds=2;
    //            map<string,string> query_params;
    //            do{
    //                //test if nodes respond
    //                response=P_SINGLETON->getChordNode()->send_request_with_timeout(failed_node,GETONLINETIME,noOfSeconds,query_params);
    //                times++;
    //            }while(response.size()==0 || times < 3);
    //            
    //            if(times<3){//node is on
    //                long int interogration_time=HELPER::deserializeLongInt(mg_get_var(conn,"time_of_fail"));//time node was declared dead time_of_fail
    //                long int node_on_time=HELPER::deserializeLongInt(response);//time node went online
    //                
    //                if(interogration_time>node_on_time){
    //                    //report to CA
    //                    response=string("abort");
    //                    mg_printf(conn, response.c_str());
    //                }
    //                
    //                //truth has been told, the node was offline
    //                //hashes are the same
    //                //select new nodes
    //                Node *node=query->selectNewNode(P_SINGLETON->getChordNode()->getFingerTable());
    //                string serializedNode=node->serializeNode();
    //                mg_printf(conn, serializedNode.c_str());
    //            }
    //        }else{
    //            //nu i a meu, dam mai departe
    //            vector<Node*> query=P_SINGLETON->getChordNode()->getPassedQueryForHash(hash);
    //            if(query.size()>0){
    //                //am gasit
    //                Node *predecesor=query[0];
    //                Node *succesor=query[1];
    //                
    //                if(succesor->getIp().compare(string(returnIP(conn)))==0){//succesorul e corect
    //                    string response;
    //                    int times=0;        
    //                    map<string,string> query_params;
    //                    query_params["hash"]=hash;
    //                    int noOfSeconds=2;
    //
    //                    do{
    //                        //test if nodes respond
    //                        response=P_SINGLETON->getChordNode()->send_request_with_timeout(predecesor,GETNNEWNODE,noOfSeconds,query_params);
    //                        times++;
    //                    }while(response.size()==0 || times < 3);
    //                  
    //                    //it failed, return a failure
    //                    if(times==3){
    //                        response=string("failed");
    //                    }
    //                    
    //                    mg_printf(conn, response.c_str());
    //                }else{
    //                   string response=string("failed");
    //                   mg_printf(conn, response.c_str());
    //                }    
    //            }
    //        }
}

#include<openssl/bio.h>

void call_chord_getquerykey(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {

    string query_hash = string(mg_get_var(conn, "query_hash"));
    string l = string(mg_get_var(conn, "l"));

    ChordNode *chord = P_SINGLETON->getChordNode();

    Query *query = chord->getHandledQueryForHash(query_hash); //see if query exists
    //if exists ask for ip to see if owner
    if (query != NULL) {
        if (query->getOwnerIP().compare(returnIP(conn)) == 0) {//is owner
            string response = query->getPublicKey() + string("+") + query->getIV();

            mg_printf(conn, response.c_str());
        } else {//not owner
            mg_printf(conn, "null");
        }
    } else {//does not exist, create query and give key
        const char *ip = returnIP(conn);

        Query *query = new Query(atoi(l.c_str()), query_hash,string(ip));
        chord->addHandledQuery(query);

        while(query->getPublicKey().size()==0||query->getIV().size()==0){
            query->generateKey();
        }
        
        string response = query->getPublicKey() + string("+") + query->getIV();

        mg_printf(conn, response.c_str());
    }
}

void call_chord_contact(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {

    char *tablech = NULL;
    char *hashch = NULL;
    char *lch = NULL;
    char *keych = NULL;
    char *enumcommandch = NULL;
    char *last_nodech = NULL;

    tablech = mg_get_var(conn, "table");
    assert((hashch = mg_get_var(conn, "hash")) != NULL);
    assert((lch = mg_get_var(conn, "l")) != NULL);
    assert((keych = mg_get_var(conn, "id")) != NULL);
    assert((enumcommandch = mg_get_var(conn, "enumeration_command")) != NULL);
    assert((last_nodech = mg_get_var(conn, "last_node")) != NULL);

    string l = string(lch);
    string tablestr = string(tablech);
    string hash = string(hashch);
    string key = string(keych);
    string enumcommand = string(enumcommandch);
    string last_node = string(last_nodech);
    /*
    ADAUG INFO despre pred si succ 
     */
    vector<Node*> predSucc;
    predSucc.push_back(P_SINGLETON->getChordNode()->getNodeForIP(string(returnIP(conn))));
    predSucc.push_back(P_SINGLETON->getChordNode()->getThisNode());
    P_SINGLETON->getChordNode()->addPassedQuery(hash, predSucc);

    ChordNode *tempNode = P_SINGLETON->getChordNode()->deserialize(tablestr);

    vector<Node*> table = tempNode->getFingerTable();
    int noOfSeconds = 400;

    if (table.size() != 0) {//I am not final note in first step
        Node* node = table.front();
        table.erase(table.begin(), table.begin() + 1);

        tempNode = new ChordNode();
        tempNode->setFingerTable(table);

        map<string, string> query_params;
        query_params["table"] = tempNode->serialize(tempNode);
        query_params["hash"] = hash;
        query_params["l"] = l;
        query_params["id"] = key;
        query_params["enumeration_command"] = enumcommand; //13=RANDOMWALKCONTACT
        query_params["last_node"] = last_node;

        string response;
        int times = 0;

        do {
            response = P_SINGLETON->getChordNode()->send_request_with_timeout(node, RANDOMWALKCONTACT, noOfSeconds, query_params);
            times++;
        } while ((response.size() == 0 || response.compare(string("failed")) == 0) && times < 3);

        if (times == 3 || response.compare("failed") == 0) {
            //failed
            string serializedNode = node->serializeNode(); //give the failed node for inspection
            query_params["failed_node"] = serializedNode; //ar trebui criptat cu cheia publica a Query ului ca sa nu fie fazut de toti
            query_params["time_of_fail"] = HELPER::serializeLongInt(time(nullptr));
            do {
                response = P_SINGLETON->getChordNode()->send_request_with_timeout(predSucc[0], GETNNEWNODE, noOfSeconds, query_params);
                times++;
            } while ((response.size() == 0 || response.compare(string("failed")) == 0) && times < 3);

            if (times == 3) {
                response = string("failed");
            }
        }
        mg_printf(conn, response.c_str());
        delete node;
    } else {
        //e ultimul nod selectat
        //ii cer sa initieze ceea ce scrie in enumeration_command
            Query *query=tempNode->getHandledQueryForHash(hash);
        enumcommand=tempNode->decrypt(enumcommand,reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()),reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()));
        last_node=tempNode->decrypt(last_node,reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()),reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()));
        
        
        Node *contact = new Node();
        contact = contact->deserializeNode(last_node);

        map<string, string> query_params;
        query_params["table"] = tempNode->serialize(tempNode);
        query_params["hash"] = hash;
        query_params["l"] = l;
        query_params["id"] = key;
       
        char *buffer;

        int times = 0;
        string response;
        do {
            response = P_SINGLETON->getChordNode()->send_request_with_timeout(contact, HELPER::getEnumForInt(atoi(enumcommand.c_str())), noOfSeconds, query_params);
            times++;
        } while ((response.size() == 0 || response.compare(string("failed")) == 0) && times < 3);

        if (times == 3) {
            response = string("failed");
        }

        mg_printf(conn, response.c_str());

    }
    mg_free(tablech);
    mg_free(hashch);
    mg_free(lch);
    mg_free(keych);
    mg_free(enumcommandch);
    mg_free(last_nodech);
}

void call_chord_getkey(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {

    char *tablech = NULL;
    char *hashch = NULL;
    char *lch = NULL;
    char *keych = NULL;

    tablech = mg_get_var(conn, "table");
    assert((hashch = mg_get_var(conn, "hash")) != NULL);
    assert((lch = mg_get_var(conn, "l")) != NULL);
    assert((keych = mg_get_var(conn, "id")) != NULL);

    string response;
    string l = string(lch);
    string tablestr = string(tablech);
    string hash = string(hashch);
    string key = string(keych);
    
    ChordNode *tempNode = P_SINGLETON->getChordNode()->deserialize(tablestr);
    
    Query *query=tempNode->getHandledQueryForHash(hash);
    l=tempNode->decrypt(l,reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()),reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()));
    key=tempNode->decrypt(key,reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()),reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()));
   
    vector<Node*> predSucc;
    predSucc.push_back(new Node(string(returnIP(conn)), 8000));
    predSucc.push_back(P_SINGLETON->getChordNode()->getThisNode());


    vector<Node*> interogatedNodes;

    int hKey = P_SINGLETON->getChordNode()->getIntSHA1(key);
    if (P_SINGLETON->getChordNode()->insideRange(hKey, P_SINGLETON->getChordNode()->getPredecessor()->getId() + 1, P_SINGLETON->getChordNode()->getThisNode()->getId())) {
        // I'm responsible for this key
        response = P_SINGLETON->getChordNode()->openData(key);
    } else {//search in other node

        interogatedNodes.push_back(P_SINGLETON->getChordNode()->closestPrecedingNode(hKey)); //closest node to key

        do {
            Query *temporaryQuery = new Query(atoi(l.c_str()));
            temporaryQuery->addFingerTable(P_SINGLETON->getChordNode()->getFingerTable(), P_SINGLETON->getChordNode()->getThisNode());

            P_SINGLETON->getChordNode()->phaseOne(temporaryQuery); //select randomwalk nodes

            char buffer[100];

            Node* node = temporaryQuery->getSelectedNodes().front();
            temporaryQuery->popFrontSelectedNodes();

            ChordNode *tempNode = new ChordNode();
            tempNode->setFingerTable(temporaryQuery->getSelectedNodes());


            map<string, string> queryParams;
            queryParams["table"] = tempNode->serialize(tempNode);
            queryParams["hash"] = temporaryQuery->getQueryHash();
            queryParams["l"] = string(P_SINGLETON->getChordNode()->itoa(temporaryQuery->getL(), buffer, 10));
            queryParams["id"] = key; //SHOULD BE CRIPTED WITH LAST NODE KEY
            queryParams["enumeration_command"] = string(P_SINGLETON->getChordNode()->itoa(17, buffer, 10)); //CLOSESTNODETOKEY
            queryParams["last_node"] = interogatedNodes.back()->serializeNode();

            int times = 0;
            int noOfSeconds = 400;

            do {
                response = P_SINGLETON->getChordNode()->send_request_with_timeout(node, RANDOMWALKCONTACT, noOfSeconds, queryParams);
                times++;
            } while ((response.size() == 0 || response.compare(string("failed")) == 0) && times < 3);

            if (times == 3 || response.compare("failed") == 0) {
                //failed
                string serializedNode = node->serializeNode(); //give the failed node for inspection
                queryParams["failed_node"] = serializedNode; //ar trebui criptat cu cheia publica a Query ului ca sa nu fie fazut de toti
                queryParams["time_of_fail"] = HELPER::serializeLongInt(time(nullptr));
                do {
                    response = P_SINGLETON->getChordNode()->send_request_with_timeout(predSucc[0], GETNNEWNODE, noOfSeconds, queryParams);
                    times++;
                } while ((response.size() == 0 || response.compare(string("failed")) == 0) && times < 3);

                if (times == 3) {
                    response = string("failed");
                }
            }

            delete temporaryQuery;

            if (response.find("serialization") != string::npos) {
                //responce has A NODE
                Node *response_node = new Node();
                response_node = response_node->deserializeNode(response);

                if (response_node->getIp().size() != 0) {//if it really had a node
                    interogatedNodes.push_back(response_node);
                }
            } else {

                break;
            }
        } while (1);
    }  
    response=tempNode->crypt(response,reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()),reinterpret_cast<unsigned char*>((char*)query->getPublicKey().c_str()));
    
    mg_printf(conn, response.c_str());

    mg_free(tablech);
    mg_free(hashch);
    mg_free(lch);
    mg_free(keych);
}

/*
 *	/get callback, handles a get peer request.
 */
void call_chord_removekey(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {
    char *key = NULL;

    assert((key = mg_get_var(conn, "key")) != NULL);

    P_SINGLETON->getChordNode()->removekey(key);

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

    //Release the allocated memory for id variable.
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
    mg_printf(conn, P_SINGLETON->getChordNode()->getHexSHA1(key));

    mg_free(key);
}

/*
 * /shutdown callback, handles a shutDown value request
 */
void call_chord_shutdown(struct mg_connection *conn,
        const struct mg_request_info *request_info, void *user_data) {
    //	P_SINGLETON->getChordNode()->shutDown();
}
