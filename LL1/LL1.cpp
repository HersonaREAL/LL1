#include "include/LL1.h"
using std::string;

bool LL1::generate_table(){
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
    while(getline(is,prodc))
        origin_input.push_back(prodc);
    isInit = true;
}
void LL1::print(){

}
void LL1::printCur(){
    
}