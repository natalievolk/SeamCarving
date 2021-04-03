#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "seamcarving.h"
#include "c_img.h"

// pixel (y, x) --> (row, col) --> row y and column x

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    create_img(grad, im->height, im->width);

    int d_x, d_y;
    int rx, gx, bx, ry, gy, by;
    uint8_t energy;

    // y_p = y+1, y_l = y-1, etc
    int y_p, y_l;
    int x_p, x_l;

    for(int y = 0; y < im->height; y++){
        for(int x = 0; x < im->width; x++){

            // dealing with wraparounds -- CHECK!!
            if (y+1 == im->height) y_p = 0;
            else y_p = y+1;
            if (y-1 < 0) y_l = im->height - 1;
            else y_l = y-1;

            if (x+1 == im->width) x_p = 0;
            else x_p = x+1;
            if (x-1 < 0) x_l = im->width - 1;
            else x_l = x-1;


            rx = (int) (get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0));
            gx = (int) (get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1));
            bx = (int) (get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2));

            ry = (int) (get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0));
            gy = (int) (get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1));
            by = (int) (get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2));

            // d_x = sqrt( rx^2 + gx^2 + bx^2 )
            d_x = pow( pow(rx, 2) + pow(gx, 2) + pow(bx, 2) , 0.5 );
            d_y = pow( pow(ry, 2) + pow(gy, 2) + pow(by, 2) , 0.5 );

// NOTE:    is it okay to just truncate here, or should I round?
            energy = (int) ( pow ( pow(d_x, 2) + pow(d_y, 2) ) / 10 );
            if (energy > 255) energy = 255;

            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    return;
}
void recover_path(double *best, int height, int width, int **path) {
    return;
}
void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    return;
}
