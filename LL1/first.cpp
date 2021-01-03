#include"LL1.h"
using std::list;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
bool LL1::generateFirstSet(){
    //终结符的first集
    for (auto ptr = terminal.cbegin(); ptr != terminal.cend();++ptr)
        first_set[*ptr] = {*ptr};
    
    //非终结符first集
    for (auto ptr = non_terminal.cbegin(); ptr != non_terminal.cend();++ptr)
        first_set[*ptr] = getFirst(*ptr);
    
    return true;
}
unordered_set<std::string> LL1::getFirst(string S){
    if(!first_set[S].empty())
        return first_set[S];
    const list<prodc_output> &all_right = cooked_production[S];
    for (auto r_ptr = all_right.cbegin(); r_ptr != all_right.cend();++r_ptr){
        for (auto s_ptr = (*r_ptr).cbegin(); s_ptr != (*r_ptr).cend();++s_ptr){
            unordered_set<std::string> first_Xi = getFirst(*s_ptr);
            
        }
    }
    return first_set[S];
}
