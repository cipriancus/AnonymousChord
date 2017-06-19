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
    oa << x;
    return ofs.str();
}

transportCode HELPER::getEnumForInt(int no) {
    switch (no) {
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
        case 17:
            return CLOSESTNODETOKEY;
        case 18:
            return GETQUERYKEY;
    }
}/* A utility function to reverse a string  */

void HELPER::reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        swap(*(str + start), *(str + end));
        start++;
        end--;
    }
}

// Implementation of itoa()

char* HELPER::itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}