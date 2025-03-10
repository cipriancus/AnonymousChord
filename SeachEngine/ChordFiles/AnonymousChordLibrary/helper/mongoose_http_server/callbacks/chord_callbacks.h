/*
 *
 *	This file contains all the callbacks to effectively handle the Chord Protocol
 *	via the HTTP server.
 */

#ifndef CHORD_CALLBACKS_H
#define CHORD_CALLBACKS_H

void call_chord_getpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_findsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_notif(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_join(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_put(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_get(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_removekey(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_setsucc(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_setpred(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_hash(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_shutdown(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_signod(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data) ;

void call_chord_sigverify(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data);

void call_chord_getfingertable(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data);

void call_chord_contact(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data);

void call_chord_getkey(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data);

void call_chord_getNewNode(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_getOnlineTime(struct mg_connection *conn,
                const struct mg_request_info *request_info, void *user_data);

void call_chord_closestnotetokey(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);

void call_chord_getquerykey(struct mg_connection *conn,
		const struct mg_request_info *request_info, void *user_data);
#endif
