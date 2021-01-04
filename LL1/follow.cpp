#include"LL1.h"
using std::list;
using std::string;
using std::unordered_set;
bool LL1::generateFollowSet(){
    for (auto ptr = non_terminal.cbegin(); ptr != non_terminal.cend();++ptr)
        getFollow(*ptr);
    return true;
}
unordered_set<std::string> LL1::getFollow(std::string S){
    if(!follow_set[S].empty())
        return follow_set[S];
    
    unordered_set<std::string> tmp_set;
    if(S==start_symbol)
        tmp_set.insert("#");
    for (auto it = cooked_production.cbegin(); it != cooked_production.cend(); ++it){
        const list<prodc_output> &all_right = (*it).second;
        const string &left = (*it).first;
        for (auto r_ptr = all_right.cbegin(); r_ptr != all_right.cend();++r_ptr){
            const prodc_output &oneR = (*r_ptr);
            for (auto i = 0; i < oneR.size();++i){
                if(oneR[i]==S){
                    //B->aS
                    if(i==(oneR.size()-1)){
                        if(oneR[i]==left)
                            continue;
                        unordered_set<std::string> left_follow = getFollow(left);
                        tmp_set.insert(left_follow.begin(), left_follow.end());
                    }else{
                        //B->aSb
                        unordered_set<std::string> beta_first = getFirst(oneR[i+1]);
                        if(beta_first.find("@")!=beta_first.end()&&oneR[i]!=left){
                            unordered_set<std::string> left_follow = getFollow(left);
                            tmp_set.insert(left_follow.begin(), left_follow.end());
                        }
                        beta_first.erase("@");
                        tmp_set.insert(beta_first.begin(), beta_first.end());
                    }
                }
            }
        }
    }
    follow_set[S] = tmp_set;
    return follow_set[S];
}