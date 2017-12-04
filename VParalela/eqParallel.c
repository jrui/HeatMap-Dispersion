#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>


struct image {
  int **pixels;
  int height, width, max, ident;
  char letter;
};
typedef struct image Image;


Image *read_image_file(char *file) {
  Image *img = (Image*) malloc(sizeof(struct image));
  FILE *fp;
  int i, j;

  fp = fopen(file, "r");
  fscanf(fp, "%c%d\n%d %d\n%d\n", &img->letter, &img->ident, &img->width, &img->height, &img->max);

  img->pixels = (int**) malloc(sizeof(int*) * img->height);
  for(i = 0; i < img->height; i++) {
    img->pixels[i] = (int*) malloc(sizeof(int) * img->width);
    for(j = 0; j < img->width; j++)
      fscanf(fp, "%d", &img->pixels[i][j]);
  }
  fclose(fp);

  return img;
}


void write_image(char *file, Image *img) {
  int i, j;

  FILE *fp2 = fopen(file, "w");

  fprintf(fp2, "%c%d\n%d %d\n%d\n", img->letter, img->ident, img->width, img->height, img->max);
  for(i = 0; i < img->height; i++)
    for(j = 0; j < img->width; j++)
      fprintf(fp2, "%d\n", img->pixels[i][j]);
}


Image *heatmap(Image *img, int n_iter) {
  int cols = img->width;
  int rows = img->height;
  int i, j, k;
  int **temp, **p;

  double timeInit = omp_get_wtime();


  p = (int**) malloc(sizeof(int*) * img->height);
  for(i = 0; i < img->height; i++)
    p[i] = (int*) malloc(sizeof(int) * img->width);

  #pragma omp parallel for
  for(i = 0; i < rows; i++) {
    p[i][0] = img->pixels[i][0];
    p[i][cols-1] = img->pixels[i][cols-1];
  }
  #pragma omp parallel for
  for(j = 0; j < cols; j++) {
    p[0][j] = img->pixels[0][j];
    p[rows-1][j] = img->pixels[rows-1][j];
  }

  for(k = 0; k < n_iter; k++) {
    #pragma omp parallel for
    for(i = 1; i < rows-1; i++) {
      #pragma omp parallel for
      for(j = 1; j < cols-1; j++)
        p[i][j] = (int) floor((img->pixels[i][j] + img->pixels[i-1][j] + img->pixels[i+1][j] + img->pixels[i][j+1] + img->pixels[i][j-1]) / 5);
    }
    temp = p;
    p = img->pixels;
    img->pixels = temp;
  }

  double timeFin = omp_get_wtime();
  printf("Tempo de cálculo: %f segundos\n", timeFin - timeInit);

  return img;
}


int main(int argc, char const *argv[]) {
  if(argc < 2)
    printf("Please specify input and output files.\n");
  else {
    Image *img = read_image_file(strdup(argv[1]));
    img = heatmap(img, atoi(argv[3]));
    write_image(strdup(argv[2]), img);
  }
  return 0;
}
