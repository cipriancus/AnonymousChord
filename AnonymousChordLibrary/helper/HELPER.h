/* 
 * File:   HELPER.h
 * Author: ciprian
 *
 * Created on March 27, 2017, 11:41 PM
 */

#ifndef HELPER_H
#define HELPER_H


#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
using namespace std;

class HELPER {
public: 
    static long int deserializeLongInt(string data);
    static string     serializeLongInt(long int x);
};

#endif /* HELPER_H */

