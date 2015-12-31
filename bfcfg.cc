#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <string>
#include <memory>

#include <cstdio>

#include "bfcfg.h"

const char BfProgram::BF_INSTR_SKIPFWD;
const char BfProgram::BF_INSTR_SKIPBACK;
std::unordered_set<char> BfProgram::BF_CF_INSTRS;

BfProgram::~BfProgram() {
    std::unordered_set<BasicBlock*> visited;
    /* dfs(cfg, visited, [](BasicBlock *cfg){ */
    /*     /1* delete cfg; *1/ */
    /*         std::cout << cfg << std::endl; */
    /*         ; */
    /* }); */
    destroy_cfg(cfg, visited);
}

BasicBlock *BfProgram::generate_bb(size_t pc) const {
    static std::map<size_t, BasicBlock*> cache;

    if (pc > last_valid_pc) {
        return nullptr;
    }
    if (cache.count(pc)) {
        return cache[pc];
    }

    BasicBlock *bb = new BasicBlock;
    bb->addr = pc;
    size_t tmp_pc = pc;

    while (tmp_pc <= last_valid_pc && !BF_CF_INSTRS.count(code[tmp_pc])) {
        bb->instructions.emplace_back(code[tmp_pc]);
        tmp_pc++;
    }

    if (tmp_pc > last_valid_pc) {
        bb->true_addr = tmp_pc;
        bb->false_addr = tmp_pc;
    } else if (code[tmp_pc] == BF_INSTR_SKIPFWD) {
        bb->instructions.emplace_back(BF_INSTR_SKIPFWD);
        bb->true_addr = tmp_pc + 1;
        bb->false_addr = brackets.at(tmp_pc) + 1;
    } else if (code[tmp_pc] == BF_INSTR_SKIPBACK) {
        bb->instructions.emplace_back(BF_INSTR_SKIPBACK);
        bb->true_addr = brackets.at(tmp_pc) + 1;
        bb->false_addr = tmp_pc + 1;

        bb->true_bb = bb;
    }

    cache.emplace(pc, bb);

    return bb;
}

BasicBlock *BfProgram::_generate_cfg(size_t pc) const {
    if (pc > last_valid_pc) {
        return nullptr;
    }

    BasicBlock *bb = generate_bb(pc);
    if (!bb->true_bb) {
        bb->true_bb = _generate_cfg(bb->true_addr);
    }
    bb->false_bb = _generate_cfg(bb->false_addr);

    return bb;
}

BracketMap BfProgram::get_bracket_map (const std::string &code) const {
    BracketMap brackets;
    std::stack<size_t> stack;
    for (size_t i = 0; i < code.length(); i++) {
        switch(code[i]) {
            case '[':
                stack.push(i);
                break;
            case ']':
                size_t openbk = stack.top();
                stack.pop();
                brackets[i] = openbk;
                brackets[openbk] = i;
                break;
        }
    }
    return brackets;
}

void BfProgram::destroy_cfg(BasicBlock *cfg,
                            std::unordered_set<BasicBlock*> &visited) {
    if (!cfg || visited.count(cfg)) {
        return;
    }

    BasicBlock *saved_true_bb = cfg->true_bb;
    BasicBlock *saved_false_bb = cfg->false_bb;

    visited.emplace(cfg);
    /* std::cout << cfg << std::endl; */
    delete cfg;

    if (saved_true_bb) {
        destroy_cfg(saved_true_bb, visited);
    }
    if (saved_false_bb) {
        destroy_cfg(saved_false_bb, visited);
    }
}

void BfProgram::dfs(BasicBlock *cfg,
                    std::unordered_set<BasicBlock*> &visited,
                    std::function<void(BasicBlock*)> callback) {
    if (!cfg || visited.count(cfg)) {
        return;
    }

    BasicBlock *saved_true_bb = cfg->true_bb;
    BasicBlock *saved_false_bb = cfg->false_bb;

    visited.emplace(cfg);
    callback(cfg);
    /* delete cfg; */

    if (saved_true_bb) {
        destroy_cfg(saved_true_bb, visited);
    }
    if (saved_false_bb) {
        destroy_cfg(saved_false_bb, visited);
    }
}
