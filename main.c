#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv)
{

  screen s;
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  
  if (argc == 2)
    parse_file(argv[1], transform, edges, s);
  else
    parse_file("stdin", transform, edges, s);
 

  /*Transformation test 
  
  printf("Translate\n");
  print_matrix(make_translate(2, 4, 6));
  printf("Scale\n");
  print_matrix(make_scale(1, 3, 5));
  printf("Rot X\n");
  print_matrix(make_rotX(180));
  printf("Rot Y\n");
  print_matrix(make_rotY(180));
  printf("Rot Z\n");
  print_matrix(make_rotZ(180));
  */

  free_matrix(edges);
  free_matrix(transform);

  return 0;
}
