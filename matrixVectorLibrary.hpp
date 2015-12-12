//
//  matrixVectorLibrary.hpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#ifndef matrixVectorLibrary_hpp
#define matrixVectorLibrary_hpp

#include <stdio.h>
#include <vector>
#include <Math.h>

/**
 Class which represents vectors
 */
class GeometricVector {
protected:
    //elements of the vector
    double *elements_;
    //size of the vector
    int size_;
public:
    //return the vector size
    int size();
    //compute the norm of the vector
    double computeNorm();
    //constructor that initialize all the element to 0
    GeometricVector(int size);
    //constructor that creates a 3d vector
    GeometricVector(double,double,double);
    //overloading of [] operator
    double& operator[](const int);
    //overloading of + operator
    GeometricVector& operator+(GeometricVector& other);
    //overloading of - operator
    GeometricVector& operator-(GeometricVector& other);
    //overloading of * operator
    GeometricVector& operator*(double other);
    //overloading of * operator
    double operator*(GeometricVector& other);
    //function to compute the vectorial product
    GeometricVector* vecorialProduct(GeometricVector* other);
    //function that return a normalized copy of the vector
    GeometricVector normalizeVector();
    
};


/**
 Class which represents matrices
 */
class Matrix {
protected:
    std::vector<GeometricVector*> rows;
    int nrows_;
    int ncolumns_;
public:
    Matrix(int, int);
    GeometricVector& operator[](const int);
    GeometricVector& operator+(GeometricVector& other);
    GeometricVector& operator-(GeometricVector& other);
    GeometricVector& operator*(double other);
    GeometricVector& operator*(GeometricVector& other);
    Matrix& operator*(Matrix& other);
};

#endif /* matrixVectorLibrary_hpp */
