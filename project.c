// Main file
// jkern6 lcreedo1
// Joey Kern and Liam Creedon

#include<stdio.h>
#include"imageManip.h"
#include"ppmIO.h"
#include<string.h>

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    printf("Input or output file missing in command line.\n");
    return 1;
  } //error 1

  FILE* open = fopen(argv[1], "r");
  if (!open) {
    printf("Input file failed to open.\n");
    return 2;
  } //error 2
  
  Image* img = read_ppm(open);
  if (!img) {
    fclose(open);
    printf("Input file failed to be read.\n");
    return 3; //error 3
  }
  
  fclose(open);

  FILE* out = fopen(argv[2], "w");
  if (!out) {
    fclose(out);
    printf("Output file failed to be opened for writing.\n");
    return 7;
  }

  if (argc < 4) {
    fclose(out);
    printf("No operation name specified.\n");
    return 4; //error 4
  }
  
  int result = function_pick(argc, argv, img);
  
  if (result == 0) {
    int write = write_ppm(out, img);
    if (write == 7) {
      printf("Output file failed to be written to.\n");
      fclose(out);
      return write; //error 7
    }  
    fclose(out);
    return 0;
  }
  else if (result == 4) {
    printf("Invalid operation name.\n");
    return 4;
  }
  else if (result == 5) {
    printf("Incorrect number of arguments for specified function call.\n");
    return 5;
  }
  else if (result == 6) {
    printf("Occlude arguments out of range.\n");
    return 6;
  }
  /*else if (result == 7) {
    printf(

    }*/
  else if (result == 8) {
    printf("Unspecifiec error.\n");
    return 8;
  }
  fclose(out);
  return 0;
}
