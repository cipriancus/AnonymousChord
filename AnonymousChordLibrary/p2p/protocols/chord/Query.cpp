/* 
 * File:   Query.cpp
 * Author: ciprian
 * 
 * Created on March 22, 2017, 1:32 AM
 */

#include "Query.h"
#include "ProtocolSingleton.h"
#include <algorithm>

Query::Query(){
    
}

Query::Query(int min,int max) {
    l=random(min,max);//l-hop random walk
    
    SHA1 *sha1 = new SHA1(); 
    int number=random();
    char no[100];
    //current hash for this query
    sha1->addBytes( reinterpret_cast<const char*>(itoa(number,no,10)), strlen(itoa(number,no,10)));  
    this->queryHash=string(reinterpret_cast<const char*>(sha1->getDigest()));
    delete sha1;  
}

Query::~Query() {
    
}

bool Query::addFingerTable(vector<Node*> fingerTable, Node* node) {
    if (find(selectedNodes.begin(), selectedNodes.end(), node) != selectedNodes.end() ){
            return false;
    }
    selectedFingerTables.push_back(fingerTable);
    selectedNodes.push_back(node);  
}

bool Query::hasBeenSelected(Node* node) {
    if ( find(selectedNodes.begin(), selectedNodes.end(), node) != selectedNodes.end() ){
            return true;
    }
    return false;
}

Node* Query::selectNewNode(vector<Node*> fingerTable) {
    Node *selectedNode;      
    string serializedTable;

        do{
            int randomNode=random(0,fingerTable.size()-1);//select a random node from current finger table
        
            selectedNode=fingerTable[randomNode];//select the node
            
            //check if it wasn't selected before
            if(hasBeenSelected(selectedNode)==false){
                map<string,string> queryParams;      
                serializedTable = P_SINGLETON->getChordNode()->send_request_with_timeout(selectedNode,GETFINGERTABLE,4,queryParams);
            }
        }while(serializedTable.size()==0);
    return selectedNode;
}



int Query::random(int min, int max){
    random_device rd;   
    mt19937 gen(rd()); 
    uniform_int_distribution<> dist(min,max);
    return dist(gen);
}

int Query::random(){
    random_device rd;   
    return rd();
}


/* A utility function to reverse a string  */
void Query::reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* Query::itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}
 