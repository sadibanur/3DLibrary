/***
Author: Sadiba Nusrat Nusrat
Course: CSC352 
Description: The is an implementation of header file. The purpose of 
        this library is to provide an interface for creating a 3D scene, 
        adding shapes to this scene (represented via a linked list (ish) 
        data structure), and saving these scenes to .stl files so they can 
        be viewed in a variety of other software.
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "3d.h"


/*
This function is responsible for creating a new, empty 3D scene object. 
It needs to allocate heap memory for a Scene3D object and initialize 
the elements to 0 and NULL.
Return: This function returns the 3D scene.
*/
Scene3D* Scene3D_create() {
    //allocating memory for the whole 3D scene.
    Scene3D* scene = malloc(sizeof(Scene3D));
    scene->count = 0;
    scene->root = NULL;
    return scene;
}


/*
This function is responsible for freeing every chunk of allocated memory 
associated with this scene. It should free all memory from the Scene3D 
object itself, as well as the Triangle3DNode objects within the scene. 
It does not return anything.
Parameter: 
    scene: A funn 3D scene.
*/
void Scene3D_destroy(Scene3D* scene) {
    Triangle3DNode* node = scene->root;
    //itearing through the triangle node to free them 
    while (node != NULL) {
        Triangle3DNode* temp = node->next;
        free(node);
        node = temp;
    }
    free(scene);
}


/*
The function writes every object in the scene to an STL file with the name 
file_name. It should write the objects / triangles to the file in the order
that they appear in the scene object. 
Parameter:
    scene: A full 3D scene.
    file_name: The name of the .stl file
*/
void Scene3D_write_stl_text(Scene3D* scene, char* file_name) {
    //opening the file
    FILE* file = fopen(file_name, "w");
    //checking if the file can't be opened
    if (file == NULL) {
        exit(1);
    }
    fprintf(file, "solid scene\n");
    Triangle3DNode* node = scene->root;
    while (node != NULL) {
        fprintf(file, "  facet normal 0.0 0.0 0.0\n");
        fprintf(file, "    outer loop\n");
        fprintf(file, "      vertex %.5f %.5f %.5f\n", node->triangle.a.x, node->triangle.a.y, node->triangle.a.z);
        fprintf(file, "      vertex %.5f %.5f %.5f\n", node->triangle.b.x, node->triangle.b.y, node->triangle.b.z);
        fprintf(file, "      vertex %.5f %.5f %.5f\n", node->triangle.c.x, node->triangle.c.y, node->triangle.c.z);
        fprintf(file, "    endloop\n");
        fprintf(file, "  endfacet\n");
        node = node->next;
    }
    fprintf(file, "endsolid scene\n");
    //closing the file 
    fclose(file);
}


void Scene3D_add_triangle(Scene3D* scene, Triangle3D triangle) {
    Triangle3DNode** node = &(scene->root);
    Triangle3DNode* new_node = malloc(sizeof(Triangle3DNode));
    new_node->triangle = triangle;
    new_node->next = *node;
    *node = new_node;
    scene->count ++;
}


/*
The function create 4 traingles and and add them together to a 4
sided shape. This includes squares, rectangles, and other shapes with 
4 sides.
Parameter:
   scene: A full 3D scene.
   a, b, c, d: The coordinate of 4 sides of a shape
*/
void Scene3D_add_quadrilateral(Scene3D* scene,
    Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d) {
    Triangle3D triangle_1 = (Triangle3D) {a, b, c};
    Triangle3D triangle_2 = (Triangle3D) {b, c, d};
    Triangle3D triangle_3 = (Triangle3D) {a, c, d};
    Triangle3D triangle_4 = (Triangle3D) {a, b, d};
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
}


/** 
The function is a helper function which create 4 traingles and and add them 
together to create a pyramid shape without a base. 
4 sides.
Parameter:
   scene: A full 3D scene.
   a, b, c, d, e: The coordinate of 4 sides of a pyramid and height
*/
void Scene3D_add_pyramid_helper(Scene3D* scene,Coordinate3D a, 
    Coordinate3D b, Coordinate3D c, Coordinate3D d, Coordinate3D e) {
    Triangle3D triangle1 = (Triangle3D) {a, e, d};
    Triangle3D triangle2 = (Triangle3D) {a, e, b};
    Triangle3D triangle3 = (Triangle3D) {b, e, c};
    Triangle3D triangle4 = (Triangle3D) {c, e, d};
    Scene3D_add_triangle(scene, triangle1);
    Scene3D_add_triangle(scene, triangle2);
    Scene3D_add_triangle(scene, triangle3);
    Scene3D_add_triangle(scene, triangle4);
}


/*
The function should add a pyramid in 3D space to the passed-in scene. 
The parameters specify the scene to add the pyramid to, and where the 
pyramid should exist in 3D space. The origin represents the center of 
the base of the pyramid. The width represents the width / length of 
each side of the base of the pyramid. The height represents how tall 
the point of the pyramid will be (how far away the point will be from 
the origin). The orientation represents the direction that the top of 
the pyramid will point towards.
Parameter:
    scene: A full 3D scene.
    origin: coordinate of the middle of the pyramid
    width: width of the base of the pyramid 
    height: The height of the cuboid 
    oreintation: direction of the pyramid 
*/
void Scene3D_add_pyramid(Scene3D* scene,Coordinate3D origin,double width, double height, char* orientation) {
    Coordinate3D a, b, c, d, e;

    if (strcmp(orientation, "right") == 0) {
        //creating the base coordinate on the right side
        a = (Coordinate3D) {origin.x, (origin.y + (width/2)), (origin.z + (width/2))};
        b = (Coordinate3D) {origin.x, (origin.y + (width/2)), (origin.z - (width/2))};
        c = (Coordinate3D) {origin.x, (origin.y - (width/2)), (origin.z - (width/2))};
        d = (Coordinate3D) {origin.x, (origin.y - (width/2)), (origin.z + (width/2))};
        e = (Coordinate3D) {(origin.x + height), origin.y, origin.z};
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }

    if (strcmp(orientation, "left") == 0) {
        //creating the base coordinate on the left side
        a = (Coordinate3D) {origin.x, (origin.y + (width/2)), (origin.z + (width/2))};
        b = (Coordinate3D) {origin.x, (origin.y + (width/2)), (origin.z - (width/2))};
        c = (Coordinate3D) {origin.x, (origin.y - (width/2)), (origin.z - (width/2))};
        d = (Coordinate3D) {origin.x, (origin.y - (width/2)), (origin.z + (width/2))};
        e = (Coordinate3D) {(origin.x - height), origin.y, origin.z};
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }

    if (strcmp(orientation, "forward") == 0) {
        //creating the base coordinate on the forward side
        a = (Coordinate3D) {(origin.x + (width/2)), origin.y, (origin.z + (width/2))};
        b = (Coordinate3D) {(origin.x + (width/2)), origin.y, (origin.z - (width/2))};
        c = (Coordinate3D) {(origin.x - (width/2)), origin.y, (origin.z - (width/2))};
        d = (Coordinate3D) {(origin.x - (width/2)), origin.y, (origin.z + (width/2))};
        e = (Coordinate3D) {origin.x, (origin.y + height), origin.z};
        //creating the base
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        //creating the side of the pyramid
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }

    if (strcmp(orientation, "backward") == 0) {
        //creating the base coordinate on the background side
        a = (Coordinate3D) {(origin.x + (width/2)), origin.y, (origin.z + (width/2))};
        b = (Coordinate3D) {(origin.x + (width/2)), origin.y, (origin.z - (width/2))};
        c = (Coordinate3D) {(origin.x - (width/2)), origin.y, (origin.z - (width/2))};
        d = (Coordinate3D) {(origin.x - (width/2)), origin.y, (origin.z + (width/2))};
        e = (Coordinate3D) {origin.x, (origin.y - height), origin.z};
        //creating the base
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        //creating the side of the pyramid
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }

    if (strcmp(orientation, "up") == 0) {
        //creating the base coordinate on the up side
        a = (Coordinate3D) {(origin.x + (width/2)), (origin.y + (width/2)), origin.z};
        b = (Coordinate3D) {(origin.x + (width/2)), (origin.y - (width/2)), origin.z};
        c = (Coordinate3D) {(origin.x - (width/2)), (origin.y - (width/2)), origin.z};
        d = (Coordinate3D) {(origin.x - (width/2)), (origin.y + (width/2)), origin.z};
        e = (Coordinate3D) {origin.x, origin.y, (origin.z + height)};
        //creating the base
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        //creating the side of the pyramid
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }

    if (strcmp(orientation, "down") == 0) {
        //creating the base coordinate on the down side
        a = (Coordinate3D) {(origin.x + (width/2)), (origin.y + (width/2)), origin.z};
        b = (Coordinate3D) {(origin.x + (width/2)), (origin.y - (width/2)), origin.z};
        c = (Coordinate3D) {(origin.x - (width/2)), (origin.y - (width/2)), origin.z};
        d = (Coordinate3D) {(origin.x - (width/2)), (origin.y + (width/2)), origin.z};
        e = (Coordinate3D) {origin.x, origin.y, (origin.z - height)};
        //creating the base
        Scene3D_add_quadrilateral(scene, a, b, c, d);
        //creating the side of the pyramid
        Scene3D_add_pyramid_helper(scene, a, b, c, d, e);
    }
}


/*
This function should create a new Object3D on the heap and populate it with
a bunch of triangles to represent a cuboid in 3D space.
The caller is responsible for freeing the memory, or, if this shape gets 
added to a Scene3D, that is can be freed when the scene is.
Parameters:
    scene: The scene to add to the shape to
    origin: The origin point for the cuboid (center)
    width: The width of the cuboid (x)
    height: The height of the cuboid (y)
    depth: The depth of the cuboid (z)
*/
void Scene3D_add_cuboid(Scene3D* scene,Coordinate3D origin, 
    double width, double height, double depth) {
    Coordinate3D a,b,c,d,e,f,g,h;
    double wid = width / 2;
    double hei = height / 2;
    double dep = depth / 2;
    //creating the coordinate of each side of the triangle 
    a = (Coordinate3D) {(origin.x + wid), (origin.y + hei), (origin.z - dep)};
    b = (Coordinate3D) {(origin.x + wid), (origin.y - hei), (origin.z - dep)};
    c = (Coordinate3D) {(origin.x - wid), (origin.y - hei), (origin.z - dep)};
    d = (Coordinate3D) {(origin.x - wid), (origin.y + hei), (origin.z - dep)};
    e = (Coordinate3D) {(origin.x + wid), (origin.y + hei), (origin.z + dep)};
    f = (Coordinate3D) {(origin.x + wid), (origin.y - hei), (origin.z + dep)};
    g = (Coordinate3D) {(origin.x - wid), (origin.y - hei), (origin.z + dep)};
    h = (Coordinate3D) {(origin.x - wid), (origin.y + hei), (origin.z + dep)};
    //creating every side of cuboid 
    Scene3D_add_quadrilateral(scene, a, b, c, d);
    Scene3D_add_quadrilateral(scene, a, b, f, e);
    Scene3D_add_quadrilateral(scene, b, c, g, f);
    Scene3D_add_quadrilateral(scene, c, d, h, g);
    Scene3D_add_quadrilateral(scene, d, a, e, h);
    Scene3D_add_quadrilateral(scene, e, f, g, h);
}


/**
Write every shape from the Scene3D to the file with file_name using the STL
binary format. The function is responsible for opening, writing to, and 
closing the file.
    Parameters:
        scene: The scene to write to the file
        file_name: The name of the file to write the STL data to
 */
void Scene3D_write_stl_binary(Scene3D* scene, char* file_name) {
    FILE* bin_file = fopen(file_name, "wb");
    if (bin_file == NULL) {
        exit(1);
    }
    Triangle3DNode* node = scene->root;
    uint32_t header = 0.0;
    uint32_t facet = (uint32_t) scene->count;
    for (int i = 0; i < 20; i ++) {
        fwrite(&header, sizeof(header), 1, bin_file);
    }
    fwrite(&facet,sizeof(facet), 1, bin_file);
    uint16_t end = 0.0;
    while (node != NULL) {
        float normal = 0.0;
        fwrite(&normal, sizeof(normal), 1, bin_file);
        fwrite(&normal, sizeof(normal), 1, bin_file);
        fwrite(&normal, sizeof(normal), 1, bin_file);
        float vertex_ax = (float) node->triangle.a.x;
        float vertex_ay = (float) node->triangle.a.y;
        float vertex_az = (float) node->triangle.a.z;
        float vertex_bx = (float) node->triangle.b.x;
        float vertex_by = (float) node->triangle.b.y;
        float vertex_bz = (float) node->triangle.b.z;
        float vertex_cx = (float) node->triangle.c.x;
        float vertex_cy = (float) node->triangle.c.y;
        float vertex_cz = (float) node->triangle.c.z;
        fwrite(&vertex_ax, sizeof(vertex_ax), 1, bin_file);
        fwrite(&vertex_ay, sizeof(vertex_ay), 1, bin_file);
        fwrite(&vertex_az, sizeof(vertex_az), 1, bin_file);
        fwrite(&vertex_bx, sizeof(vertex_bx), 1, bin_file);
        fwrite(&vertex_by, sizeof(vertex_by), 1, bin_file);
        fwrite(&vertex_bz, sizeof(vertex_bz), 1, bin_file);
        fwrite(&vertex_cx, sizeof(vertex_cx), 1, bin_file);
        fwrite(&vertex_cy, sizeof(vertex_cy), 1, bin_file);
        fwrite(&vertex_cz, sizeof(vertex_cz), 1, bin_file);
        fwrite(&end, sizeof(end), 1, bin_file);
        node = node->next;
    }
    fclose(bin_file);
}


/**
The function converts the four spherical coordinates to cartesian 
coordinates (x, y, z):
(radius, theta, phi)
(radius, theta, phi - increment)
(radius, theta - increment, phi) 
(radius, theta - increment, phi - increment)
Parameter:
    origin: The origin point for the sphere (center)
    radius: The desired radius of the sphere
    phi: The variable of spherical coordinates
    theta: The variable of spherical coordinates
Returns:
    The function return cartesian coordinates
 */
Coordinate3D convert_coordinates(double radius, double theta, double phi, Coordinate3D origin) {
    Coordinate3D cart;
    double x, y, z;
    x = roundf((radius * sin(phi * (PI/180)) * cos(theta* (PI/180))*10000)/10000);
    y = roundf((radius * sin(phi * (PI/180)) * sin(theta* (PI/180))*10000)/10000);
    z = roundf((radius * cos(phi* (PI/180)))*10000)/10000;
    cart = (Coordinate3D) {(x + origin.x), (y + origin.y), (z + origin.z)};
    return cart;
}


/**
This function should create a new Object3D on the heap and populate it with
a bunch of triangles to represent a sphere in 3D space.
The caller is responsible for freeing the memory, or, if this shape gets 
added to a Scene3D, that is can be freed when the scene is.
Parameters:
    rigin: The origin point for the sphere (center)
    radius: The desired radius of the sphere
    increment: A value in the range (180, 0) that determines 
        the smoothness of the sphere.                
*/
void Scene3D_add_sphere(Scene3D* scene,Coordinate3D origin,double radius, double increment) {
    Coordinate3D a,b,c,d;
    for (double phi = increment; phi <= 180.0; phi = phi + increment) {
        for (double theta = 0.0; theta < 360.0; theta = theta + increment) {
            a = convert_coordinates(radius, theta , phi, origin);
            b = convert_coordinates(radius, theta, phi-increment, origin);
            c = convert_coordinates(radius, theta-increment, phi, origin);
            d = convert_coordinates(radius, theta-increment, phi-increment, origin);
            Scene3D_add_quadrilateral(scene, a, b, c, d);
        }
    }
}


/**
This function should create a new Object3D on the heap and populate it with
a bunch of triangles to represent a cube-based fractal in 3D space.
The caller is responsible for freeing the memory, or, if this shape gets 
added to a Scene3D, that is can be freed when the scene is.
Parameters:
    origin: The origin point for the fractal (center)
    size: Used for the width, height, and depth of the center cube
    levels: The number of levels to recurse to when building the fractal
 */
void Scene3D_add_fractal(Scene3D* scene, Coordinate3D origin, double size, int levels) {
    if (levels == 0) {
        return;
    } else if (levels == 1) {
       Scene3D_add_cuboid(scene, origin, size, size, size);
    } else {
        Scene3D_add_cuboid(scene, origin, size, size, size);
        double offset = 0.5 * size;
        Coordinate3D origin1 = (Coordinate3D) {(origin.x + offset), origin.y, origin.z};
        Coordinate3D origin2 = (Coordinate3D) {(origin.x - offset), origin.y, origin.z};
        Coordinate3D origin3 = (Coordinate3D) {origin.x, (origin.y + offset), origin.z};
        Coordinate3D origin4 = (Coordinate3D) {origin.x, (origin.y - offset), origin.z};
        Coordinate3D origin5 = (Coordinate3D) {origin.x, origin.y, (origin.z + offset)};
        Coordinate3D origin6 = (Coordinate3D) {origin.x, origin.y, (origin.z - offset)};
        
        Scene3D_add_fractal(scene, origin1, offset, levels - 1);
        Scene3D_add_fractal(scene, origin2, offset, levels - 1);
        Scene3D_add_fractal(scene, origin3, offset, levels - 1);
        Scene3D_add_fractal(scene, origin4, offset, levels - 1);
        Scene3D_add_fractal(scene, origin5, offset, levels - 1);
        Scene3D_add_fractal(scene, origin6, offset, levels - 1);
    }
}