#include"LL1.h"
using std::list;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;
bool LL1::cookProduction(){
    tmp_production.assign(raw_production.begin(), raw_production.end());
    int n = tmp_production.size();
    for (int i = 0; i < n;++i){
        for (int j = 0; j < i;++j){
            const string &Aj = tmp_production[j].first;
            list<prodc_output> &Ai_right = tmp_production[i].second;
            const list<prodc_output> &Aj_right = tmp_production[j].second;
            //遍历Ai右边
            for (auto it = Ai_right.begin(); it != Ai_right.end();++it){
                if((*it)[0]==Aj){
                    //Ai->Ajγ
                    //产生新候选式
                    vector<prodc_output> new_right;
                    for (auto j_it = Aj_right.begin(); j_it != Aj_right.end();++j_it){
                        //替换AJ
                        prodc_output tmp((*j_it));
                        tmp.insert(tmp.end(), (*it).begin() + 1, (*it).end());

                        new_right.push_back(tmp);
                    }
                    Ai_right.insert(Ai_right.end(), new_right.begin(), new_right.end());
                    
                    //删除原候选式
                    it = Ai_right.erase(it);
                }
            }
        }
        removeDirect(tmp_production, i);
    }
    simplifiy();
    return true;
}
void LL1::removeDirect(Production_tmp &pt, int i){
    //P->Pa1|Pa2|...|Pam|b1|b2|...|bn
    //P->β1P'|β2P'|...|βnP'
    //P'->a1P'|a2P'|...|amP'|@
    
    list<prodc_output> alpha;
    list<prodc_output> beta;
    const string &left = pt[i].first;
    const string &new_left = left + "'";
    const list<prodc_output> &all_right = pt[i].second;
    for (auto right_it = all_right.begin(); right_it != all_right.end();++right_it){
        if((*right_it)[0]==left){
            //去除P，α后面加上P'
            alpha.emplace_back((*right_it).begin()+1,(*right_it).end());
            alpha.back().push_back(new_left);
        }else{
            beta.push_back(*right_it);
            //β后面加上P'
            beta.back().push_back(new_left);
        }
    }
    if(!alpha.empty()){
        alpha.emplace_back(prodc_output({"@"})); //α集加入一个空串候选式
        pt[i].second = beta;
        pt.push_back(std::make_pair(new_left, alpha));
    }
}

void LL1::simplifiy(){
    cooked_production.insert(tmp_production.begin(), tmp_production.end());
    unordered_map<prodc_input, bool> used;
    for (auto ptr = cooked_production.begin(); ptr != cooked_production.end();++ptr)
        used[(*ptr).first] = false;
    //从开始符号深度遍历,简化产生式
    dfs(start_symbol,used);

    //去除没有被遍历到的产生式
    for (auto ptr = used.cbegin(); ptr != used.cend();++ptr)
        if(!(*ptr).second)
            cooked_production.erase((*ptr).first);
            
    //记录终结符与非终结符
    for(auto ptr = cooked_production.begin(); ptr != cooked_production.end();++ptr){
        non_terminal.insert((*ptr).first);

        const list<prodc_output> &all_right = (*ptr).second;
        for (auto lst_it = all_right.cbegin(); lst_it != all_right.cend();++lst_it){
            for (auto str_it = (*lst_it).cbegin(); str_it != (*lst_it).cend();++str_it){
                if(cooked_production.find((*str_it))==cooked_production.end())
                    terminal.insert(*str_it);
            }
        }
    }
}
void LL1::dfs(const string &S,unordered_map<prodc_input, bool> &used){
    if(used[S])
        return;
    used[S] = true;
    const list<prodc_output> &all_right = cooked_production[S];
    for (auto lst_it = all_right.cbegin(); lst_it != all_right.cend();++lst_it){
        for (auto str_it = (*lst_it).cbegin(); str_it != (*lst_it).cend();++str_it){
            //深度遍历非终结符
            if(used.find((*str_it))!=used.end())
                dfs((*str_it),used); 
        }
    }
}