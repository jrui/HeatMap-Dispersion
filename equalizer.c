#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct pixel {
  int r, g, b;
};
typedef struct pixel Pixel;

struct image {
  Pixel *pixels;
  int height, width, max, ident;
  char letter;
};
typedef struct image Image;


Image *read_image_file(char *file) {
  Image *img = (Image*) malloc(sizeof(struct image));
  FILE *fp;
  int i = 0, j = 1;

  fp = fopen(file, "r");
  fscanf(fp, "%c%d %d %d %d\n", &img->letter, &img->ident, &img->width, &img->height, &img->max);

  img->pixels = (Pixel*) malloc(sizeof(struct pixel) * img->width * img->height);
  while(j > 0) {
    j = fscanf(fp, "%d", &(&img->pixels[i])->r);
    j = j > 0 ? fscanf(fp, "%d", &(&img->pixels[i])->g) : 0;
    j = j > 0 ? fscanf(fp, "%d", &(&img->pixels[i])->b) : 0;
    i++;
  }
  fclose(fp);

  return img;
}


void write_image(char *file, Image *img) {
  int i, j;

  strcat(file, ".ppm");
  FILE *fp2 = fopen(file, "w");

  fprintf(fp2, "%c%d %d %d %d\n", img->letter, img->ident, img->width, img->height, img->max);
  j = (img->width * img->height);
  for(i = 0; i < j; i++)
    fprintf(fp2, "%d %d %d\n", img->pixels[i].r, img->pixels[i].g, img->pixels[i].b);

}


Image *heatmap(Image *img, int n_iter) {
  Pixel *pixels = img->pixels;
  int cols = img->width, rows = img->height;
  int i, j, k;
  printf("%d", &img->pixels[0].r);

  for(k = 0; k < n_iter; k++) {
    for(i = 0; i < cols; i++) {
      for(j = 0; j < rows; j++) {
        if(i == 0) {
          if(j == 0) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i+1)*j].r + pixels[i*(j+1)].r) / 3);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i+1)*j].g + pixels[i*(j+1)].g) / 3);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i+1)*j].b + pixels[i*(j+1)].b) / 3);
          }
          else if(j == rows - 1) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[i*(j+1)].r) / 3);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[i*(j+1)].g) / 3);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[i*(j+1)].b) / 3);
          }
          else {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[(i+1)*j].r + pixels[i*(j+1)].r) / 4);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[(i+1)*j].g + pixels[i*(j+1)].g) / 4);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[(i+1)*j].b + pixels[i*(j+1)].b) / 4);
          }
        }
        else if(i == cols - 1) {
          if(j == 0) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i+1)*j].r + pixels[i*(j-1)].r) / 3);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i+1)*j].g + pixels[i*(j-1)].g) / 3);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i+1)*j].b + pixels[i*(j-1)].b) / 3);
          }
          else if(j == rows - 1) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[i*(j-1)].r) / 3);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[i*(j-1)].g) / 3);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[i*(j-1)].b) / 3);
          }
          else {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[(i+1)*j].r + pixels[i*(j-1)].r) / 4);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[(i+1)*j].g + pixels[i*(j-1)].g) / 4);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[(i+1)*j].b + pixels[i*(j-1)].b) / 4);
          }
        }
        else {
          if(j == 0) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i+1)*j].r + pixels[i*(j+1)].r + pixels[i*(j-1)].r) / 4);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i+1)*j].g + pixels[i*(j+1)].g + pixels[i*(j-1)].g) / 4);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i+1)*j].b + pixels[i*(j+1)].b + pixels[i*(j-1)].b) / 4);
          }
          else if(j == rows - 1) {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[i*(j+1)].r + pixels[i*(j-1)].r) / 4);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[i*(j+1)].g + pixels[i*(j-1)].g) / 4);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[i*(j+1)].b + pixels[i*(j-1)].b) / 4);
          }
          else {
            pixels[i*j].r = (int) floor((pixels[i*j].r + pixels[(i-1)*j].r + pixels[(i+1)*j].r + pixels[i*(j+1)].r + pixels[i*(j-1)].r) / 5);
            pixels[i*j].g = (int) floor((pixels[i*j].g + pixels[(i-1)*j].g + pixels[(i+1)*j].g + pixels[i*(j+1)].g + pixels[i*(j-1)].g) / 5);
            pixels[i*j].b = (int) floor((pixels[i*j].b + pixels[(i-1)*j].b + pixels[(i+1)*j].b + pixels[i*(j+1)].b + pixels[i*(j-1)].b) / 5);
          }
        }
      }
    }
  }

  return img;
}


int main(int argc, char const *argv[]) {
  if(argc < 2)
    printf("Please specify input and output files.\n");
  else {
    Image *img = read_image_file(strdup(argv[1]));
    img = heatmap(img, 100);
    write_image(strdup(argv[2]), img);
  }
  return 0;
}
