#include <vector>
#include <map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <stack>
#include <string>
#include <memory>

#include <cstdio>

#include "bfcfg.h"

const char BfProgram::BF_INSTR_SKIPFWD = '[';
const char BfProgram::BF_INSTR_SKIPBACK = ']';
const std::unordered_set<char> BfProgram::BF_CF_INSTRS{BF_INSTR_SKIPFWD,
        BF_INSTR_SKIPBACK};

BfProgram::~BfProgram() {
    dfs([](BasicBlock *cfg){
        delete cfg;
    });
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

void BfProgram::_dfs(BasicBlock *cfg,
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
        _dfs(saved_true_bb, visited, callback);
    }
    if (saved_false_bb) {
        _dfs(saved_false_bb, visited, callback);
    }
}

void BfProgram::_bfs(BasicBlock *cfg,
                std::function<void(BasicBlock*)> callback) {
    std::queue<BasicBlock*> q;
    std::unordered_set<BasicBlock*> visited;

    q.push(cfg);

    while (!q.empty()) {
        BasicBlock *bb = q.front();
        q.pop();
        if (visited.count(bb)) {
            continue;
        }
        visited.emplace(bb);
        callback(bb);
        if (bb->true_bb) {
            q.push(bb->true_bb);
        }
        if (bb->false_bb) {
            q.push(bb->false_bb);
        }
    }

}
