/**
    stringutils.cpp
    Utility funcions for http parser

    @author Bertrand Martel
    @version 1.0
*/
#include "stringutils.h"
#include "sstream"
#include "vector"
#include <cctype>

using namespace std;

/**
 * @brief split
 *      split a string with a character delimiter
 * @param s
 *      string to split
 * @param delim
 *      character delimiter
 * @return
 *      vector of splitted string
 */
vector<string> stringutils::split(string s, char delim) {

    vector<string> elems;

    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

/**
 * @brief isNum
 *      check if char * is numeric data
 * @param s
 *      char * input
 * @return
 *      true if data is numeric
 */
bool stringutils::isNum(char *s) {

    int i = 0,  flag;

    while(s[i]){
            //if there is a letter in a string then string is not a number
        if(isalpha(s[i])){
            flag = 0;
            break;
        }
        else flag = 1;
        i++;
        }
    if (flag == 1) return true;
    else return false;
}
