#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"LL1/LL1.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;

int main(){
    string fromFile;
    cout << "input from file?(y/n):";
    cin >> fromFile;
    if(fromFile=="y"){
        //input file
        string path;
        std::ifstream if1, if2;
        cout << "G file :";
        cin >> path;
        if1.open(path);
        cout << "input file :";
        cin >> path;
        if2.open(path);
        if(if1.fail()||if2.fail()){
            std::cerr << "file can not open" << endl;
            exit(1);
        }

        //work
        LL1 ll1(if1);
        ll1.generate_table();
        ll1.print();
        if(ll1.parse(if2))
            cout << "result:okkkkkkkkkkkkkkkkkkkkkkkkkkkkk" << endl;
        else
            cout << "result:nooooooooooooooooooooooooooooo" << endl;

    }else{
        string quit;
        cout << "input Grammar,ctrl+z to end" << endl;
        //input grammar
        LL1 ll1(cin);
        while(!ll1.generate_table()){
            cout << "generate table fail,Please re-enter the grammar" << endl;
            ll1.init(cin);
        }

        ll1.print();

        //work
        do{
            cin.clear();
            cout << "input text(ctrl+z end):" << endl;
            if(ll1.parse(cin))
                cout << "result:okkkkkkkkkkkkkkkkkkkkkkkkkkkkk" << endl;
            else
                cout << "result:nooooooooooooooooooooooooooooo" << endl;
            cout << "quit?(y/n):";
            cin.clear();
            cin >> quit;
        } while (quit != "y");
    }
}