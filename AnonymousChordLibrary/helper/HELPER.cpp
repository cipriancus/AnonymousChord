/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HELPER.cpp
 * Author: ciprian
 * 
 * Created on March 27, 2017, 11:41 PM
 */

#include "HELPER.h"

long int HELPER::deserializeLongInt(string data) {
    long int x;
    istringstream ifs(data);
    boost::archive::text_iarchive ia(ifs);
    ia>>x;
    return x;
}

string HELPER::serializeLongInt(long int x) {
    ostringstream ofs;
    boost::archive::text_oarchive oa(ofs);   
    ofs.flush();
    oa<<x;
    return ofs.str();
}

transportCode HELPER::getEnumForInt(int no){
    switch(no){
        case 0:
            return GETPRED;
        case 1:
            return FINDSUCC;
        case 2:
            return NOTIF;
        case 3:
            return JOIN;
        case 4:
            return PUT;
        case 5:
            return GET;
        case 6:
            return REMOVEKEY;
        case 7:
            return SETSUCC;
        case 8:
            return SETPRED;
        case 9:
            return CHECKPRED;
        case 10:
            return SIGNOD;
        case 11:
            return SIGVERIFY;
        case 12:
            return GETFINGERTABLE;
        case 13:
            return RANDOMWALKCONTACT;
        case 14:
            return GETNNEWNODE;
        case 15:
            return GETONLINETIME;
        case 16:
            return RANDOMWALKGETKEY;
    }
}