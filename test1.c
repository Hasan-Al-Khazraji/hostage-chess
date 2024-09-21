#include "hclib.h"

#include <stdlib.h>

int main( int argc, char **argv )
{

  exboard_t *board;

  board = newboard();
  free( board );


  return 0;
}

