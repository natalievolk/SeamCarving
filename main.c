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

    //write_img(grad, "3x4_gradient.bin");
    //write_img(grad, "6x5_gradient.bin");

    destroy_image(im);
    destroy_image(grad);
    return 0;
}