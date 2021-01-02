#include "LL1.h"
#include<iostream>
#include<sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

bool LL1::generate_table(){
    removeDirect();
    return true;
}
bool LL1::parse(std::istream &is){
    return true;
}

void LL1::init(std::istream &is){
    origin_input.clear();
    raw_production.clear();
    cooked_production.clear();
    terminal.clear();
    non_terminal.clear();
    first_set.clear();
    follow_set.clear();
    analysis_table.clear();
    string prodc;
    //读取产生式
    while(getline(is,prodc))
        if(!prodc.empty())
            origin_input.push_back(prodc);
    
    string2production();
    isInit = true;
}
LL1::LL1(std::istream &is){
    init(is);
}
void LL1::print(){
    cout << "\nrawProduction:" << endl;
    //输出未处理产生式
    for (auto beg = raw_production.begin(); beg != raw_production.end();++beg){
        cout << (*beg).first << " -> ";
        for (auto outBeg = (*beg).second.begin(); outBeg != (*beg).second.end();++outBeg){
            for (auto strBeg = (*outBeg).begin(); strBeg != (*outBeg).end();++strBeg)
                cout << *strBeg << " ";
            if(outBeg==(*beg).second.end()-1)
                cout << endl;
            else
                cout << "| ";
        }
    }
    cout << "\ncookedProduction:" << endl;
    //输出未处理产生式
    for (auto beg = cooked_production.begin(); beg != cooked_production.end();++beg){
        cout << (*beg).first << " -> ";
        for (auto outBeg = (*beg).second.begin(); outBeg != (*beg).second.end();++outBeg){
            for (auto strBeg = (*outBeg).begin(); strBeg != (*outBeg).end();++strBeg)
                cout << *strBeg << " ";
            if(outBeg==(*beg).second.end()-1)
                cout << endl;
            else
                cout << "| ";
        }
    }

}
void LL1::printCur(){
    
}

void LL1::string2production(){
    for (auto i = 0; i != origin_input.size();++i){
        //将原始输入读到字符串流
        stringstream strS(origin_input[i]);
        string left_symbol;
        string right_symbol;
        strS >> left_symbol;
        prodc_output out;
        while(strS>>right_symbol){
            if(right_symbol=="->")
                continue;
            if(right_symbol=="|"){
                //读到|清空out，开始下一个读入
                if(!out.empty())
                    raw_production[left_symbol].push_back(out);
                out.clear();
                continue;
            }
            out.push_back(right_symbol);
        }
        //存入产生式数据中
        raw_production[left_symbol].push_back(out);
    }
}