#include <string>
#include "bfcfg.h"

#include <cstdio>
#include <queue>

void bfs_callback_dot(BasicBlock *bb) {
    // Example line this function produces
    // 0[shape=record,label="{0 | +++[}"];0->4;0->18;

    std::cout << bb->addr << "[shape=record,label=\"{" << bb->addr
        << " | ";
    for (char c : bb->instructions) {
        if (c == '<' || c == '>')
            std::cout << "\\";
        std::cout << c;
    }
    std::cout << "}\"];";

    std::cout << bb->addr << "->";
    if (bb->true_bb)
        std::cout << bb->true_bb->addr;
    else
        std::cout << "None";
    if (bb == bb->true_bb)
        std::cout << "[dir=back]";
    std::cout << ";";

    std::cout << bb->addr << "->";
    if (bb->false_bb)
        std::cout << bb->false_bb->addr;
    else
        std::cout << "None";
    std::cout << ";";
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    std::string code = "+++[->,.[+++---]<]---";
    /* std::string code = "+++[->,.<]"; */
    /* std::string code = "+++[->,.<]---"; */
    /* std::string code = ",."; */

    BfProgram prog {code};
    prog.generate_cfg();
    std::cout << "digraph graf {\n";
    prog.bfs(bfs_callback_dot);
    std::cout << "None[shape=record];\n";
    std::cout << "}\n";
    return 0;
}
