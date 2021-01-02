#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<string>


#ifndef LL_1
#define LL_1
class LL1
{
private:
    //宏声明
    using prodc_input = std::string;//产生式输入
    using prodc_output = std::vector<std::string>;//产生式输出 eg. E -> A B C
    using Production = std::unordered_map<prodc_input, std::vector<prodc_output>>;
    using First_set = std::unordered_map<std::string, std::unordered_set<std::string>>;
    using Follow_set = std::unordered_map<std::string, std::unordered_set<std::string>>;

    //变量声明
    std::vector<std::string> origin_input;//最原始的输入，即文法
    Production raw_production;//未消除左递归的产生式
    Production cooked_production;//已处理好的产生式
    std::unordered_set<std::string> terminal;//终结符
    std::unordered_set<std::string> non_terminal;//非终结符
    First_set first_set;//首符集
    Follow_set follow_set;//随符集
    std::unordered_map<prodc_input, std::unordered_map<std::string, prodc_output>> analysis_table;//预测分析表
    bool isInit = false;

    //函数声明
    bool cookProduction();//消除左递归
    bool generateFirstSet();//得到first集
    bool generateFollowSet();//得到follow集合
    void printCur();//打印parse过程中栈，剩余输入符等信息
    void string2production();
    void removeDirect();//消除直接左递归
    void removeIndirect();//处理间接左递归

public:
    LL1() = default;
    LL1(std::istream &is);
    bool generate_table();
    bool parse(std::istream &is);
    void init(std::istream &is);
    void print();
};



#endif