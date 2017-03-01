/*
 *  ProtocolSingleton.h
 *  P2P_Engine
 *
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#define P_SINGLETON ProtocolSingleton::getInstance()

#ifndef PROTOCOL_SINGLETON_H
#define PROTOCOL_SINGLETON_H

#include "ChordNode.h"
#include "CertificationAuthority.h"
#include <string>

class ProtocolSingleton
{
public:
	//To obtain the singleton instance.
	static ProtocolSingleton* getInstance();
	
	//Destructor.
	~ProtocolSingleton();

	//Action methods.
	ChordNode*	initChordNode(const string &ip, int port, const string &overlayIntifier, const string &rootDirectory);
        CertificationAuthority*	initCA(const string &ip, int port, const string &overlayIntifier, const string &rootDirectory);

	void		stopChord();
	
	//Getters & setters.
	void		setProtocolNode(ChordNode *n) { myNode = n ;}
	ChordNode*	getChordNode() { return myNode; }
	CertificationAuthority*	getCA() { return myCA; }
        string getMode()  {return mode;}
protected:
	ProtocolSingleton();
	
private:
        string mode;//define the mode in which the Protocol is used, Node or CA
	ChordNode *myNode;//This could be more generic ( ChordNode* is hard coded. )
	CertificationAuthority *myCA;
        static ProtocolSingleton *instance;//The singleton instance of the protocol container.
};

#endif
