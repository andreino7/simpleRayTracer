//
//  scene.cpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#include "database.hpp"



Database::Database(GeometricVector* vrp,GeometricVector* prp,GeometricVector* vpn,GeometricVector* vup,int windowXmin, int windowYmin, int windowXmax, int windowYmax, GeometricVector* Ia, GeometricVector* Is,GeometricVector* lightPosition ) {
    vrp_ = vrp;
    prp_ = &((*prp) + (*vrp));
    vpn_ = vpn;
    vup_ = vup;
    Ia_ = Ia;
    Is_ = Is;
    lightPosition_ = lightPosition;
    int height = windowYmax-windowYmin;
    int width = windowXmax - windowXmin;
    int xmin = (*vrp_)[0]-windowXmax;
    int ymin = (*vrp_)[1]-windowYmax;
    //initialize a new window
    window_ = new Window(xmin, ymin,height,width,(*vrp_)[2],700);
    polygons_ = *new std::vector<Polygon*>();
}

void Pixel::color(GeometricVector color) {
    color_ = color;
}

void Pixel::plot() {
    glColor3f(color_[0], color_[1], color_[2]);
    glBegin(GL_POINTS);
    glVertex2f(screenXcoordinate_, screenYcoordinate_);
    glEnd();
}

Database* Database::addPolygon(Polygon *p) {
    polygons_.push_back(p);
    return this;
}

std::vector<Polygon*> Database::polygons() {
    return polygons_;
}

GeometricVector* Database::prp() {
    return prp_;
}

GeometricVector Database::getPixelInWC(int row, int column) {
    return window_->getPixel(row, column)->pixelWoorldCoordinates();
}

GeometricVector Database::lightPosition() {
    return *lightPosition_;
}

GeometricVector Database::Ia() {
    return *Ia_;
}

GeometricVector Database::Is() {
    return *Is_;
}

void Window::drawPixels() {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid.size(); j++) {
            grid[i][j]->plot();
        }
    }
}

Window::Window(int xmin, int ymin, int height, int width, int zPosition, int numberOfPixels) : zPosition_(zPosition) {
    grid = *new std::vector<std::vector<Pixel*>>(700);
    for (int i = 0; i < 700; i++) {
        grid[i] = *new std::vector<Pixel*>(700);
    }
    //determine pixel height and width
    double pixelHeight = double(height) / numberOfPixels;
    double pixelWidth = double(width) / numberOfPixels;
    
    for (int i = 0; i < numberOfPixels; i++) {
        for (int j = 0; j < numberOfPixels; j++) {
            //create a new pixel in the i, j position, with the selected height and width
            grid[i][j] = new Pixel(i,j,pixelHeight, pixelWidth, zPosition, xmin, ymin);
        }
    }
}

GeometricVector Pixel::pixelWoorldCoordinates() {
    return GeometricVector(xcoordinate_, ycoordinate_, zcoordinate_);
}

Pixel::Pixel(int row, int column, double pixelHeight, double pixelWidth, double z, int xmin, int ymin) : xcoordinate_(column*pixelWidth+xmin), ycoordinate_(row*pixelHeight+ymin), screenXcoordinate_(column), screenYcoordinate_(row), zcoordinate_(z), color_(3) {}


Pixel* Window::getPixel(int row, int column) {
    return grid[row][column];
}

Window* Database::window() {
    return window_;
}