//
//  raytracing.hpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#ifndef raytracing_hpp
#define raytracing_hpp

#include <stdio.h>
#include "database.hpp"
#include "polygons.hpp"
#include <Math.h>

/**
    Class that implements a simple ray tracer.
 */
class RayTracer {
protected:
    //number of pixels on the screen
    int numberOfPixels;
    //maximum depth of the raytracing algorithm
    int maxDepth;
    //funtion that trace a ray from the current point to the pixel, and intersect the ray with the objects
    //It takes as parameter the point from which the ray start, the current depth of the ray tracing algorithm, the row and column
    //of the current pixel
    GeometricVector trace(GeometricVector point, GeometricVector rayDirection, int depth, int rows, int column);
    //function to shade the pixel using a lambertian illumination model.
    //It takes as parameter the point of thi intersection, the intersected polygon, the ray, the normal in the poin of intersection, the current depth of the raytracing algorithm and the distance of the object from the ray
    GeometricVector shade(GeometricVector point, Polygon *p, GeometricVector ray, GeometricVector normal, GeometricVector intersection, int depth, int rows, int column,double);
    //the database of the objects
    Database *db;
    //the center of projection
    GeometricVector prp_;
    //the current polygon intersected
    Polygon* currentPolygon;
public:
    //constructor: create a new instance of the raytracer given the center of projection and the dabase containing the objects
    RayTracer(int,GeometricVector,Database*);
    //function to execute the raytracing algorithm
    void execute();
};

#endif /* raytracing_hpp */
