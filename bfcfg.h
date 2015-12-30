#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <string>
#include <memory>

#include <cstdio>

using BracketMap = std::map<size_t, size_t>;


struct BasicBlock {
    size_t addr;
    size_t true_addr;
    size_t false_addr;
    BasicBlock *true_bb;
    BasicBlock *false_bb;
    std::vector<char> instructions;
};


class BfProgram {
    static const char BF_INSTR_SKIPFWD = '[';
    static const char BF_INSTR_SKIPBACK = ']';
    static std::unordered_set<char> BF_CF_INSTRS;

    const std::string code;
    const size_t last_valid_pc;
    BracketMap brackets;

    BracketMap get_bracket_map(const std::string &code) const;
    BasicBlock *_generate_cfg(size_t pc) const;
    void destroy_cfg(BasicBlock *cfg, std::unordered_set<BasicBlock*> &visited);
    BasicBlock *generate_bb(size_t pc) const;

public:
    BfProgram(const std::string code):
        code(code),
        brackets(get_bracket_map(code)),
        last_valid_pc(code.length() - 1),
        cfg(nullptr)
    {
        BF_CF_INSTRS.emplace(BF_INSTR_SKIPFWD);
        BF_CF_INSTRS.emplace(BF_INSTR_SKIPBACK);
    };

    ~BfProgram();

    void generate_cfg() {
        cfg = _generate_cfg(0);
    }

    BasicBlock *cfg;
};

