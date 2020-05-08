// Image manipulation algoritms
// jkern6 lcreedo1
// Joey Kern and Liam Creedon

#include"ppmIO.h"
#include"imageManip.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include<math.h>

const int PI = 3.141592653589;

int function_pick(int argc, const char* argv[], Image* img) {


  //chooses function to call

  if (!strcmp(argv[3], "swap")) {
    if (argc != 4) { return 5; }
    
    else {
      //printf("Check\n");
      return swap(img);
    }
  }
  else if (!strcmp(argv[3], "grayscale")) {
    if (argc != 4) { return 5; }
    else { return grayscale(img); }
  }
  else if (!strcmp(argv[3], "contrast")) {
    if (argc != 5) { return 5; }
    else { return contrast(img, argv); }
  }
  else if (!strcmp(argv[3], "zoom_in")) {
    if (argc != 4) { return 5; }
    else { return zoom_in(img); }
  }
  else if (!strcmp(argv[3], "zoom_out")) {
    if (argc != 4) { return 5; }
    else { return zoom_out(img); }
  }
  else if (!strcmp(argv[3], "occlude")) {
    if (argc != 8) { return 5;}
    else { return occlude(img, argv); }
  }
  else if (!strcmp(argv[3], "blur")) {
    if (argc != 5) { return 5; }
    else { return blur(img, argv); }
  }
  else { return 4; }
    
}

int swap(Image* img) { //change RGB values, use temp holder
  unsigned char temp;
  int row_x_col = (img->cols) * (img->rows);

  for (int i = 0; i < row_x_col; i++) {

    temp = img->data[i].r;
    img->data[i].r = img->data[i].g;
    img->data[i].g = img->data[i].b;
    img->data[i].b = temp;
  }
  return 0;
}

int grayscale(Image* img) { //get intensity factor, intensity = 0.30*red + 0.59*green + 0.11*blue

  unsigned char intensity;
  Pixel temp;
  int row_x_col = (img->cols) * (img->rows);

  for (int i = 0; i < row_x_col; i++) {
    temp = img->data[i];

    //NTSC equation for grayscale
    intensity = (0.30*temp.r) + (0.59 * temp.g) + (0.11 * temp.b);

    //assign r g and b the same value so that it is a shade of gray
    temp = (Pixel) {intensity, intensity, intensity};
    img->data[i] = temp;

  }

  return 0;
}

int contrast(Image* img, const char* argv[]) { //contrast factor param

  double adjust;

  if (sscanf(argv[4], "%lf", &adjust) != 1) {
    return 5;
  }

  unsigned char r;
  unsigned char g;
  unsigned char b;
  Pixel temp;
  int row_x_col = (img->cols) * (img->rows);

  for (int i = 0; i < row_x_col; i++) {
    temp = img->data[i];

    //calls a helper function where the math for
    //contrast lives, called "contrast_fun"
    r = contrast_fun(temp.r, adjust);
    g = contrast_fun(temp.g, adjust);
    b = contrast_fun(temp.b, adjust);
    temp = (Pixel) {r, g, b};
    img->data[i] = temp;
  }
  
  return 0;
}

unsigned char contrast_fun(unsigned char c, double adj) {

  double d = (double) c;

  //have decimals here so it's not int division
  d = d - (255.0 / 2.0);
  d = d * adj;
  d = d + (255.0 / 2.0);
  
  if (d > 255.0) {
    d = 255.0;
  }
  
  if (d < 0.0) {
    d = 0.0;
  }
  
  c = (unsigned char) d;
  return c;
}

int zoom_in(Image* img) {

  int num_col = img->cols;
  int num_row = img->rows;
  int doub_col = num_col * 2;
  int doub_row = num_row * 2;
  Image * zoomed = (Image *) malloc(sizeof(Image));
  zoomed->cols = doub_col;
  zoomed->rows = doub_row;
  zoomed->data = (Pixel *) malloc(zoomed->rows * zoomed->cols * sizeof(Pixel));
  //printf("r: %d\nc: %d\nnr: %d\nnc: %d\n", num_row, num_col, doub_row, doub_col);

  if (!(zoomed->data)) {
    return 8;
  }

  for (int i = 0; i < num_row; i++) {
    for (int j = 0; j < num_col; j++) {
      for (int k = i*2; k < i*2 + 2; k++) {
	for (int l = j*2; l < j*2 + 2; l++) {
	  (zoomed->data + k*zoomed->cols + l)->r = (img->data + i*num_col + j)->r;
	  (zoomed->data + k*zoomed->cols + l)->g = (img->data + i*num_col + j)->g;
	  (zoomed->data + k*zoomed->cols + l)->b = (img->data + i*num_col + j)->b;
	}
      }
    }
  }

  int row_x_col = (zoomed->cols) * (zoomed->rows);
  img->data = realloc(img, doub_row * doub_col * sizeof(Pixel));
  //img = temp;
  img->rows = doub_row;
  img->cols = doub_col;

  for (int i = 0; i < row_x_col; i++) {

    img->data[i] = zoomed->data[i];

  }
  
  
  free(zoomed);
  return 0;
}

int zoom_out(Image* img) {

  int num_col = img->cols;
  int num_row = img->rows;
  int half_col = num_col / 2;
  int half_row = num_row / 2;
  Image * zoomed = (Image *) malloc(sizeof(Image));
  zoomed->cols = half_col;
  zoomed->rows = half_row;
  zoomed->data = (Pixel *) malloc(half_col * half_row * sizeof(Pixel));

  for (int i = 0; i < half_row; i++) {
    for (int j = 0; j < half_col; j++) {
      
      zoomed->data[i * half_col + j].r = (img->data[2 * i * num_col + 2 * j].r + img->data[(1 + 2 * i) * num_col + 2 * j].r + img->data[2 * i * num_col + 1 + 2 * j].r + img->data[(2 * i + 1) * num_col + 1 + 2 * j].r) / 4;

      zoomed->data[i * half_col + j].g = (img->data[2 * i * num_col + 2 * j].g + img->data[(1 + 2 * i) * num_col + 2 * j].g + img->data[2 * i * num_col + 1 + 2 * j].g + img->data[(2 * i + 1) * num_col + 1 + 2 * j].g) / 4;

      zoomed->data[i * half_col + j].b = (img->data[2 * i * num_col + 2 * j].b + img->data[(1 + 2 * i) * num_col + 2 * j].b + img->data[2 * i * num_col + 1 + 2 * j].b + img->data[(2 * i + 1) * num_col + 1 + 2 * j].b) / 4;

    }
  }

  int row_x_col = (zoomed->cols) * (zoomed->rows);

  img->rows = half_row;
  img->cols = half_col;
  
  for (int i = 0; i < row_x_col; i++) {
    
    img->data[i] = zoomed->data[i];

  }
  free(zoomed);
  return 0;
}

int occlude(Image* img, const char* argv[]) {

  //argv4 is irow argv5 is icol argv6 is frow argv7 is fcol
  int irow, icol, frow, fcol;

  if (sscanf(argv[4], "%d", &icol) != 1) {
    return 5;
  }
  if (sscanf(argv[5], "%d", &irow) != 1) {
    return 5;
  }
  if (sscanf(argv[6], "%d", &fcol) != 1) {
    return 5;
  }
  if (sscanf(argv[7], "%d", &frow) != 1) {
    return 5;
  }

  if (irow > frow || icol > fcol || irow < 0 || icol < 0 || frow > (img->rows) - 1 || fcol > (img->cols) - 1) {
    return 6;
  }
    for (int i = irow; i < frow; i++) {
      for (int j = icol; j < fcol; j++) {
	img->data[i*img->cols + j].r = 0;
	img->data[i*img->cols + j].g = 0;
	img->data[i*img->cols + j].b = 0;
      }
    }
  
  return 0;
}

int blur(Image* img, const char* argv[]) { //blur factor param

  double sigma;
  if (sscanf(argv[4], "%lf", &sigma) != 1) {
    return 5;
  }
  int n = (int)ceil(10*sigma);
  if (n % 2 == 0)  {
    n += 1;
  }

  double ** gaus = gaus_dist(sigma, n);

  Image *temp = malloc(sizeof(Image));
  if (!temp) { return 8; }

  int rows = img->rows;
  int cols = img->cols;
  temp->rows = rows;
  temp->cols = cols;
  temp->data = malloc(sizeof(Pixel) * temp->rows * temp->cols);
  if (!temp->data) { return 8; }

  for (int a = 0; a < rows; a++) {
    for (int b = 0; b < cols; b++) {
      *(temp->data + a*cols + b) = weight(img, a, b, gaus, n);
    }
  }

  for (int i =0; i < n; i++) {
    free(gaus[i]);
  }
  free(gaus);

  for (int i = 0; i < rows * cols; i++) {

    img->data[i] = temp->data[i];
    
  }
  free(temp->data);
  free(temp);
  return 0;
}

double ** gaus_dist(double sigma, int n) {
  double ** gaus = malloc(n * sizeof(double));
  assert(gaus);
  for (int x = 0; x < n; x++) {
    gaus[x] = malloc(n * sizeof(double));
    assert(gaus[x]);
  }
  for (int a = 0; a < n; a++) {
    int dx = abs(n/2 - a);
    for (int b = 0; b < n; b++) {
      int dy = abs(n/2 - b);
      gaus[a][b] = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
    }
  }
  return gaus;
}

Pixel weight(Image *img, int c, int d, double ** gaus, int n) {

  int rows = img->rows;
  int cols = img->cols;
  double red = 0;
  double blue = 0;
  double green = 0;
  int gausx = 0;
  int gausy = 0;
  double gaus_tot = 0;

  for (int i = c - n / 2; i <= c + n / 2; i++) {
    for (int j = d - n / 2; j <= d + n / 2; j++) {
      if (i > -1 && i < rows && j > -1 && j < cols) {

	//this is convoluding
	red += (img->data + i * cols + j)->r * gaus[gausx][gausy];
	green += (img->data + i * cols + j)->g * gaus[gausx][gausy];
	blue += (img->data + i * cols + j)->b * gaus[gausx][gausy];

	//adds to the sum
	gaus_tot += gaus[gausx][gausy];
      }
      
      gausy++;      
    }
    
    gausx++;
    gausy = 0;
  }

  Pixel pix;

  //divide the sum by the gaussian sum
  pix.r = (unsigned char) (red / gaus_tot);
  pix.g = (unsigned char) (green / gaus_tot);
  pix.b = (unsigned char) (blue / gaus_tot);

  //return newly created pixel
  return pix;
}

double sq(double num) {
  return num * num;
}
