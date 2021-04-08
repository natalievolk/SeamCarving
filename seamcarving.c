#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <float.h>

#include "seamcarving.h"
#include "c_img.h"

// pixel (y, x) --> (row, col) --> row y and column x

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    create_img(grad, im->height, im->width);

    double d_x, d_y;
    double rx, gx, bx, ry, gy, by;
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

            //printf("(%d, %d, %d)\n", get_pixel(im, y, x, 0), get_pixel(im, y, x, 1), get_pixel(im, y, x, 2));

            rx = (double) (get_pixel(im, y, x_p, 0) - get_pixel(im, y, x_l, 0));
            gx = (double) (get_pixel(im, y, x_p, 1) - get_pixel(im, y, x_l, 1));
            bx = (double) (get_pixel(im, y, x_p, 2) - get_pixel(im, y, x_l, 2));

            ry = (double) (get_pixel(im, y_p, x, 0) - get_pixel(im, y_l, x, 0));
            gy = (double) (get_pixel(im, y_p, x, 1) - get_pixel(im, y_l, x, 1));
            by = (double) (get_pixel(im, y_p, x, 2) - get_pixel(im, y_l, x, 2));

            // d_x = sqrt( rx^2 + gx^2 + bx^2 )
            d_x = pow(rx, 2) + pow(gx, 2) + pow(bx, 2);
            d_y = pow(ry, 2) + pow(gy, 2) + pow(by, 2);

            energy = (int) ( pow(d_x + d_y, 0.5 ) / 10 );
            if (energy > 255) energy = 255;

            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

int min_2(int v1, int v2) {
    if (v1 <= v2)    return v1;
    else            return v2;
    
}

int min_3(int v1, int v2, int v3) {
    if (v1 <= v2 && v1 <= v3)         return v1;
    else if (v2 <= v1 && v2 <= v3)    return v2;
    else                            return v3;
}

// NOTE: seam NOT allowed to wrap around edges
void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    *best_arr = (double *)malloc(sizeof(double)*(grad->height * grad->width));

    int width = grad->width;

    for (int j = 0; j < width; j++) {
        (*best_arr)[j] = get_pixel(grad, 0, j, 0);
    }

    for (int i = 1; i < grad->height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                (*best_arr)[i*width + j] =  get_pixel(grad, i, j, 0) 
                                + min_2((*best_arr)[(i-1) * width + j], (*best_arr)[(i-1) * width + j+1]);
            }
            else if (j == width - 1) {
                (*best_arr)[i*width + j] =  get_pixel(grad, i, j, 0) 
                                + min_2((*best_arr)[(i-1) * width + j], (*best_arr)[(i-1) * width + j-1]);
            }
            else {
                (*best_arr)[i*width + j] =  get_pixel(grad, i, j, 0) 
                                + min_3((*best_arr)[(i-1) * width + j+1], (*best_arr)[(i-1) * width + j], 
                                    (*best_arr)[(i-1) * width + j-1]);
            }
        }
    }
}


void recover_path(double *best, int height, int width, int **path) {
    *path = (int *)malloc(sizeof(int) * height);
    //double min = DBL_MAX;
    double min = -100;
    int min_location = 0;

    // find min in last row
    for (int j = 0; j < width; j++) {
        if (min < 0  ||  best[(height-1)*width + j] < min) {
            min = best[(height-1)*width + j];
            min_location = j;
        }
    }

    (*path)[height-1] = min_location;
    
    int offset;
    for (int i = height-2; i >= 0; i--) {
        offset = 0;
        min = best[i*width + min_location];

        //printf("%d\n", i);
        //printf("%f, %f, %f\n", best[i*width + min_location], 
        //                best[i*width + min_location + 1], best[i*width + min_location - 1]);

        if (min_location + 1 < width && min > best[i*width + min_location + 1]) {
            min = best[i*width + min_location + 1];
            offset = 1;
        }
        if (min_location - 1 >= 0 && min >= best[i*width + min_location - 1]) {
            min = best[i*width + min_location - 1];
            offset = -1;
        }
        //printf("%f\n\n", min);

        min_location = min_location + offset;

        (*path)[i] = min_location;
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    create_img(dest, src->height, src->width - 1);

    int skip;
    uint8_t red, green, blue;
    int set_x;

    for (int i = 0; i < src->height; i++) {
        skip = path[i];
        set_x = 0;

        for (int j = 0; j < src->width; j++) {
            if (j == skip) { 
                set_x--;
            } else {
                red =  get_pixel(src, i, j, 0);
                green = get_pixel(src, i, j, 1);
                blue = get_pixel(src, i, j, 2);

                set_pixel(*dest, i, set_x, red, green, blue);
            }
            set_x++;
        }
    }
}