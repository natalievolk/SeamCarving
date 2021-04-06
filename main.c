#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <math.h>


int main() {
    struct rgb_img *im;
    char file[200] = "6x5.bin";
    read_in_img(&im, file);

    struct rgb_img *grad;

    calc_energy(im, &grad);

    //print_grad(grad);

    double *best_arr;

    dynamic_seam(grad, &best_arr);

    for (int i = 0; i < grad->height; i++) {
        for (int j = 0; j < grad->width; j++) {
            printf("%f ", best_arr[i*(grad->width) + j]);
        }
        printf("\n");
    }

    int *path;
    recover_path(best_arr, grad->height, grad->width, &path);

    for (int i = 0; i < grad->height; i++) {
        printf("%d\n", path[i]);
    }

    //write_img(grad, "3x4_gradient.bin");
    //write_img(grad, "6x5_gradient.bin");
    struct rgb_img *dest;

    remove_seam(im, &dest, path);

    write_img(dest, "6x5_remove.bin");
    //set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b)
    destroy_image(dest);

    destroy_image(im);
    destroy_image(grad);
    return 0;
}
