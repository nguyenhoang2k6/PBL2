#include "Hoadon.h"
#include "Item.h"
#include "Nhanvien.h"
#include "Thongke.h"
#include<windows.h>
#include<iostream>
#include<stdlib.h>
#include<iomanip>
using namespace std;

void TextColor5(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau,x);
}

void read(const string &x){
    ifstream in;
    in.open(x);
    string line;
    while (getline(in,line)){
        cout<<line<<endl;
    }
    in.close();
}

Hoadon::Hoadon(){
    Menu=new Item[MAX];
    soluong=new int[MAX];
    total = 0;
    price = 0;
}

Hoadon::~Hoadon(){
    delete [] Menu;
    delete [] soluong;
}

bool check_exist(const string &x){
    ifstream in;
    in.open("Hoadon/"+x+".txt");
    if(in){
        in.close();
        return true;
    }
    return false;
}

ostream& operator<<(ostream& out,const Hoadon &p){
    out<<"Ngay thu ngan: "<<p.day<<"/"<<p.month<<"/"<<p.year<<endl;
    out<<"Nhan vien thu ngan: "<<p.maNV<<endl;
    out<<setw(30)<<left<<"Ten mon";
    out<<setw(10)<<left<<"So luong";
    out<<setw(10)<<left<<"Don gia";
    out<<setw(10)<<left<<"Thanh tien"<<endl;
    out<<"---------------------------------------------------------------"<<endl;
    for (int i=0;i<p.total;i++){
        out<<setw(30)<<left<<p.Menu[i].getTenItem();
        out<<setw(10)<<left<<p.soluong[i];
        out<<setw(10)<<left<<p.Menu[i].getprice();
        out<<setw(10)<<left<<p.soluong[i]*p.Menu[i].getprice()<<endl;
    }
    out<<"---------------------------------------------------------------"<<endl;
    out<<"Tong gia tien: "<<p.price<<endl;
    return out;
}

void Cashier(Hoadon &p,const string &maNV){
    // Reset hóa đơn
    p.total = 0;
    p.price = 0;
    
    Nhapngay:
    cout<<"Nhap ngay thu ngan: ";cin>>p.day>>p.month>>p.year;
    if(KiemTraNgay(p.day,p.month,p.year)==0){
        TextColor5(12);cout<<"\nNgay,thang,nam khong hop le\n";TextColor5(7);
        cout<<"\nBan co muon nhap lai ngay thu ngan (y/n) : ";
        char t;cin>>t;
        if(t=='y'){
            system("cls");
            goto Nhapngay;
        }
        return;
    }
    
    p.maNV = maNV;
    cout<<"Nhap ma hoa don: ";cin>>p.maHD;
    
    while (check_exist(p.maHD)){
        cout<<"Ma da duoc su dung, vui long nhap lai: ";
        cin>>p.maHD;
    }
    
    bool continueAdding = true;
    while(continueAdding){
        Cont:
        system("cls");
        Item *x=new Item[MAX];
        display(x);
        int n=getInfo(x);
        
        cout<<"Nhap ma mat hang: ";
        string line;
        cin>>line;
        
        if(check_maItem(x,line)==0){
            TextColor5(12); cout<<"Ma Item nay khong ton tai";TextColor5(7); 
            cout<<"\n\nBan co muon nhap lai ma Item ? (y/n) : ";
            char t;cin>>t;
            if(t=='y'){
                delete [] x;
                goto Cont;
            } else {
                delete [] x;
                continueAdding = false;
                break;
            }
        }
        
        // Tìm item và thêm vào hóa đơn
        bool itemFound = false;
        for (int i=0;i<n-1;i++){
            if(line == x[i].getmaItem()){
                // Kiểm tra xem item đã có trong hóa đơn chưa
                bool itemExists = false;
                for(int j=0;j<p.total;j++){
                    if(p.Menu[j].getmaItem() == line){
                        // Item đã tồn tại, cập nhật số lượng
                        cout<<"Nhap so luong: ";
                        int newQuantity;
                        cin>>newQuantity;
                        p.soluong[j] += newQuantity;
                        itemExists = true;
                        break;
                    }
                }
                
                if(!itemExists){
                    // Item mới, thêm vào hóa đơn
                    p.Menu[p.total] = x[i];
                    cout<<"Nhap so luong: ";
                    cin>>p.soluong[p.total];
                    p.total++;
                }
                itemFound = true;
                break;
            }
        }
        
        delete [] x;
        
        if(!itemFound){
            TextColor5(12); cout<<"Khong tim thay mat hang!";TextColor5(7);
        }
        
        // Tính lại tổng tiền
        p.price = 0;
        for(int i=0;i<p.total;i++){
            p.price += p.soluong[i] * p.Menu[i].getprice();
        }
        
        // Hiển thị hóa đơn tạm thời
        system("cls");
        cout<<"=== HOA DON TAM THOI ==="<<endl;
        cout<<setw(30)<<left<<"Ten mon";
        cout<<setw(10)<<left<<"So luong";
        cout<<setw(10)<<left<<"Don gia";
        cout<<setw(10)<<left<<"Thanh tien"<<endl;
        cout<<"---------------------------------------------------"<<endl;
        for(int i=0;i<p.total;i++){
            cout<<setw(30)<<left<<p.Menu[i].getTenItem();
            cout<<setw(10)<<left<<p.soluong[i];
            cout<<setw(10)<<left<<p.Menu[i].getprice();
            cout<<setw(10)<<left<<p.soluong[i]*p.Menu[i].getprice()<<endl;
        }
        cout<<"---------------------------------------------------"<<endl;
        cout<<"Tong tam tinh: "<<p.price<<endl;
        
        cout<<"\nThem mon khac?\n1. Co\n2. Khong - Thanh toan\nLua chon: ";
        int choice;
        cin>>choice;
        if(choice == 2){
            continueAdding = false;
        }
    }
    
    // Lưu hóa đơn và log
    ofstream file;
    file.open("Hoadon/"+p.maHD+".txt");
    file<<p;
    file.close();
    
    ofstream log;
    log.open("History/log.txt",ios::app);
    log<<p.day<<" "<<p.month<<" "<<p.year<<" "<<p.maNV<<" "<<p.maHD<<" "<<p.price<<endl;
    log.close();
    
    // Hiển thị hóa đơn cuối cùng
    system("cls");
    cout<<"=== HOA DON CHINH THUC ==="<<endl;
    read("Hoadon/"+p.maHD+".txt");
    cout<<"\nHoa don da duoc luu thanh cong!"<<endl;
    system("pause");
}

void display(Hoadon &p){
    hoadon:
    ifstream in;
    in.open("history/log.txt");
    int day,month,year,price;
    string maNV,maHD;
    
    cout<<endl;
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|  Ma Hoa Don"<<"\t|\t"<<"Thoi gian thuc hien giao dich      |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl;
    
    while(in>>day>>month>>year>>maNV>>maHD>>price){
        if(day<10 && month<10){
            cout<<"|  "<<maHD<<"\t|\t"<<day<<"/"<<month<<"/"<<setw(21)<<left<<year<<setw(11)<<right<<"|"<<endl;
        }
        else if(day<10){
            cout<<"|  "<<maHD<<"\t|\t"<<day<<"/"<<month<<"/"<<setw(21)<<left<<year<<setw(10)<<right<<"|"<<endl;
        }
        else if(month<10){
            cout<<"|  "<<maHD<<"\t|\t"<<day<<"/"<<month<<"/"<<setw(21)<<left<<year<<setw(10)<<right<<"|"<<endl;
        }
        else {
            cout<<"|  "<<maHD<<"\t|\t"<<day<<"/"<<month<<"/"<<setw(20)<<left<<year<<setw(10)<<right<<"|"<<endl;
        }
    }
    cout<<"+----------------------------------------------------------+"<<endl;
    in.close();
    
    cout<<"\nChon hoa don can xem: ";
    string maHD1;
    cin>>maHD1;
    
    if(!check_exist(maHD1)){
        TextColor5(12); cout<<"\nHoa don nay khong ton tai"; TextColor5(7);
        cout<<"\nBan co muon tiep tuc xem hoa don? (y/n) : ";
        char t;cin>>t;
        if(t=='y'){
            system("cls");
            goto hoadon;
        }
    }
    else{
        system("cls");
        read("Hoadon/"+maHD1+".txt");
        cout<<"\nBan co muon tiep tuc xem hoa don? (y/n) : ";
        char t;cin>>t;
        if(t=='y'){
            system("cls");
            goto hoadon;
        }
    }
}