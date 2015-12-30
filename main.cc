#include <string>
#include "bfcfg.h"

int main(int argc, char **argv) {
    std::string code = "+++[->,.[+++---]<]---";
    /* std::string code = "+++[->,.<]"; */
    /* std::string code = ",."; */
    BfProgram prog {code};
    /* prog.generate_cfg(); */
    /* BasicBlock *cfg = prog.cfg; */
    std::cout << prog.cfg << std::endl;
    /* int x = 33; */
    return 0;
}
