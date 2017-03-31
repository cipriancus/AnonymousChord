/*
 *  ChordTransportCode.h
 *  iPhone_p2p_engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#ifndef CHORD_TRANSPORT_CODE_H
#define CHORD_TRANSPORT_CODE_H

typedef enum {
	GETPRED = 0, //GETPRED 0
	FINDSUCC, //FINDSUCC,<id>1
	NOTIF, //NOTIF,<ip>,<id>,<port>2
	JOIN,//3
	PUT,//4
	GET,//5
	REMOVEKEY,//6
	SETSUCC,//7
	SETPRED,//8
	CHECKPRED,//9
        SIGNOD,//10
        SIGVERIFY,//11
        GETFINGERTABLE,//get finger table from a node 12
        RANDOMWALKCONTACT,//call_chord_contact for selecting a path 13
        GETNNEWNODE,//get a new node for a random walk 14
        GETONLINETIME,//15
        RANDOMWALKGETKEY,//16
        CLOSESTNODETOKEY//17 get closest node to a key
} transportCode;

#endif
