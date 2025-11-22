#include "app/PasswordQL.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<conio.h>
#include<Windows.h>
using namespace std;

void TextColor3(int x){
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color , x );
}  

void Manager(){
    string _Username,_Pass,username,pass;
    ifstream in;
    in.open("Password/admin.txt");
    in>>username>>pass;
    
    Man:
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t+-------------------+"<<endl;
	cout<<"\t\t\t\t\t\t\t|    -LOGIN NOW-    |"<<endl;
	cout<<"\t\t\t\t\t\t\t+-------------------+"<<endl;
	cout<<"\n\n";
    cout<<"\t\t\t\t\t\tUsername : ";cin>>_Username;
    cout<<"\n";
    cout<<"\t\t\t\t\t\tPassword : ";
    passInput(_Pass);
    
    if (_Username==username&&pass==_Pass) {
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
                goto Man;
                break;
            }
            case '2':{
                exit(0);
                break;
            }
        }
    }
    in.close();
}

void resetPass(){
    string _Pass,username,pass,newpass,data;
    ifstream in;
    in.open("Password/admin.txt");
    in>>username>>pass;
    
    Reset:
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t+--------------------------+"<<endl;
	cout<<"\t\t\t\t\t\t\t|     -RESET PASSWORD-     |"<<endl;
	cout<<"\t\t\t\t\t\t\t+--------------------------+"<<endl;
    cout<<"\n\n\t\t\t\t\tPlease input current password : ";
    passInput(_Pass);
    
    if (_Pass==pass){
        newpass="";
        data="";
        cout<<"\n\n\t\t\t\t\t\t\t New password : ";
        passInput(newpass);
        cout<<"\n\n\t\t\t\t\t     Retype your new password : ";
        passInput(data);
        
        if (newpass==data){
            cout<<"\n\n\t\t\t\t\t\t\tSuccessful change\n\n";
            ofstream out;
            out.open("Password/admin.txt",ios::trunc);
            out<<username<<" "<<newpass;
            out.close();
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
    in.close();
}