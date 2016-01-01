# bfcfg

it's a little control flow graph library for brainfuck.

`main.cc` is a sample client program that outputs to stdout the cfg in "dot"
representation for visualization.

    $ ./main 0 > cfg.dot
    $ dot -Tpng cfg.dot > cfg.png  # requires graphviz
