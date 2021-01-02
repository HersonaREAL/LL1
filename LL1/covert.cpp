#include"LL1.h"
using std::list;
using std::string;
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
                    std::cout << (*it)[0] << std::endl;
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