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
void call_chord_getfingertable(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data){
    
  
    std::ostringstream ofs;
    boost::archive::text_oarchive oa(ofs);
    ChordNode *node=P_SINGLETON->getChordNode();
    ofs.flush();
    oa<<node;
    
    mg_printf(conn, ofs.str().c_str());
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
