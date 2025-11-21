#include "PasswordNV.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<conio.h>
#include<Windows.h>
using namespace std;

void TextColor7(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}


std::string Staff(){
    string _Username,_Pass,username,pass;
    ifstream in;
    bool found=false;
    in.open("Password/staff.txt");
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t+-------------------+"<<endl;
	cout<<"\t\t\t\t\t\t\t|    -LOGIN NOW-    |"<<endl;
	cout<<"\t\t\t\t\t\t\t+-------------------+"<<endl;
	cout<<"\n\n";
    cout<<"\t\t\t\t\t\tUsername : ";cin>>_Username;
    cout<<"\n";
    cout<<"\t\t\t\t\t\tPassword : ";
    passInput(_Pass);
    while(in>>username>>pass){
        if (_Username==username&&pass==_Pass) {
            found =true;
            break;}
    }
    if (found)
    {
        cout<<"\n\n\t\t\t\t\t\t\tLogged in successfully!\n";
    }
    else {
        cout<<"\n\n\t\t\t\t\t\t  Username or Password is incorrect";
        cout<<"\n\n\t\t\t\t\t\t\t    -Notification-";
        cout<<"\n\t\t\t\t\t\t\t+--------------------+";
	    cout<<"\n\t\t\t\t\t\t\t|   1.Login again    |";
	    cout<<"\n\t\t\t\t\t\t\t|   2.Exit           |";
	    cout<<"\n\t\t\t\t\t\t\t+--------------------+";
	    cout<<"\n\n\t\t\t\t\t\tYour choice : ";
    char d;cin>>d;
    switch(d){
        case '1':{
            system("cls");
            Staff();
            break;
        }
        case '2':{
            exit(0);
            break;
        }
    }
    }
    in.close();
    return _Username;
}

void resetPassNV(const string &username){
    string _Pass,_Username,pass,newpass,data;
    ifstream on;
    bool userFound = false;
    
    on.open("Password/staff.txt");
    while(on>>_Username>>pass){
        if (username==_Username) {
            userFound = true;
            break;
        }
    }
    on.close();
    
    if (!userFound) {
        cout<<"\n\n\t\t\t\t\t\tUser not found!";
        return;
    }
    
    Reset:
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t+--------------------------+"<<endl;
	cout<<"\t\t\t\t\t\t\t|     -RESET PASSWORD-     |"<<endl;
	cout<<"\t\t\t\t\t\t\t+--------------------------+"<<endl;
    cout<<"\n\n\t\t\t\t\tPlease input current password : ";
    passInput(_Pass);
    
    if (_Pass==pass){
        data="";
        newpass="";
        cout<<"\n\n\t\t\t\t\t\t\t New password : ";
        passInput(newpass);
        cout<<"\n\n\t\t\t\t\t     Retype your new password : ";
        passInput(data);
        
        if (newpass==data){
            cout<<"\n\n\t\t\t\t\t\t\tSuccessful change\n\n";
            ofstream out;
            out.open("Password/temp.txt",ios::out);
            ifstream in;
            in.open("Password/staff.txt");
            while(in>>_Username>>pass){
                if (username==_Username){
                    out<<_Username<<" "<<newpass<<endl;
                }
                else out<<_Username<<" "<<pass<<endl;
            }
            in.close();
            out.close();
            remove("Password/staff.txt");
            rename("Password/temp.txt","Password/staff.txt");
        }
        else{
            cout<<"\n\n\t\t\t\t\t\tNew password is incorrect,try again?";
            cout<<"\n\n\t\t\t\t\t\t\t       -Notification-";
            cout<<"\n\t\t\t\t\t\t\t   +--------------------+";
	        cout<<"\n\t\t\t\t\t\t\t   |   1.Yes            |";
	        cout<<"\n\t\t\t\t\t\t\t   |   2.No             |";
	        cout<<"\n\t\t\t\t\t\t\t   +--------------------+";
	        cout<<"\n\n\t\t\t\t\t\tYour choice ";
            char d; cin>>d;
            switch(d){
                case '1':{
                    system("cls");
                    goto Reset;
                    break;
                }
                case '2':{
                    system("cls");
                    break;
                }
            }            
        }
    }
    else{
        cout<<"\n\n\t\t\t\t\t\t  Incorrect Password"; 
        cout<<"\n\n\t\t\t\t\t\t\t      -Notification-";
        cout<<"\n\t\t\t\t\t\t\t   +---------------------+";
	    cout<<"\n\t\t\t\t\t\t\t   |     1.Try again     |";
	    cout<<"\n\t\t\t\t\t\t\t   |     2.Return        |";
	    cout<<"\n\t\t\t\t\t\t\t   +---------------------+";
	    cout<<"\n\n\t\t\t\t\t\tYour choice : ";
        char t; cin>>t;
        switch(t){
            case '1':{
                system("cls");
                goto Reset;
                break;
            }
            case '2':{
                return;
                break;
            }
        }
    }
}