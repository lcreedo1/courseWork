// Header file for image manipulation
// jkern6 and lcreedo1
// Joey Kern and Liam Creedon

#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H
#include"ppmIO.h"
#include<math.h>

int function_pick(int argc, const char* argv[], Image* img); //decides which function to use

int swap(Image* img); //change RGB values, use temp holder

int grayscale(Image* img); //get intensity factor, intensity = 0.30*red + 0.59*green + 0.11*blue

int contrast(Image* img, const char* argv[]); //contrast factor param

unsigned char contrast_fun(unsigned char ch, double adj); //helper for contrast

int zoom_in(Image* img);

int zoom_out(Image* img);

int occlude(Image* img, const char* argv[]); //occlusion location param

int blur(Image* img, const char* argv[]); //blur factor param

double ** gaus_dist(double sigma, int n); //creates gaus distribution

Pixel weight(Image *img, int c, int d, double ** gaus, int n); //applies gaus distribution to pixel and surrounding

double sq(double num); //squares the number

#endif
