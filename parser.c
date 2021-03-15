#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file(char *filename,
                struct matrix *transform,
                struct matrix *edges,
                screen s)
{

  FILE *f;
  char line[256];
  clear_screen(s);
  int state = -1;
  char *current;
  color clr;
  clr.red = DEFAULT_COLOR;
  clr.blue = DEFAULT_COLOR;
  clr.green = 255;

  if (strcmp(filename, "stdin") == 0)
    f = stdin;
  else
    f = fopen(filename, "r");

  while (fgets(line, 255, f) != NULL)
  {
    line[strlen(line) - 2] = '\0';
    //printf(":%d\n", strlen(line));
    printf(":%s\n", line);
    //printf(":%d\n", state);

    if (state == 0) //line
    {
      current = line;
      double x0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double y0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double z0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double x1 = (double)strtol(strsep(&current, " "), NULL, 10);
      double y1 = (double)strtol(strsep(&current, " "), NULL, 10);
      double z1 = (double)strtol(strsep(&current, " "), NULL, 10);
      //printf("%f %f %f %f %f %f\n",x0,y0,z0,x1,y1,z1);
      add_edge(edges, x0, y0, z0, x1, y1, z1);
      state = -1;
    }
    if (state == 1) //scale
    {
      current = line;
      double x0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double y0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double z0 = (double)strtol(strsep(&current, " "), NULL, 10);
      //printf("%f %f %f\n",x0,y0,z0);
      struct matrix *m = new_matrix(4, 4);
      m = make_scale(x0, y0, z0);
      matrix_mult(m, transform);
      state = -1;
    }
    if (state == 2) //move
    {
      current = line;
      double x0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double y0 = (double)strtol(strsep(&current, " "), NULL, 10);
      double z0 = (double)strtol(strsep(&current, " "), NULL, 10);
      //printf("%f %f %f\n",x0,y0,z0);
      struct matrix *m = new_matrix(4, 4);
      m = make_translate(x0, y0, z0);
      matrix_mult(m, transform);
      state = -1;
    }
    if (state == 3) //rotate
    {
      current = line;
      char *axis = strsep(&current, " ");
      double theta = (double)strtol(strsep(&current, " "), NULL, 10);
      struct matrix *m = new_matrix(4, 4);
      //printf("%s %f\n", axis, theta);

      if (strcmp(axis, "x") == 0)
      {
        m = make_rotX(theta);
      }

      if (strcmp(axis, "y") == 0)
      {
        m = make_rotY(theta);
      }
      if (strcmp(axis, "z") == 0)
      {
        m = make_rotZ(theta);
      }
      matrix_mult(m, transform);
      state = -1;
    }
    if (state == 4) //save
    {
      clear_screen(s);
      draw_lines(edges, s, clr);
      save_extension(s, line);

      state = -1;
    }

    if (strcmp(line, "line") == 0)
    {
      state = 0;
    }
    if (strcmp(line, "scale") == 0)
    {
      state = 1;
    }
    if (strcmp(line, "move") == 0)
    {
      state = 2;
    }
    if (strcmp(line, "rotate") == 0)
    {
      state = 3;
    }
    if (strcmp(line, "save") == 0)
    {
      state = 4;
    }
    if (strcmp(line, "ident") == 0)
    {
      ident(transform);
    }
    if (strcmp(line, "apply") == 0)
    {
      matrix_mult(transform, edges);
    }
    if (strcmp(line, "display") == 0)
    {
      clear_screen(s);
      draw_lines(edges, s, clr);
      display(s);
    }
  }
}
