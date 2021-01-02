#include"LL1.h"
using std::string;
using std::vector;
bool LL1::cookProduction(){
    
    return true;
}
void LL1::removeDirect(){
    //P->Pa1|Pa2|...|Pam|b1|b2|...|bn
    //P->b1P'|b2P'|...|bnP'
    //P'->a1P'|a2P'|...|amP'|@
    
    for (auto Produ_it = raw_production.begin(); Produ_it != raw_production.end();++Produ_it){
        vector<prodc_output> alpha;
        vector<prodc_output> beta;
        const string &left = (*Produ_it).first;
        const string &new_left = left + "'";
        const vector<prodc_output> &all_right = (*Produ_it).second;

        //收集α和β
        for (auto right_it = all_right.begin(); right_it != all_right.end();++right_it){
            if((*right_it)[0]==left){
                //去除P，α后面加上P'
                alpha.emplace_back((*right_it).begin()+1,(*right_it).end());
                alpha.back().push_back(new_left);
            }
            else{
                beta.push_back(*right_it);
                //β后面加上P'
                beta.back().push_back(new_left);
            }
        }

        if(!alpha.empty()){
            beta.emplace_back(prodc_output({"@"}));//β集加入一个空串候选式
            cooked_production[left] = beta;
            cooked_production[new_left] = alpha;
        }else{
            cooked_production[left] = all_right;
        }

    }
}