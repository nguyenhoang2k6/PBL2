#include "Nhanvien.h"
#include "Item.h"
#include "Hoadon.h"
#include "PasswordQL.h"
#include "PasswordNV.h"
#include "Thongke.h"
#include "passInput.h"
#include<iostream>
#include<Windows.h>
#include<iomanip>
#include<conio.h>
#define N 100
using namespace std;

// KHAI BÁO PROTOTYPE (ĐÃ XÓA TextColor và InLine)
void menuchinh();
void menuStaff();
void menuManager();
void menuQuanlynhanvien();
void menuQuanlyItem();
void menuThongke();
void menu();

void menuchinh(){
    cout<<"\n\n\t\t\t\t\t\t\t\t-MENU-";
    cout<<"\n\t\t\t\t\t\t========================================";
    cout<<"\n\t\t\t\t\t\t||"<<"\t                              ||";
    cout<<"\n\t\t\t\t\t\t||"<<"\t       1.Staff                ||";
    cout<<"\n\t\t\t\t\t\t||"<<"\t                              ||";
    cout<<"\n\t\t\t\t\t\t||"<<"\t       2.Manager              ||";
	cout<<"\n\t\t\t\t\t\t||"<<"\t                              ||";
	cout<<"\n\t\t\t\t\t\t||"<<"\t       3.Exit                 ||";
	cout<<"\n\t\t\t\t\t\t||"<<"\t                              ||";
	cout<<"\n\t\t\t\t\t\t========================================";
}

void menuStaff(){
    system("cls");
    cout<<"\n\n\t\t\t\t\t\t     -STAFF-";
    cout<<"\n\t\t\t+---------------------------------------------------------------+";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                1.Hien Thi Danh Sach Nhan Vien       \t\t|";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                2.Hien Thi Danh Sach Item            \t\t|";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                3.Them Hoa Don                       \t\t|";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                4.Doi Pass                           \t\t|";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                5.Quay Lai                           \t\t|";       
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t|"<<"                6.Exit                               \t\t|";
    cout<<"\n\t\t\t|"<<"                                                     \t\t|";
    cout<<"\n\t\t\t+---------------------------------------------------------------+";
}

void menuManager(){
    system("cls");
    cout<<"\n\n\t\t\t\t\t\t\t -MANAGER-";
    cout<<"\n\t\t\t\t+------------------------------------------------------------+";
    cout<<"\n\t\t\t\t|"<<"\t                                                     |";
    cout<<"\n\t\t\t\t|"<<"\t              1.Quan Ly Nhan Vien                    |";
    cout<<"\n\t\t\t\t|"<<"\t                                                     |";
    cout<<"\n\t\t\t\t|"<<"\t              2.Quan Ly Thuc Don                     |";
	cout<<"\n\t\t\t\t|"<<"\t                                                     |";
    cout<<"\n\t\t\t\t|"<<"\t              3.Quan Ly Hoa Don                      |";
    cout<<"\n\t\t\t\t|"<<"\t                                                     |";
	cout<<"\n\t\t\t\t|"<<"\t              4.Thong Ke                             |";
	cout<<"\n\t\t\t\t|"<<"\t                                                     |";
	cout<<"\n\t\t\t\t|"<<"\t              5.Doi Pass                             |";
	cout<<"\n\t\t\t\t|"<<"\t                                                     |";
    cout<<"\n\t\t\t\t|"<<"\t              6.Quay Lai                             |";       
    cout<<"\n\t\t\t\t|"<<"\t                                                     |";
	cout<<"\n\t\t\t\t|"<<"\t              7.Exit                                 |";
	cout<<"\n\t\t\t\t|"<<"\t                                                     |";
	cout<<"\n\t\t\t\t+------------------------------------------------------------+";
}

void menuQuanlynhanvien(){
    system("cls");
    cout<<"\n\n\t\t\t\t\t\t\t   -QUAN LY NHAN VIEN-";
    cout<<"\n\t\t\t\t\t*********************************************************";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
    cout<<"\n\t\t\t\t\t*"<<"\t         1.Hien Thi Danh Sanh Nhan Vien         *";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
    cout<<"\n\t\t\t\t\t*"<<"\t         2.Them Nhan Vien                       *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t         3.Xoa Nhan Vien                        *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t         4.Quay Lai                             *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t         5.Exit                                 *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*********************************************************";
}

void menuQuanlyItem(){
    system("cls");
    cout<<"\n\n\t\t\t\t\t\t\t   -QUAN LY THUC DON-";
    cout<<"\n\t\t\t\t\t*********************************************************";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
    cout<<"\n\t\t\t\t\t*"<<"\t             1.Hien Thi Thuc Don                *";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
    cout<<"\n\t\t\t\t\t*"<<"\t             2.Them Item                        *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t             3.Xoa Item                         *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t             4.Quay Lai                         *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*"<<"\t             5.Exit                             *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                *";
	cout<<"\n\t\t\t\t\t*********************************************************";
}

void menuThongke(){
    system("cls");
    cout<<"\n\n\t\t\t\t\t\t\t\t    -THONG KE-";
    cout<<"\n\t\t\t\t\t*******************************************************************";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
    cout<<"\n\t\t\t\t\t*"<<"\t               1.Thong Ke Theo Ngay                       *";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
    cout<<"\n\t\t\t\t\t*"<<"\t               2.Thong Ke Theo Thang                      *";
    cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
	cout<<"\n\t\t\t\t\t*"<<"\t               3.Thong Ke Theo Nam                        *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
	cout<<"\n\t\t\t\t\t*"<<"\t               4.Thong Ke Theo Theo Nhan Vien             *";              
	cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
	cout<<"\n\t\t\t\t\t*"<<"\t               5.Quay Lai                                 *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
	cout<<"\n\t\t\t\t\t*"<<"\t               6.Exit                                     *";
	cout<<"\n\t\t\t\t\t*"<<"\t                                                          *";
	cout<<"\n\t\t\t\t\t*******************************************************************";
}

void menu(){
    Menu:
    menuchinh();
    int a;
    cout<<"\n\n\t\t\t\t\t\tNhap lua chon :";cin>>a;
    while(a<0||a>3){
        cout<<"\n\n\t\t\t\t\t\tNhap lua chon :";cin>>a;   
    }
    switch (a)
    {
        case 1:
        {   
            system("cls");
            string maNV=Staff();
            Staff:
            menuStaff();
            int k;
            cout<<"\n\n\t\t\tNhap lua chon :";cin>>k;
            while(k<0||k>6){
            cout<<"\n\n\t\t\tNhap lua chon :";cin>>k;
            }
            switch (k){
                case 1:
                {
                    Nhanvien *nv=new Nhanvien[N];
                    system("cls");
                    display(nv);
                    cout<<"\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    delete [] nv;
                    goto Staff;
                    break;
                }
                case 2:
                {
                    Item *x=new Item[N];
                    system("cls");
                    display(x);
                    cout<<"\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    delete [] x;
                    goto Staff;
                    break;
                }
                case 3:
                {
                    system("cls");
                    Hoadon p;
                    Cashier(p,maNV);
                    cout<<"\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    goto Staff;
                    break;
                }
                case 4:
                {
                    system("cls");
                    resetPassNV(maNV);
                    goto Staff;
                    break;
                }
                case 5:
                    goto Menu;
                    break;
                case 6:
                    exit(0);
                    break;
            }  
        }
        case 2:
        {   
            system("cls");
            Manager();
            Manager:
            menuManager();
            int b;
            cout<<"\n\n\t\t\t\tNhap lua chon :";cin>>b;
            while(b<0||b>7){
            cout<<"\n\n\t\t\t\tNhap lua chon :";cin>>b;
            }
            switch (b)
            {
               case 1:
                {
                    Quanlynhanvien:
                    Nhanvien *NV=new Nhanvien[N];
                    menuQuanlynhanvien();
                    int c;
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>c;getchar();
                    while(c<0||c>5){
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>c;getchar();
                    }
                    switch (c)
                    {
                        case 1:
                        {
                            system("cls");
                            display(NV);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Quanlynhanvien;
                            break;
                        }
                        case 2:
                        {
                            system("cls");
                            Add(NV);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Quanlynhanvien;
                            break;
                        }
                        case 3:
                        {
                            system("cls");
                            Delete(NV);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Quanlynhanvien;
                            break;
                        }
                        case 4:
                            goto Manager;
                            break;
                        case 5:
                            exit(0);
                            break;
                    }
                    delete [] NV;
                }
                break;
                
                case 2:
                {
                    QuanlyItem:
                    Item *x=new Item[N];
                    menuQuanlyItem();
                    int d;
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>d;getchar();
                    while(d<0||d>5){
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>d;getchar();
                    }
                    switch (d)
                    {
                        case 1:
                        {
                            system("cls");
                            display(x);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto QuanlyItem;
                            break;
                        }
                        case 2:
                        {
                            system("cls");
                            Add(x);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto QuanlyItem;
                            break;
                        }
                        case 3:
                        {
                            system("cls");
                            Delete(x);
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto QuanlyItem;
                            break;
                        }
                        case 4:
                            goto Manager;
                            break;
                        case 5:
                            exit(0);
                            break;
                    }
                    delete [] x;
                }
                break;

                case 3:
                { 
                    system("cls");
                    cout<<endl;
                    cout<<"=====================";
                    cout<<"Danh Sach Hoa Don";
                    cout<<"====================="<<endl;
                    Hoadon a;
                    display(a);
                    cout<<"\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    goto Manager;
                    break;
                }

                case 4:
                {
                    Thongke:
                    menuThongke();
                    int h;
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>h;getchar();
                    while(h<0||h>6){
                    cout<<"\n\n\t\t\t\t\tNhap lua chon :";cin>>h;getchar();
                    }
                    switch (h){
                        case 1:
                        {
                            system("cls");
                            statsDay();
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Thongke;
                            break;
                        }
                        case 2:
                        {
                            system("cls");
                            statsMonth();
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Thongke;
                            break;
                        }
                        case 3:
                        {
                            system("cls");
                            statsYear();
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Thongke;
                            break;
                        }
                        case 4:
                        {
                            system("cls");
                            statsNhanvien();
                            cout<<"\nNhan phim bat ky de tiep tuc...";
                            _getch();
                            goto Thongke;
                            break;
                        }
                        case 5:
                            goto Manager;
                            break;
                        case 6:
                            exit(0);
                            break;
                    }
                }
                break;

                case 5:
                {
                    system("cls");
                    resetPass();
                    goto Manager;
                    break;
                }

                case 6:
                   goto Menu;
                   break;

                case 7:
                   exit(0);
                   break;
            }
        }

        case 3:
            exit(0);
            break;
    } 
}

int main(){
    menu();
    return 0;
}