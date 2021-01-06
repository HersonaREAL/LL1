#include<iostream>
#include<unordered_map>
#include<algorithm>
#include<map>
#include<unordered_set>
#include<vector>
#include<list>
#include<string>


#ifndef LL_1
#define LL_1
class LL1
{
private:
    //宏声明
    using prodc_input = std::string;//产生式输入
    using prodc_output = std::vector<std::string>;//产生式输出 eg. E -> A B C
    using Production = std::unordered_map<prodc_input, std::list<prodc_output>>;
    using Production_tmp = std::vector<std::pair<prodc_input, std::list<prodc_output>>>;
    using Production__lst_tmp = std::list<std::pair<prodc_input, std::list<prodc_output>>>;
    using First_set = std::unordered_map<std::string, std::unordered_set<std::string>>;
    using Follow_set = std::unordered_map<std::string, std::unordered_set<std::string>>;

    //变量声明
    std::vector<std::string> origin_input;//最原始的输入，即文法
    Production raw_production;//未消除左递归的产生式
    Production_tmp tmp_production;//中间产物
    Production__lst_tmp lst_tmp_prodc; //中间产物
    Production cooked_production;//已处理好的产生式
    std::unordered_set<std::string> terminal;//终结符
    std::unordered_set<std::string> non_terminal;//非终结符
    First_set first_set;//首符集
    Follow_set follow_set;//随符集
    std::map<prodc_input, std::map<prodc_output, std::unordered_set<std::string>>> right_first_set;//候选式首符集合
    std::unordered_map<prodc_input, std::unordered_map<std::string, prodc_output>> analysis_table;//预测分析表
    bool isInit = false;
    std::string start_symbol;//开始符号

    //函数声明
    bool cookProduction();//消除左递归并提取左公因子
    bool generateFirstSet();//得到first集
    bool generateFollowSet();//得到follow集合
    void printCur(const std::list<std::string> &stk,const std::list<std::string> &content,std::list<std::string>::const_iterator it);//打印parse过程中栈，剩余输入符等信息
    void string2production();
    void removeDirect(Production_tmp &pt, int i);//消除直接左递归
    void simplifiy();
    void removeLeftCommonFactor();//消除左公因子
    void dfs(const std::string &S,std::unordered_map<prodc_input, bool> &used);
    std::unordered_set<std::string> getFirst(std::string S);
    std::unordered_set<std::string> getFollow(std::string S);
public:
    LL1() = default;
    LL1(std::istream &is);
    bool generate_table();
    bool parse(std::istream &is);
    void init(std::istream &is);
    void print();
};



#endif