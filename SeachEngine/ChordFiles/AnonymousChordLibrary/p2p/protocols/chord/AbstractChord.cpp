#include "AbstractChord.h"
#include "ProtocolSingleton.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <random>  


// /////////////////////////////////////////// //
//                  INITIALISE                 //
// /////////////////////////////////////////// //

void AbstractChord::initialise(string ip, int id, int port) {
    thisNode = new Node(ip, id, port);
    successor = thisNode;
    predecessor = thisNode;
    next = 0; // C++ we have to set next to zero to avoid possible garbage...
    timeToCheck = 2000; // miliSecond

    for (int i = 0; i < spacesize; i++) {
        fingerTable.push_back(thisNode);
    }
}

// /////////////////////////////////////////// //
//               CHORD ALGORITHM               //
// /////////////////////////////////////////// //

Node* AbstractChord::findSuccessor(int id) {
    if (insideRange(id, thisNode->getId() + 1, successor->getId()))
        return successor;

    Node *pred = closestPrecedingNode(id);

    //Forge the message that we will sendRequest (FINDSUCC)
    std::ostringstream oss;
    oss << id;

    //Create a request.
    Request *request = new Request(this->getIdentifier(), FINDSUCC);
    request->addArg("id", oss.str());

    string succ = sendRequest(request, pred);

    return new Node(succ);
}

bool AbstractChord::getSignatureFromCA(Node* CA) {
    if (CA != NULL) {
        std::ostringstream oss;
        oss << this->getThisNode()->getPort();

        //Create a request.
        Request *request = new Request(this->getIdentifier(), SIGNOD);
        request->addArg("ip", this->getThisNode()->getIp());
        request->addArg("port", oss.str());
        request->addArg("nid", this->getThisNode()->getIdString());

        string signature = sendRequest(request, CA);
        this->getThisNode()->setSignature(signature);

        return true;
    }
    return false;
}

bool AbstractChord::verifyNodeSignature(Node* node, string signature, Node* CA) {
    if (CA != NULL) {
        std::ostringstream oss;
        oss << node->getPort();

        //Create a request.
        Request *request = new Request(this->getIdentifier(), SIGVERIFY);
        request->addArg("ip", node->getIp());
        request->addArg("port", oss.str());
        request->addArg("nid", node->getIdString());
        request->addArg("signature", signature);

        string response = sendRequest(request, CA);

        if (response.compare("true") == 0)
            return true;
        return false;
    }
    return false;
}

Node* AbstractChord::closestPrecedingNode(int nid) {
    // optimization
    if (thisNode == successor) {
        return thisNode;
    }
    for (int i = fingerTable.size() - 1; i > 0; i--) {
        if (insideRange(fingerTable[i]->getId(), thisNode->getId() + 1, nid - 1)) {
            return fingerTable[i];
        }
    }
    return successor;
}

void AbstractChord::join(Node* chord) {
    //Forge the actual request.
    Request *request = new Request(this->getIdentifier(), FINDSUCC);
    request->addArg("overlay_id", this->getIdentifier());
    request->addArg("id", thisNode->getIdString());

    //Send the request.
    string succ = this->sendRequest(request, chord);

    // update the successor
    successor = new Node(succ);
}

void AbstractChord::stabilize() {
    //Forge the message that we will sendRequest (GETPRED)
    Request *pred_request = new Request(this->getIdentifier(), GETPRED);
    string pred = sendRequest(pred_request, successor);

    Node *x = new Node(pred);
    if (x->getId() != thisNode->getId() && insideRange(x->getId(),
            thisNode->getId() + 1, successor->getId() - 1)) {
        successor = x;
    }

    //Forge the message that we will sendRequest (NOTIF)
    Request *notif_request = new Request(this->getIdentifier(), NOTIF);
    notif_request->addArg("node", thisNode->toString());
    sendRequest(notif_request, successor);
}

void AbstractChord::notify(Node *node) {
    if ((predecessor->getId() == thisNode->getId()) ||
            (insideRange(node->getId(), predecessor->getId() + 1, thisNode->getId() - 1))) {
        predecessor = node;
    }
}

void AbstractChord::fixFingersTable() {
    next++;
    if (next > spacesize) {
        next = 1;
    }
    fingerTable[next - 1] = findSuccessor((thisNode->getId() + (int) pow(2,
            next - 1)) % (int) pow(2, spacesize));
}

/* called periodically. checks whether predecessor has failed*/
void AbstractChord::checkPredecessor() {
    Request *request = new Request(this->getIdentifier(), GETPRED);
    sendRequest(request, predecessor);
}

// /////////////////////////////////////////// //
//               HELPER METHODS                //
// /////////////////////////////////////////// //

bool AbstractChord::insideRange(int id, int begin, int end) {
    int MAXid = pow(2, spacesize);
    int MINid = 0;

    return (begin < end && begin <= id && id <= end) ||
            (begin > end && ((begin <= id && id <= MAXid) || (MINid <= id && id <= end))) ||
            ((begin == end) && (id == begin));

}

string AbstractChord::printStatus() {
    stringstream ss(stringstream::in | stringstream::out);

    ss << getIdentifier() << " on " << thisNode->getIp() << ":"
            << thisNode->getPort() << "\n" << "<NODE: " << thisNode->getId()
            << ", PRED: " << predecessor->getId() << ", SUCC: "
            << successor->getId() << ">\n" << "\tFingers Table: [";
    for (int i = 0; i < fingerTable.size() - 1; i++) {
        ss << fingerTable[i]->getId() << ", ";
    }
    ss << fingerTable[fingerTable.size() - 1]->getId() << "]\n\n";

    return ss.str();
}

int AbstractChord::random(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

int AbstractChord::random() {
    random_device rd;
    return rd();
}
