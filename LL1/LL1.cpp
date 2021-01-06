#include "LL1.h"
#include<iostream>
#include<sstream>
#include<iomanip>
using std::cout;
using std::endl;
using std::list;
using std::string;
using std::stringstream;
using std::vector;

bool LL1::generate_table(){
    if(!isInit)
        return false;
    cookProduction();
    generateFirstSet();
    generateFollowSet();

    //生成预测分析表
    for (auto ptr = cooked_production.cbegin(); ptr != cooked_production.cend();++ptr){
        const string &A = (*ptr).first;
        const list<prodc_output> &all_right = (*ptr).second;
        for (auto r_ptr = all_right.cbegin(); r_ptr != all_right.cend();++r_ptr){
            //A->alpha，*r_ptr = alpha
            const auto &alpha = *r_ptr;
            for (auto t_it = terminal.cbegin(); t_it != terminal.cend();++t_it){
                const string &a = (*t_it);

                //跳过空串
                if(a=="@") continue;

                //终结符a∈first(A)
                if(first_set[A].find(a)!=first_set[A].end()){
                    //a∈first(alpha)
                    if(right_first_set[A][alpha].find(a)!=right_first_set[A][alpha].end())
                        analysis_table[A][a] = alpha;
                }
            }
            
        }
        if(first_set[A].find("@")!=first_set[A].end()){
            for (auto it = follow_set[A].cbegin(); it != follow_set[A].cend();++it)
                analysis_table[A][*it] = {"@"};
        }
    }
    return true;
}
bool LL1::parse(std::istream &is){
    list<string> content;
    string word;
    while(is>>word)
        content.push_back(word);
    content.push_back("#");
    list<string> stk;
    stk.push_back("#");
    stk.push_back(start_symbol);
    for (auto it = content.cbegin(); it !=content.cend();){
        printCur(stk,content,it);
        const string stk_top = stk.back();
        //语法正确
        if(stk_top=="#"){
            if (stk_top != *it)
                return false;
            stk.pop_back();
            cout << "action: accept" << endl
                 << endl;
            return true;
        }
        //终结符
        if(terminal.find(stk_top)!=terminal.end()){
            if(stk_top!=*it)
                return false;
            cout << "action: match " << *it << endl
                 << endl;
            stk.pop_back();
            ++it;
        }else{
            //非终结符
            if(analysis_table[stk_top][*it].empty())
                return false;
            //遇到空串
            if(analysis_table[stk_top][*it][0]=="@"){
                cout << "action: " << stk_top << "-> @" << endl
                     << endl;
                stk.pop_back();
                continue;
            }
            stk.pop_back();
            for (auto s_it = analysis_table[stk_top][*it].crbegin(); s_it != analysis_table[stk_top][*it].crend();++s_it)
                stk.push_back(*s_it);
            cout << "action: " << stk_top << " -> ";
            for (auto s_it = analysis_table[stk_top][*it].cbegin(); s_it != analysis_table[stk_top][*it].cend();++s_it)
                cout << *s_it << " ";
            cout << endl
                 << endl;
        }

    }

    return false;
}

void LL1::init(std::istream &is){
    //清空缓存
    origin_input.clear();
    raw_production.clear();
    tmp_production.clear();
    cooked_production.clear();
    terminal.clear();
    non_terminal.clear();
    first_set.clear();
    follow_set.clear();
    analysis_table.clear();
    start_symbol.clear();
    right_first_set.clear();
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
    cout << "startSymbol:" << start_symbol << endl;
    cout << "\nrawProduction:" << endl;

    //输出未处理产生式
    for (auto beg = raw_production.begin(); beg != raw_production.end();++beg){
        cout << (*beg).first << " -> ";
        for (auto outBeg = (*beg).second.begin(); outBeg != (*beg).second.end();++outBeg){
            for (auto strBeg = (*outBeg).begin(); strBeg != (*outBeg).end();++strBeg)
                cout << *strBeg << " ";
            if(outBeg==(--(*beg).second.end()))
                cout << endl;
            else
                cout << "| ";
        }
    }

    //输出处理好的产生式
    cout << "\ncookedProduction:" << endl;
    for (auto beg = cooked_production.begin(); beg != cooked_production.end();++beg){
        cout << (*beg).first << " -> ";
        for (auto outBeg = (*beg).second.begin(); outBeg != (*beg).second.end();++outBeg){
            for (auto strBeg = (*outBeg).begin(); strBeg != (*outBeg).end();++strBeg)
                cout << *strBeg << " ";
            if(outBeg==(--((*beg).second.end())))
                cout << endl;
            else
                cout << "| ";
        }
    }

    //输出终结符与非终结符
    cout << "\nnonTerminal = {";
    for (auto ptr = non_terminal.cbegin(); ptr != non_terminal.cend();++ptr)
        cout << *ptr << ", ";
    cout << "}" << endl;
    cout << "Terminal = {";
    for (auto ptr = terminal.cbegin(); ptr != terminal.cend();++ptr)
        cout << *ptr << ", ";
    cout << "}" << endl;

    //输出first集
    cout << "\nfirst:" << endl;
    for (auto ptr = first_set.cbegin(); ptr != first_set.cend();++ptr){
        if(non_terminal.find((*ptr).first)!=non_terminal.end()){
            cout << "first(" << (*ptr).first << ") = {";
            for (auto s_ptr = (*ptr).second.cbegin(); s_ptr != (*ptr).second.cend();++s_ptr)
                cout << *s_ptr << ", ";
            cout << "}" << endl;
        }
    }

    //输出候选式first
    cout << "\nright first:" << endl;
    for (auto ptr = right_first_set.begin(); ptr != right_first_set.end();++ptr){
        cout << (*ptr).first << "->" << endl;
        auto first_map = (*ptr).second;
        for (auto k_it = first_map.begin(); k_it != first_map.end();++k_it){
            cout << "first(";
            for (auto s_it = (*k_it).first.begin(); s_it != (*k_it).first.end();++s_it)
                cout << *s_it << " ";
            cout << ")={";
            for(auto s_it = (*k_it).second.begin(); s_it != (*k_it).second.end();++s_it)
                cout << *s_it << ", ";
            cout << "}\n";
        }
        cout << endl;
    }

    //输出follow集
    cout << "\nfollow:" << endl;
    for (auto ptr = follow_set.cbegin(); ptr != follow_set.cend();++ptr){
        if(non_terminal.find((*ptr).first)!=non_terminal.end()){
            cout << "follow(" << (*ptr).first << ") = {";
            for (auto s_ptr = (*ptr).second.cbegin(); s_ptr != (*ptr).second.cend();++s_ptr)
                cout << *s_ptr << ", ";
            cout << "}" << endl;
        }
    }

    //输出预测分析表
    cout << "\ntable:" << endl;
    cout<<std::left <<std::setw(15)<< " " ;
    for (auto it = terminal.cbegin(); it != terminal.cend();++it)
        if((*it)!="@")
            cout<<std::left <<std::setw(15)<< (*it) ;
    for (auto t_it = analysis_table.cbegin(); t_it != analysis_table.cend(); ++t_it){
        cout << endl<<endl<<std::left<<std::setw(15)<< (*t_it).first;
        for (auto it = terminal.cbegin(); it != terminal.cend();++it){
            if((*it)!="@"){
                //cout<< (*it)<<" -> " ;
                const prodc_output &one_right = analysis_table[(*t_it).first][*it];
                string s_out;
                if(one_right.empty())
                    s_out += "null";
                for (auto s_it = one_right.cbegin(); s_it != one_right.cend();++s_it)
                    s_out += (*s_it + " ");
                cout <<std::left<< std::setw(15)<<s_out;
            }
        }
    }
    cout << endl<<endl;
}
void LL1::printCur(const list<std::string> &stk,const list<string> &content,list<string>::const_iterator it){
    cout << "stack: ";
    for (auto s_it = stk.cbegin(); s_it != stk.cend();++s_it)
        cout << *s_it << ", ";
    cout << "\nremain: ";
    while(it!=content.cend())
        cout << *it++ << " ";
    cout << endl;

}

void LL1::string2production(){
    for (auto i = 0; i != origin_input.size();++i){
        //将原始输入读到字符串流
        stringstream strS(origin_input[i]);
        string left_symbol;
        string right_symbol;
        strS >> left_symbol;
        prodc_output out;
        if(i==0)
            start_symbol = left_symbol;
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