#include "board.h"

int main(int argc,char* argv[]) {
    
    int dim = 400;
    bool abilita_funzioni_avanzate = true;
    Board b(dim, 0.4, 0.7, abilita_funzioni_avanzate);
    b(1, 1) = Sir::i;
    //b.print(100);
    b.draw();
    return 1;
}
