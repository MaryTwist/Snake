#include "program.h"

int main(int argc, char** argv){
  program_t *p = program_create();

  program_init(p, 800, 600);
  program_run(p);
  program_close(p);

}
