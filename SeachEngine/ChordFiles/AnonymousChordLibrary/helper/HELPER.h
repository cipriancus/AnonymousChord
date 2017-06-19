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
#include "ChordTransportCode.h"
using namespace std;

class HELPER {
public: 
    static long int deserializeLongInt(string data);
    static string     serializeLongInt(long int x);
    static transportCode getEnumForInt(int no);
    static void reverse(char str[], int length);
    static char* itoa(int num, char* str, int base);

};

#endif /* HELPER_H */

