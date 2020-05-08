// Helper Functions
// jkern6 lcreedo1
// Joey Kern and Liam Creedon
// 601.220, Fall 2018

#include <assert.h>
#include "ppmIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp);

  Image * img = malloc(sizeof(Image));

  //char for reading in file lines
  const int max = 255;
  char str[max];

  //this is for the header
  if (fgets(str, max, fp) == NULL) {
    free(img);
    img = NULL;
    return img;
  }

  //line after header
  if (fgets(str, max, fp) == NULL) {
    free(img);
    img = NULL;
    return img;
  }

  while (str[0] == '#') {
    if (fgets(str, max, fp) == NULL) {
      free(img);
      img = NULL;
      return img;
    }
  }

  sscanf(str, "%d %d", &img->cols, &img->rows);

  int colors;

  if (fscanf(fp, "%d", &colors) != 1) {
    free(img);
    img = NULL;
    return img;
  }

  if (colors != max) {
    free(img);
    img = NULL;
    return img;
  }

  //ignores new line char after colors
  getc(fp);
  
  img->data = malloc(sizeof(Pixel) * img->cols * img->rows);
  if ((int) fread(img->data, sizeof(Pixel), (img->cols * img->rows), fp) != (img->cols * img->rows)) {
    free(img->data);
    free(img);
    img = NULL;
    return img;
  }

  return img;
  
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp);
  if (!fp) {
    return 7;
  }
  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);
  
  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->rows * im->cols, fp);

  if (num_pixels_written != im->rows * im->cols) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
    return 7;
  }

  return num_pixels_written;
}
