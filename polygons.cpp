//
//  polygons.cpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#include "polygons.hpp"


GeometricVector Rectangle::materialDiffuseColor(GeometricVector intersection) {
    return Polygon::materialDiffuseColor();
}

GeometricVector Sphere::materialDiffuseColor(GeometricVector intersection) {
    return Polygon::materialDiffuseColor();
}

Polygon::Polygon(GeometricVector* materialDiffuseColor, GeometricVector* ka, GeometricVector* ks, GeometricVector* kt,GeometricVector* kd,double exp, bool isRefractive) :exp_(exp){
    materialDiffuseColor_ = materialDiffuseColor;
    ka_ = ka;
    ks_ = ks;
    kt_ = kt;
    kd_ = kd;
    isRefractive_ = isRefractive;
}

int Polygon::exp() {
    return exp_;
}

Sphere::Sphere(double xcenter, double ycenter, double zcenter, double radius, GeometricVector* materialDiffuseColor, GeometricVector* ka, GeometricVector* ks, GeometricVector* kt,GeometricVector* kd, int exp, bool isRefractive) : Polygon(materialDiffuseColor,ka,ks,kt,kd,exp,isRefractive), radius_(radius) {
    center_ = new GeometricVector(xcenter,ycenter,zcenter);
}

void Polygon::colorPixel(int row, int column, GeometricVector color) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POINTS);
    glVertex2f(column, row);
    glEnd();
}


std::vector<double> Sphere::computeT(GeometricVector points, GeometricVector direction) {
    //parameters of the equation quadratic equation
    double bHalf = 0;
    double a = 0;
    double c = 0;
    
    //compute the parameter of the quadratic equation using a little bit of geometry
    for (int i = 0; i < center_->size(); i++) {
        bHalf += direction[i]*(points[i]-(*center_)[i]);
        a += direction[i]*direction[i];
        c += (points[i]-(*center_)[i])*(points[i]-(*center_)[i]);
    }
    c = c - radius_*radius_;
    
    //compute the delta of the equation
    double delta = bHalf*bHalf-a*c;
    
    //list that will contain the solutions of the equation
    std::vector<double> solutions;
    //if delta greater than 0 there are intersections
    if (delta > 0) {
        //compute the solutions
        double t1 = (bHalf+sqrt(delta))/a;
        double t2 = (bHalf+sqrt(delta))/a;
        solutions.push_back(t1);
        solutions.push_back(t2);
    }
    return solutions;
}

bool Polygon::isRefractive() {
    return  isRefractive_;
}

GeometricVector Sphere::computeNormal(GeometricVector intersection) {
    GeometricVector normal = GeometricVector(3);
    for (int i = 0; i < normal.size(); i++) {
        //use the formula to compute the normal of a sphere in a specific point
        normal[i] = (intersection[i]-(*center_)[i])/radius_;
    }
    return normal;
}

GeometricVector Rectangle::computeNormal(GeometricVector intersection) {
    //compute the normal of the rectangle in the intersection point. However the normal doens't depend
    //on the intersection point
    
    //find the two vectors
    GeometricVector vector1 = (*v2_)-(*v1_);
    GeometricVector vector2 = (*v3_)-(*v1_);
    //check if they should be changed
    if (reverse) {
        GeometricVector tmp = vector1;
        vector1 = vector2;
        vector2 = tmp;
    }
    //do vectorial product to compute the normal
    GeometricVector n = *vector2.vecorialProduct(&vector1);
    return n;
}

Rectangle::Rectangle(GeometricVector* v1, GeometricVector* v2, GeometricVector* v3, GeometricVector* v4, GeometricVector* materialDiffuseColor, GeometricVector* ka, GeometricVector* ks, GeometricVector* kt,GeometricVector* kd,double exp,bool isRefractive,bool reverse) : Polygon(materialDiffuseColor,ka,ks,kt,kd,exp,isRefractive), v1_(v1), v2_(v2), v3_(v3), v4_(v4), reverse(reverse) {}

std::vector<double> Rectangle::computeT(GeometricVector points, GeometricVector direction) {
    //compute the normal to the plane corresponding to the polygon
    GeometricVector vector1 = (*v2_)-(*v1_);
    GeometricVector vector2 = (*v3_)-(*v1_);
    GeometricVector n = *vector2.vecorialProduct(&vector1);
    //compute the D parameter of the plain equation
    double D = -((*v1_)*n);
    
    double numerator = 0;
    double denominator = 0;
    
    //compute numerator and denominator of the solution
    for (int i = 0; i < direction.size(); i++) {
        numerator += n[i]*points[i];
        denominator += n[i]*direction[i];
    }
    numerator += D;
    
    std::vector<double> solutions;
    //check if denominator is different from zero, i.e. if the denominator different from zero
    //the direction of the ray is not paralle to the plane -> ray and plane intersect each other
    if (denominator != 0) {
        //compute t
        double t = -(numerator/denominator);
        
        //some simple algebra to check if the intersection point is inside the rectangle
        //and not only on the plane of the rectangle
        GeometricVector intersection = points+direction*t;
        GeometricVector vector3 = (*v4_)-(*v3_);
        GeometricVector vector4 = intersection-(*v1_);
        GeometricVector vector5 = intersection-(*v3_);
        vector1.normalizeVector();
        vector3.normalizeVector();
        vector4.normalizeVector();
        vector5.normalizeVector();

        if (vector1*vector4 >= 0 && vector3*vector5 >= 0 && t >= 1) {
            solutions.push_back(t);
        }
    }
    return solutions;
}

GeometricVector Polygon::ka() {
    return *ka_;
}

GeometricVector Polygon::ks() {
    return *ks_;
}

GeometricVector Polygon::kt() {
    return *kt_;
}

GeometricVector Polygon::kd() {
    return *kd_;
}

GeometricVector Polygon::materialDiffuseColor() {
    return *materialDiffuseColor_;
}

Floor::FloorTile::FloorTile(double xmin, double zmin, double width, double height, GeometricVector color) : xMinCoordinate_(xmin), zMinCoordinate_(zmin), width_(width), height_(height), color_(color[0],color[1],color[2]) {}

Floor::Floor(GeometricVector* v1, GeometricVector* v2, GeometricVector* v3, GeometricVector* v4, GeometricVector* materialDiffuseColor, GeometricVector* ka, GeometricVector* ks, GeometricVector* kt,GeometricVector* kd,double exp,bool isRefractive,bool reverse) : Rectangle(v1, v2, v3, v4, materialDiffuseColor, ka, ks, kt, kd,exp,isRefractive,reverse)
 {
     GeometricVector wallColor = GeometricVector(3);
         for (int i = 0; i < 200; i++) {
             for (int j = 0; j < 80; j++) {
                 double xmin = -200+j*5;
                 double zmin = -300+i*3;
                 double width = 5;
                 double height = 3;
                 if ((i+j)%2==0) {
                     wallColor = GeometricVector(0.0,0.0,0.0);
                 } else {
                     wallColor = GeometricVector(1.0,1.0,1.0);
                 }
                 tiles.push_back(new FloorTile(xmin,zmin,width,height,wallColor));
             }
         }
}

GeometricVector Floor::materialDiffuseColor(GeometricVector intersection) {
    for (int i = 0; i < tiles.size(); i++) {
        if (intersection[0] >= tiles[i]->xMinCoordinate_ && intersection[0] < tiles[i]->xMinCoordinate_+tiles[i]->width_ && intersection[2] >= tiles[i]->zMinCoordinate_ && intersection[2] < tiles[i]->zMinCoordinate_+tiles[i]->height_ ) {
            return tiles[i]->color_;
        }
    }
    return GeometricVector(0.72,0.72,0.72);
}
