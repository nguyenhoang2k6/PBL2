#include "app/passInput.h"
#include <iostream>
#include <conio.h>
using namespace std;

void passInput(string &x){
    x="";
    char c;
    do {
        c = _getch();
        if (c == 13 || c == ' ') {
            break;
        }
        if((c==8 || c==127) && !x.empty()) {
            cout<<"\b \b";
            x.erase(x.size()-1);
        }
        else { 
            x.push_back(c);
            _putch('*');
        }
    } while(c!=13 || c!=' ');
}