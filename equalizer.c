#include <stdio.h>
#include <stdlib.h>


struct pixel {
  int r, g, b;
};
typedef struct pixel Pixel;

struct image {
  Pixel *pixels;
  int height, width, max;
};
typedef struct image Image;


int main(int argc, char const *argv[]) {
  Image *img = (Image*) malloc(sizeof(struct image));
  FILE *fp;
  char c;
  int identifier, i, j = 1;

  fp = fopen(argv[1], "r");
  fscanf(fp, "%c%d %d %d %d\n", &c, &identifier, &img->width, &img->height, &img->max);

  img->pixels = (Pixel*) malloc(sizeof(struct pixel) * img->width * img->height);
  i = 0;
  while(j > 0) {
    j = fscanf(fp, "%d", &(&img->pixels[i])->r);
    j = j > 0 ? fscanf(fp, "%d", &(&img->pixels[i])->g) : 0;
    j = j > 0 ? fscanf(fp, "%d", &(&img->pixels[i])->b) : 0;
    i++;
  }

  FILE *fp2 = fopen("test.ppm", "w");
  fprintf(fp2, "%c%d %d %d %d\n", c, identifier, img->width, img->height, img->max);
  j = (img->width * img->height);
  for(i = 0; i < j; i++)
    fprintf(fp2, "%d %d %d\n", img->pixels[i].r/2, img->pixels[i].g/2, img->pixels[i].b/2);

  return 0;
}
