#include <string>
#include "bfcfg.h"

#include <cstdio>
#include <queue>

void visit(BasicBlock *bb) {
    std::cout << bb << " "<< bb->addr << std::endl;
}

void bfs(BasicBlock *cfg) {
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
        visit(bb);
        if (bb->true_bb) {
            q.push(bb->true_bb);
        }
        if (bb->false_bb) {
            q.push(bb->false_bb);
        }
    }


}

int main(int argc, char **argv) {
    /* std::string code = "+++[->,.[+++---]<]---"; */
    /* std::string code = "+++[->,.<]"; */
    std::string code = "+++[->,.<]---";
    /* std::string code = ",."; */
    BfProgram prog {code};
    prog.generate_cfg();
    BasicBlock *cfg = prog.cfg;
    bfs(cfg);
    return 0;
}
