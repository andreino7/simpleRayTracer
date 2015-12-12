//
//  scene.hpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <vector>
#include "matrixVectorLibrary.hpp"
#include "polygons.hpp"



class Pixel {
protected:
    double xcoordinate_;
    double ycoordinate_;
    double zcoordinate_;
    int screenXcoordinate_;
    int screenYcoordinate_;
    //final color of the pixel
    GeometricVector color_;
public:
    //constructor: it initializes the coordiantes of the pixel
    Pixel(int,int,double,double,double,int,int);
    //function to get the woorld coordinate of the pixel
    GeometricVector pixelWoorldCoordinates();
    //function to set the color of the pixel
    void color(GeometricVector color);
    //function to plot the pixel
    void plot();
};

class Window {
protected:
    //grid of pixels which compose the window
    std::vector<std::vector<Pixel *>> grid;
    int height_;
    int width_;
    int zPosition_;
public:
    //constructor of the window: it creates a window of the desired dimension
    Window(int, int, int, int, int, int);
    //function to get the pixel in position i, j
    Pixel* getPixel(int, int);
    //function to draw the pixel on the screen
    void drawPixels();
};



class Database {
protected:
    //all the parameters necessary to compute the image
    GeometricVector* vrp_;
    GeometricVector* prp_;
    GeometricVector* vpn_;
    GeometricVector* vup_;
    GeometricVector* Ia_;
    GeometricVector* Is_;
    GeometricVector* lightPosition_;
    //window on the view plane
    Window *window_;
    
    //list of the polygon
    std::vector<Polygon*> polygons_;
public:
    //constructor which initialize all the parameters such a vrp, prp etc.. It creates also a new window
    Database(GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,int,int,int,int, GeometricVector*,GeometricVector*,GeometricVector*);
    //return the pixel coordinate in the world coordinate system
    GeometricVector getPixelInWC(int,int);
    //return the prp
    GeometricVector* prp();
    //return Ia
    GeometricVector Ia();
    //return Is
    GeometricVector Is();
    //return a pointer to the window
    Window* window();
    //return the light position
    GeometricVector lightPosition();
    //return all the polygons in the database
    std::vector<Polygon*> polygons();
    //function to add a polygon to the database
    Database* addPolygon(Polygon* p);
};

#endif /* scene_hpp */
