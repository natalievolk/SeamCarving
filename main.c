#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <math.h>


int main() {

    int *path;
    recover_path(best_arr, grad->height, grad->width, &path);

    for (int i = 0; i < grad->height; i++) {
       printf("%d\n", path[i]);
    }

    destroy_image(im);
    destroy_image(grad);

    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "Photos/HJoceanSmall.bin");
    
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

