//
//  main.cpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include "matrixVectorLibrary.hpp"
#include "database.hpp"
#include "raytracing.hpp"


Database* db;
RayTracer* rayTracer;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    db->window()->drawPixels();
    glutSwapBuffers();
}

void reshape (int w, int h) {
    /* set the viewport */
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);  // lower-left corner
    
    /* Matrix for projection transformation */
    glMatrixMode (GL_PROJECTION);
    
    /* replaces the current matrix with the identity matrix */
    glLoadIdentity ();
    
    /* Define a 2d orthographic projection matrix */
    gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);  // left, right, bottom, top
}


void initDatabase() {
        GeometricVector *vrp = new GeometricVector(0,-20,-10);
        GeometricVector *prp = new GeometricVector(0,20,10);
        GeometricVector *vup = new GeometricVector(0,1,0);
        GeometricVector *vpn = new GeometricVector(0,0,1);
        GeometricVector *Ia = new GeometricVector(1,1,1);
        GeometricVector *Is = new GeometricVector(1,1,1);
        GeometricVector *light = new GeometricVector(0,40,-5);
    
    
        db = new Database(vrp,prp,vpn,vup,-40, -40, 40, 40,Ia,Is,light);
        rayTracer = new RayTracer(700,*prp,db);
        GeometricVector* sphereColor = new GeometricVector(1,1,1);
        GeometricVector* ka = new GeometricVector(0.231,0.231,0.231);
        GeometricVector* ks = new GeometricVector(0.774,0.774,0.774);
        GeometricVector* kt = new GeometricVector(1,1,1);
        GeometricVector* ktwall = new GeometricVector(1,1,1);
        GeometricVector* kd = new GeometricVector(0.278,0.278,0.278);
    
        GeometricVector* ka2 = new GeometricVector(0.35,0.06,0.02);
        GeometricVector* ks2 = new GeometricVector(0.1,0.1,0.1);
        GeometricVector* kd2 = new GeometricVector(0.82,0.2,0.1);
    
        GeometricVector* kd3 = new GeometricVector(0.521,0.72,0.2);
    
        GeometricVector* kd4 = new GeometricVector(0.1,0.2,0.8);
    
        GeometricVector* ka3 = new GeometricVector(0.854902,0.647059,0.12549);
    
    
        int exp = 89.6;
        int exp2 = 5;
    
        GeometricVector* wallColor = new GeometricVector(1,0,0);
        GeometricVector* wallColor2 = new GeometricVector(0,1,0);
        GeometricVector* wallColor3 = new GeometricVector(0,0,1);
    
    
        Sphere *s = new Sphere(-15,0,-40,20,sphereColor,ka,ks,ktwall,kd,exp,true);
        Sphere *s1 = new Sphere(25,0,-30,10,sphereColor,ka,ks,ktwall,kd,exp,true);
        GeometricVector *v1 = new GeometricVector(-40,-20,-10);
        GeometricVector *v2 = new GeometricVector(-40,30,-10);
        GeometricVector *v3 = new GeometricVector(-40,30,-80);
        GeometricVector *v4 = new GeometricVector(-40,-20,-80);
    
        GeometricVector *v5 = new GeometricVector(40,-20,-10);
        GeometricVector *v6 = new GeometricVector(40,30,-10);
        GeometricVector *v7 = new GeometricVector(40,30,-80);
        GeometricVector *v8 = new GeometricVector(40,-20,-80);
    
        GeometricVector *v9 = new GeometricVector(-40,-20,-80);
        GeometricVector *v10 = new GeometricVector(-40,30,-80);
        GeometricVector *v11= new GeometricVector(40,30,-80);
        GeometricVector *v12 = new GeometricVector(40,-20,-80);
    
        GeometricVector *v13 = new GeometricVector(-200,-70,-10);
        GeometricVector *v14 = new GeometricVector(200,-70,-10);
        GeometricVector *v15= new GeometricVector(200,-70,-90);
        GeometricVector *v16 = new GeometricVector(-200,-70,-90);
    
    
        Rectangle *r = new Rectangle(v1,v2,v3,v4,wallColor,ka2,ks2,kt,kd2,exp2,false);
        Rectangle *r1 = new Rectangle(v5,v6,v7,v8,wallColor2,ka2,ks2,kt,kd3,exp2,false,true);
        Rectangle *r2 = new Rectangle(v9,v10,v11,v12,wallColor3,ka2,ks2,kt,kd4,exp2,false);
        Floor *r3 = new Floor(v13,v14,v15,v16,wallColor,ka3,ks2,kt,kd4,exp2,false);
        db->addPolygon(s)->addPolygon(s1)->addPolygon(r1)->addPolygon(r2)->addPolygon(r)->addPolygon(r3);
   // db->addPolygon(r1)->addPolygon(r3);
}

/*******************************************************************/

int main(int argc, char** argv) {
    initDatabase();
    rayTracer->execute();
    /* deal with any GLUT command Line options */
    glutInit(&argc, argv);
    /* create an output window */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(700, 700);
    
    /* set the name of the window and try to create it */
    glutCreateWindow("CS 425 - HOMEWORK 01");
    
    /* specify clear values for the color buffers */
    glClearColor (1.0, 1.0, 1.0, 1.0);
    
    /* Receive keyboard inputs */
//    glutKeyboardFunc (Keyboard);
    
    /* assign the display function */
    glutDisplayFunc(display);
    
    /* assign the idle function */
    glutIdleFunc(display);
    
    /* sets the reshape callback for the current window */
    glutReshapeFunc(reshape);
    
    /* enters the GLUT event processing loop */
    glutMainLoop();
    
    
    
    
    return (EXIT_SUCCESS);
}

