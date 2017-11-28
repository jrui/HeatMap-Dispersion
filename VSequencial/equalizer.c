#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct pixel {
  int r, g, b;
};
typedef struct pixel Pixel;

struct image {
  Pixel **pixels;
  int height, width, max, ident;
  char letter;
};
typedef struct image Image;


Image *read_image_file(char *file) {
  Image *img = (Image*) malloc(sizeof(struct image));
  FILE *fp;
  int i, j;

  fp = fopen(file, "r");
  fscanf(fp, "%c%d %d %d %d\n", &img->letter, &img->ident, &img->width, &img->height, &img->max);

  img->pixels = (Pixel**) malloc(sizeof(struct pixel) * img->height);
  for(i = 0; i < img->height; i++) {
    img->pixels[i] = (Pixel*) malloc(sizeof(struct pixel) * img->width);
    for(j = 0; j < img->width; j++) {
      fscanf(fp, "%d", &(&img->pixels[i][j])->r);
      fscanf(fp, "%d", &(&img->pixels[i][j])->g);
      fscanf(fp, "%d", &(&img->pixels[i][j])->b);
    }
  }
  fclose(fp);

  return img;
}


void write_image(char *file, Image *img) {
  int i, j;

  strcat(file, ".ppm");
  FILE *fp2 = fopen(file, "w");

  fprintf(fp2, "%c%d %d %d %d\n", img->letter, img->ident, img->width, img->height, img->max);
  for(i = 0; i < img->height; i++)
    for(j = 0; j < img->width; j++)
      fprintf(fp2, "%d %d %d\n", img->pixels[i][j].r, img->pixels[i][j].g, img->pixels[i][j].b);
}


Image *heatmap(Image *img, int n_iter) {
  Pixel **p2 = img->pixels;
  int cols = img->width, rows = img->height;
  int i, j, k;

  Pixel **p1 = (Pixel**) malloc(sizeof(struct pixel) * img->height);
  for(i = 0; i < img->height; i++)
    p1[i] = (Pixel*) malloc(sizeof(struct pixel) * img->width);

  for(k = 0; k < n_iter; k++) {
    for(i = 0; i < rows; i++) {
      for(j = 0; j < cols; j++) {
        if(i == 0) {
          //Primeira linha
          if(j == 0) {
            //[0][0]
            p1[i][j].r = floor((p2[i][j].r + p2[i+1][j].r + p2[i][j+1].r) / 3);
            p1[i][j].g = floor((p2[i][j].g + p2[i+1][j].g + p2[i][j+1].g) / 3);
            p1[i][j].b = floor((p2[i][j].b + p2[i+1][j].b + p2[i][j+1].b) / 3);
          }
          else {
            if(j == cols - 1) {
              //[0][cols]
              p1[i][j].r = floor((p2[i][j].r + p2[i+1][j].r + p2[i][j-1].r) / 3);
              p1[i][j].g = floor((p2[i][j].g + p2[i+1][j].g + p2[i][j-1].g) / 3);
              p1[i][j].b = floor((p2[i][j].b + p2[i+1][j].b + p2[i][j-1].b) / 3);
            }
            else {
              //[0][0<j<cols]
              p1[i][j].r = floor((p2[i][j].r + p2[i+1][j].r + p2[i][j-1].r + p2[i][j+1].r) / 4);
              p1[i][j].g = floor((p2[i][j].g + p2[i+1][j].g + p2[i][j-1].g + p2[i][j+1].g) / 4);
              p1[i][j].b = floor((p2[i][j].b + p2[i+1][j].b + p2[i][j-1].b + p2[i][j+1].b) / 4);
            }
          }
        }
        else {
          if(i == rows - 1) {
            //ultima linha
            if(j == 0) {
              //[rows][0]
              p1[i][j].r = floor((p2[i][j].r + p2[i-1][j].r + p2[i][j+1].r) / 3);
              p1[i][j].g = floor((p2[i][j].g + p2[i-1][j].g + p2[i][j+1].g) / 3);
              p1[i][j].b = floor((p2[i][j].b + p2[i-1][j].b + p2[i][j+1].b) / 3);
            }
            else {
              if(j == cols - 1) {
                //[rows][cols]
                p1[i][j].r = floor((p2[i][j].r + p2[i-1][j].r + p2[i][j-1].r) / 3);
                p1[i][j].g = floor((p2[i][j].g + p2[i-1][j].g + p2[i][j-1].g) / 3);
                p1[i][j].b = floor((p2[i][j].b + p2[i-1][j].b + p2[i][j-1].b) / 3);
              }
              else {
                //[rows][0<j<cols]
                p1[i][j].r = floor((p2[i][j].r + p2[i-1][j].r + p2[i][j+1].r + p2[i][j-1].r) / 4);
                p1[i][j].g = floor((p2[i][j].g + p2[i-1][j].g + p2[i][j+1].g + p2[i][j-1].g) / 4);
                p1[i][j].b = floor((p2[i][j].b + p2[i-1][j].b + p2[i][j+1].b + p2[i][j-1].b) / 4);
              }
            }
          }
          else {
            if(j == 0) {
              //primeira coluna
              //[0<x<rows][0]
              p1[i][j].r = floor((p2[i][j].r + p2[i+1][j].r + p2[i-1][j].r + p2[i][j+1].r) / 4);
              p1[i][j].g = floor((p2[i][j].g + p2[i+1][j].g + p2[i-1][j].g + p2[i][j+1].g) / 4);
              p1[i][j].b = floor((p2[i][j].b + p2[i+1][j].b + p2[i-1][j].b + p2[i][j+1].b) / 4);
            }
            else {
              if(j == rows - 1) {
                //ultima coluna
                p1[i][j].r = floor((p2[i][j].r + p2[i-1][j].r + p2[i+1][j].r + p2[i][j-1].r) / 4);
                p1[i][j].g = floor((p2[i][j].g + p2[i-1][j].g + p2[i+1][j].g + p2[i][j-1].g) / 4);
                p1[i][j].b = floor((p2[i][j].b + p2[i-1][j].b + p2[i+1][j].b + p2[i][j-1].b) / 4);
              }
              else {
                p1[i][j].r = floor((p2[i][j].r + p2[i-1][j].r + p2[i+1][j].r + p2[i][j+1].r + p2[i][j-1].r) / 5);
                p1[i][j].g = floor((p2[i][j].g + p2[i-1][j].g + p2[i+1][j].g + p2[i][j+1].g + p2[i][j-1].g) / 5);
                p1[i][j].b = floor((p2[i][j].b + p2[i-1][j].b + p2[i+1][j].b + p2[i][j+1].b + p2[i][j-1].b) / 5);
              }
            }
          }
        }
      }
    }
    p2 = p1;
  }

  img->pixels = p1;
  return img;
}


int main(int argc, char const *argv[]) {
  if(argc < 2)
    printf("Please specify input and output files.\n");
  else {
    Image *img = read_image_file(strdup(argv[1]));
    img = heatmap(img, 500);
    write_image(strdup(argv[2]), img);
  }
  return 0;
}
