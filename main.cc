#include <string>
#include "bfcfg.h"

int main() {
    std::string code = "+++[->,.[+++---]<]---";
    BfProgram prog {code};
    BasicBlock *cfg = prog.generate_cfg();
    /* ubracket_map brackets = get_bracket_map(code); */
}
