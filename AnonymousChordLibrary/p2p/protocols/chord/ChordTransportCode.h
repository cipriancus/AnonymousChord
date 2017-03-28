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
	GETPRED = 0, //GETPRED
	FINDSUCC, //FINDSUCC,<id>
	NOTIF, //NOTIF,<ip>,<id>,<port>
	JOIN,
	PUT,
	GET,
	REMOVEKEY,
	SETSUCC,
	SETPRED,
	CHECKPRED,
        SIGNOD,
        SIGVERIFY,
        GETFINGERTABLE,//get finger table from a node
        RANDOMWALKCONTACT,//call_chord_contact for selecting a path
        GETNNEWNODE,//get a new node for a random walk
        GETONLINETIME,
        RANDOMWALKGETKEY
} transportCode;

#endif
