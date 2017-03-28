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
