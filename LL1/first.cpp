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
    first_set["@"] = {"@"};

    //非终结符first集
    for (auto ptr = non_terminal.cbegin(); ptr != non_terminal.cend();++ptr)
        getFirst(*ptr);
    
    return true;
}
unordered_set<std::string> LL1::getFirst(string S){
    if(!first_set[S].empty())
        return first_set[S];
    const list<prodc_output> &all_right = cooked_production[S];
    for (auto r_ptr = all_right.cbegin(); r_ptr != all_right.cend();++r_ptr){
        auto s_ptr = (*r_ptr).cbegin();
        unordered_set<std::string> first_Xi;
        for (; s_ptr != (*r_ptr).cend();++s_ptr){
            first_Xi = getFirst(*s_ptr);
            first_set[S].insert(first_Xi.begin(), first_Xi.end());
            if(first_Xi.find("@")!=first_Xi.end())
                first_set[S].erase("@");
            else
                break;
        }
        if(s_ptr==(*r_ptr).cend()&&first_Xi.find("@")!=first_Xi.end())
            first_set[S].insert("@");
    }
    return first_set[S];
}
