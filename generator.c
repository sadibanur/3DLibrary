/***
Author: Sadiba Nusrat Nusrat
Course: CSC352 
Description: The is an implementation the library. In this program,
        we have used the functions of the library to create different
        3D shapes.
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "3d.h"

int main() {
    //creating the 3D scene
    Scene3D* output = Scene3D_create();

    //object 1 (Pyramid)
    char* directions[] = {"up", "down", "left", "right", "forward", "backward"};
    Coordinate3D origin = (Coordinate3D){100, 100, 100};
    for (int i = 0; i <= 5; i ++) {
        Scene3D_add_pyramid(output, origin, 40, 50, directions[i]);
    }

    //object 2 (Cuboid)
    Coordinate3D coordinate;
    coordinate = (Coordinate3D){25, 25, 25}; 
    Scene3D_add_cuboid(output, coordinate, 50, 50, 50); // Eye
    coordinate = (Coordinate3D){15, 40, 0}; 
    Scene3D_add_cuboid(output, coordinate, 10, 10, 10); // Eye
    coordinate = (Coordinate3D){35, 40, 0}; 
    Scene3D_add_cuboid(output, coordinate, 10, 10, 10); // Mouth
    coordinate = (Coordinate3D){25, 15, 0}; 
    Scene3D_add_cuboid(output, coordinate, 30, 7, 10); 

    //object 3 (Sphere)
    Coordinate3D origin2 = (Coordinate3D){200, 200, 0};
    Scene3D_add_sphere(output, origin2, 45, 10);

    //object 4 (Fractal)
    Coordinate3D origin3 = (Coordinate3D) {100, 100, 0};
    Scene3D_add_fractal(output, origin3, 50, 5);
    
    Scene3D_write_stl_binary(output, "output.stl");
    Scene3D_destroy(output);
    return 0;
}