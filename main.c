#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <math.h>


int main() {
    /*
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
    //struct rgb_img *dest;

    //remove_seam(im, &dest, path);

    //write_img(dest, "6x5_remove.bin");
    //set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b)
    //destroy_image(dest);
    

    destroy_image(im);
    destroy_image(grad);

    */


    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "HJoceanSmall.bin");
    
    for(int i = 0; i < 300; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        //if (i % 10 == 0) {
            char filename[200];
            sprintf(filename, "Images/bin/img%d.bin", i);
            write_img(cur_im, filename);
        //}

        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);

    return 0;
}

