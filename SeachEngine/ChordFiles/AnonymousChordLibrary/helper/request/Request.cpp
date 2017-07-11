#include "Request.h"
#include "ChordTransportCode.h"
#include <curl/curl.h>

/* Constructor */
Request::Request(string overlayID, int code) {
    this->overlayID = overlayID;
    this->code = code;
}

/* Adds an arument to the Request arguments Map. */
void Request::addArg(string key, string value) {

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_SSL);

    curl = curl_easy_init();
    char *output = curl_easy_escape(curl, value.c_str(), value.length());
  
    arguments.insert(arg(key, string(output)));

    /* always cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

/* Request an argument to the request arguments Map. */
string Request::getArg(string key) {
    argMap::iterator it = arguments.find(key);

    if (it != arguments.end()) {
        return (it->second);
    }

    return "";
}

unsigned int Request::getCheckSum(string str) {
    return 0;
}

/* Serialize the whole request into a callback string. "/callback?var1=a&var2=b....&var1000=bar" */
string Request::serialize() {
    argMap::iterator it;

    ostringstream ss;
    string *callback;

    switch (this->getCode()) {
        case GETPRED:
            callback = new string("/getpred");
            break;
        case FINDSUCC:
            callback = new string("/findsucc");
            break;
        case NOTIF:
            callback = new string("/notif");
            break;
        case JOIN:
            callback = new string("/join");
            break;
        case PUT:
            callback = new string("/put");
            break;
        case GET:
            callback = new string("/get");
            break;
        case REMOVEKEY:
            callback = new string("/removekey");
            break;
        case SETSUCC:
            callback = new string("/setsucc");
            break;
        case SETPRED:
            callback = new string("/setpred");
            break;
        case SIGNOD:
            callback = new string("/signod");
            break;
        case SIGVERIFY:
            callback = new string("/sigverify");
            break;
        case GETFINGERTABLE:
            callback = new string("/getfingertable");
            break;
        case RANDOMWALKCONTACT:
            callback = new string("/randomwalkcontact");
            break;
        case GETNNEWNODE:
            callback = new string("/getnewnode");
            break;
        case GETONLINETIME:
            callback = new string("/getonlinetime");
            break;
        case RANDOMWALKGETKEY:
            callback = new string("/randomwalkgetkey");
            break;
        case CLOSESTNODETOKEY:
            callback = new string("/closestnodetokey");
            break;
        case GETQUERYKEY:
            callback = new string("/getquerykey");
            break;
        default:
            cout << " UNHANDLED CHORD TRANSPORT CODE! ... ASSERTING" << endl;
            assert(this->getCode());
            break;
    }

    ss << (*callback) << "?";
    for (it = arguments.begin(); it != arguments.end(); ++it) {
        ss << (*it).first << "=" << (*it).second << "&";
    }
    ss << "checksum=" << getCheckSum(ss.str());
    //	cout << ss.str() << endl;
    return ss.str();
}

