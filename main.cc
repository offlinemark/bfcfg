#include <string>
#include "bfcfg.h"

#include <cstdio>
#include <cstdlib>
#include <queue>

struct cntxt_struct {
    unsigned cnt;
};

void bfs_callback_dot(void *context, BasicBlock *bb) {
    // Example line this function produces
    // 0[shape=record,label="{0 | +++[}"];0->4;0->18;

    struct cntxt_struct *x = (struct cntxt_struct*) context;
    x->cnt++;

    std::cout << bb->addr << "[shape=record,label=\"{" << bb->addr
        << " | ";
    for (char c : bb->instructions) {
        if (c == '<' || c == '>')
            std::cout << "\\";
        std::cout << c;
    }
    std::cout << "}\"];";

    std::cout << bb->addr << ":s->";
    if (bb->true_bb)
        std::cout << bb->true_bb->addr;
    else
        std::cout << "None";
    std::cout << ":c[label=\"T\"];";

    std::cout << bb->addr << ":s->";
    if (bb->false_bb)
        std::cout << bb->false_bb->addr;
    else
        std::cout << "None";
    std::cout << ":c[label=\"F\"];";
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    std::string code[] = {"+++[->,.[+++---]<]---",
     "++++[>+++++<-]>[<+++++>-]+<+[>[>+>+<<-]++>>[<<+>>-]>>>[-]++>[-]+>>>+[[-]++++++>>>]<<<[[<++++++++<++>>-]+<.<[>----<-]<]<<[>>>>>[>>>[-]+++++++++<[>-<-]+++++++++>[-[<->-]+[<<<]]<[>+<-]>]<<-]<<-]",
     "+++[->,.<]",
     "+++[->,.<]---",
     ",."};

    BfProgram prog {code[atoi(argv[1])]};
    prog.generate_cfg();
    std::cout << "digraph graf {\n";
    struct cntxt_struct yo{};
    prog.bfs(&yo, bfs_callback_dot);
    std::cout << "None[shape=record];\n";
    std::cout << "}\n";
    /* std::cout << "Num basic blocks " << yo.cnt << std::endl; */
    return 0;
}
