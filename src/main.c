#include "lex.h"
#include <stdio.h>
int main(int argc, char **argv) {
    struct Token *chain = lexFile(argv[1]);
    if(chain == 0) {
        return -1;
    }
    printTokenChain(chain);
    printf("\e[1;31mCompiled successfully\n");
    freeTokenChain(chain);
    return 0;
}
