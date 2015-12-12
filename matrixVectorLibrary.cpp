//
//  matrixVectorLibrary.cpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#include "matrixVectorLibrary.hpp"


GeometricVector::GeometricVector(int size) : size_(size) {
    elements_ = new double[size];
    for (int i = 0; i < size_; i++) {
        elements_[i] = 0;
    }
}

GeometricVector::GeometricVector(double el1, double el2, double el3) {
    size_ = 3;
    elements_ = new double[size_];
    elements_[0] = el1;
    elements_[1] = el2;
    elements_[2] = el3;

}

GeometricVector GeometricVector::normalizeVector() {
    double norm = computeNorm();
    GeometricVector result = GeometricVector(3);
    //Divides each element of the vector using the norm
    for (int i = 0; i < size_; i++) {
        result[i] = ((*this)[i]) / norm;
    }
    return result;
}

double GeometricVector::computeNorm() {
    double sumOfSquare = 0;
    //sums the square of each element of the vector
    for (int i = 0; i < size_; i++) {
        sumOfSquare += ((*this)[i])*((*this)[i]);
    }
    //returns the square root of the sum of the squares
    return sqrt(sumOfSquare);
}

double& GeometricVector::operator[](const int index) {
    //if the index is less than 0 or greater than size it throws an exception
    if (index < 0 || index >= size_) {
        throw "Index out of bound";
    }
    //otherwise return a reference to the element at idnex
    return elements_[index];
}

double GeometricVector::operator*(GeometricVector& other) {
    //checks if the vectors have the same size
    if (size_ != other.size_) {
        throw "nomber of columns of first matrix different from number of rows of second matrix";
    }
    double result = 0;
    //perform the scalar product of two vectors
    for (int i = 0; i < size_; i++) {
        result += (*this)[i]*other[i];
    }
    return result;
}

GeometricVector& GeometricVector::operator*(double d) {
    GeometricVector* result = new GeometricVector(size_);
    //multiply each element of the vector for d.
    for (int i = 0; i < result->size_; i++) {
        (*result)[i] = (*this)[i]*d;
    }
    return *result;
}

GeometricVector* GeometricVector::vecorialProduct(GeometricVector *other) {
    if (size_ !=3 || other->size_ !=3) {
        throw "Illegal vector";
    }
    GeometricVector* result = new GeometricVector(size_);
    //compute each component of the croos product
    (*result)[0] = (*this)[1]*(*other)[2] - (*this)[2]*(*other)[1];
    (*result)[1] = (*this)[2]*(*other)[0] - (*this)[0]*(*other)[2];
    (*result)[2] = (*this)[0]*(*other)[1] - (*this)[1]*(*other)[0];
    return result;
}

GeometricVector& GeometricVector::operator+(GeometricVector &other) {
    //checks if the vectors have the same size
    if (size_ != other.size_) {
        throw "nomber of columns of first matrix different from number of rows of second matrix";
    }
    GeometricVector* result = new GeometricVector(size_);
    //subtract each pair of corresponding elements
    for (int i = 0; i < size_; i++) {
        (*result)[i] = (*this)[i] + other[i];
    }
    return *result;
}

GeometricVector& GeometricVector::operator-(GeometricVector &other) {
    //checks if the vectors have the same size
    if (size_ != other.size_) {
        throw "nomber of columns of first matrix different from number of rows of second matrix";
    }
    GeometricVector* result = new GeometricVector(size_);
    //subtract each pair of corresponding elements
    for (int i = 0; i < size_; i++) {
        (*result)[i] = (*this)[i] - other[i];
    }
    return *result;
}

int GeometricVector::size() {
    return size_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//MATRIX


Matrix::Matrix(int nrows, int ncolumns) : nrows_(nrows), ncolumns_(ncolumns), rows(nrows) {
    for (int i = 0; i < nrows; i++) {
       rows[i] = new GeometricVector(ncolumns);
    }
}

GeometricVector& Matrix::operator[] (const int index) {
    return *rows.at(index);
}

GeometricVector& Matrix::operator*(GeometricVector &other) {
    if (ncolumns_ != other.size()) {
        throw "nomber of columns of first matrix different from number of rows of second matrix";
    }
    GeometricVector* result = new GeometricVector(ncolumns_);
    //matrix product
    for (int i = 0; i < result->size(); i++) {
        double temp = 0;
        for (int j = 0; j < result->size(); j++) {
            temp += (*this)[i][j]*other[j];
        }
        (*result)[i] = temp;
        
    }
    return *result;
}

Matrix& Matrix::operator*(Matrix &other) {
    //checks if the matrix can be multiplied
    if (ncolumns_ != other.nrows_) {
        throw "nomber of columns of first matrix different from number of rows of second matrix";
    }
    Matrix* result = new Matrix(nrows_,other.ncolumns_);
    //compute the matrix product
    for (int i = 0; i < result->nrows_; i++) {
        for (int j = 0; j < result->ncolumns_; j++) {
            double temp = 0;
            for (int k=0; k < other.nrows_; k++) {
                temp += ((*this)[i][k])*(other[k][j]);
            }
            (*result)[i][j] = temp;
        }
    }
    return *result;
}