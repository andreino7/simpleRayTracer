//
//  polygons.hpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#ifndef polygons_hpp
#define polygons_hpp
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include "matrixVectorLibrary.hpp"
#include <Math.h>


/**
 Abstract class for the polygon
 */
class Polygon {
public:
    //function to compute the parameter t at the intersections
    virtual std::vector<double> computeT(GeometricVector,GeometricVector) = 0;
    //function to compute the normal at the intersections
    virtual GeometricVector computeNormal(GeometricVector) = 0;
    //function to color the pixel i j
    virtual void colorPixel(int,int,GeometricVector);
    //return the reflection exponent
    int exp();
    //getter for ka
    GeometricVector ka();
    //getter for ks
    GeometricVector ks();
    //getter for kt
    GeometricVector kt();
    //getter for kd
    GeometricVector kd();
    //getter for the material diffuse color
    GeometricVector materialDiffuseColor();
    //getter for the material diffuse color in a specific point
    virtual GeometricVector materialDiffuseColor(GeometricVector) = 0;
    bool isRefractive();
protected:
    GeometricVector* materialDiffuseColor_;
    GeometricVector* ka_;
    GeometricVector* ks_;
    GeometricVector* kt_;
    GeometricVector* kd_;
    int exp_;
    bool isRefractive_;
    //constructor that creates a polygon given the parameters ka,ks,kt,kd,the material diffuse color, the exponent and the it
    //the object is refractive
    Polygon(GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,double,bool);
};


/**
 Class rectangle, it inherits from polygon. It represents the wall.
 */
class Rectangle : public Polygon {
protected:
    //parameter to check if the normal shoul be multiplied by -1
    bool reverse;
    //vertices
    GeometricVector* v1_;
    GeometricVector* v2_;
    GeometricVector* v3_;
    GeometricVector* v4_;
    //function to compute the normal in the point passed as parameter
    virtual GeometricVector computeNormal(GeometricVector);

public:
    //constructor: it basically does the same thing of the polygon constructor, but it has one additional parameter: if the normal must be
    //inverted
    Rectangle(GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,double,bool,bool reverse = false);
    //function that implements the computation of the intersection
    virtual std::vector<double> computeT(GeometricVector,GeometricVector);
    //function that returns the material diffuse color in a specific point, it calls the base class function that returns the
    //the material diffuse color of the object
    virtual GeometricVector materialDiffuseColor(GeometricVector);
};

/**
 Class which represent the floor of the room
 */
class Floor : public Rectangle {
public:
    //constructor: it only calls the base class constructor
    Floor(GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,double,bool,bool reverse = false);
    //function that returns the material diffuse color in a specific point, useful to create the pattern of the floor
    virtual GeometricVector materialDiffuseColor(GeometricVector);
protected:
    //nested class floor tile, which represents a tile on the floor
    class FloorTile {
    public:
        //constructor: takes as parameter the xmin zmin height and width value the tile and its color
        FloorTile(double,double,double,double,GeometricVector);
        double xMinCoordinate_;
        double zMinCoordinate_;
        double width_;
        double height_;
        GeometricVector color_;
    };
    //list of the ties which compose the floor
    std::vector<FloorTile*> tiles;

};

/**
 Class that represents a sphere
 */
class Sphere : public Polygon {
protected:
    double radius_;
    GeometricVector* center_;
public:
    //constructor that takes as parameter the x,y,z coordinate of the sphere, the radius and all the parameter necessary to ligth the sphere,
    //such as ka, ks, kt etc.
    Sphere(double,double,double,double,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,GeometricVector*,int,bool);
    virtual std::vector<double> computeT(GeometricVector,GeometricVector);
    //function that implements the computation of the intersection
    virtual GeometricVector computeNormal(GeometricVector);
    //function that returns the material diffuse color in a specific point, it calls the base class function that returns the
    //the material diffuse color of the object
    virtual GeometricVector materialDiffuseColor(GeometricVector);
};

#endif /* polygons_hpp */
