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


// Hàm này chỉ có trách nhiệm xác thực, KHÔNG hiển thị giao diện hay xử lý lỗi
bool Staff(string _Username, string _Pass){
    ifstream in;
    in.open("Password/staff.txt");
    
    // Bổ sung: Kiểm tra nếu file không mở được
    if (!in.is_open()) {
        // In ra lỗi hoặc xử lý phù hợp
        return false; 
    }
    
    string username, pass;
    while(in >> username >> pass){
        if (_Username == username && _Pass == pass) {
            in.close();
            return true; // Xác thực thành công
        }
    }
    
    in.close();
    return false; // Không tìm thấy tài khoản
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