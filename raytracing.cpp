//
//  raytracing.cpp
//  rayTracing
//
//  Created by Andrea Rottigni on 04/12/15.
//  Copyright © 2015 Andrea Rottigni. All rights reserved.
//

#include "raytracing.hpp"

RayTracer::RayTracer(int pixels, GeometricVector prp, Database* b) : prp_(3), numberOfPixels(pixels), maxDepth(5) {
    prp_ = *b->prp();
    db = b;
}

void RayTracer::execute() {
    //retrieve all the polygons in the database
    std::vector<Polygon*> polygons = db->polygons();
    //for each pixel
    for (int i = 0; i < numberOfPixels; i++) {
        for (int j = 0; j < numberOfPixels; j++) {
            //compute the direction of the ray from the pixel to prp
            GeometricVector rayDirection = db->getPixelInWC(i, j) - *db->prp();
            //call function to shade the pixels and compute refraction and reflection
            GeometricVector color = trace(prp_,rayDirection, 1, i, j);
            //set the color of pixel
            db->window()->getPixel(i, j)->color(color);
            currentPolygon = NULL;
        }
    }
}

GeometricVector RayTracer::trace(GeometricVector point, GeometricVector rayDirection, int depth, int rows, int column) {
    //set the minimum t to the maximum possible number
    double t = std::numeric_limits<double>::max();
    Polygon* nearest = NULL;
    //iterate on every polygon in the database
    for (int k = 0; k < db->polygons().size(); k++) {
        //compute the t parameters for the intersection of the ray with the current polygon of the cycle
        std::vector<double> tmp = db->polygons()[k]->computeT(point,rayDirection);
        //for each solution found (i.e. intersection point)
        for (int f = 0; f < tmp.size(); f++) {
            //check if it is the minimum distant intersection
            if (tmp[f] < t && db->polygons()[k]!=currentPolygon) {
                t = tmp[f];
                nearest = db->polygons()[k];
            }
        }
    }
    //if an intersecting polygon was found
    if (nearest) {
        //set the current polygon to the nearest one
        currentPolygon = nearest;
        GeometricVector oldIntersection = point;
        GeometricVector intersection = point + rayDirection*t;
        //compute the distance from the current intersection and the point from which the ray start
        double distance = (intersection-oldIntersection).computeNorm();
        //get the normal in the intersection point
        GeometricVector normal = nearest->computeNormal(intersection);
        //shade the intersection point using a lambertian reflection
        return shade(intersection, nearest, rayDirection, normal, intersection, depth,rows,column,distance);
    } else {
        //if the ray doesn't intersect anything return the background color
        return GeometricVector(0.72,0.72,0.72);
    }
}

GeometricVector RayTracer::shade(GeometricVector point, Polygon *poly, GeometricVector ray, GeometricVector normal, GeometricVector intersection, int depth, int rows, int column, double distance) {
    
    GeometricVector normalizedRay = ray.normalizeVector();
    //color of the reflecting ray
    GeometricVector rColor = GeometricVector(3);
    //color of the transmission ray
    GeometricVector tColor = GeometricVector(3);
    //direction of the light from point of intersection
    GeometricVector lightDirection = (db->lightPosition()-intersection);
    //compute nearest intersection of the light ray
    double t = std::numeric_limits<double>::max();
    Polygon* nearest = NULL;
    for (int k = 0; k < db->polygons().size(); k++) {
        std::vector<double> tmp = db->polygons()[k]->computeT(intersection,lightDirection);
        for (int f = 0; f < tmp.size(); f++) {
            if (tmp[f] < t && db->polygons()[k]!=currentPolygon) {
                t = tmp[f];
                nearest = db->polygons()[k];
            }
        }
    }
    //if the light ray intersect a polygon the point is in shadow
    bool inShadow = nearest != NULL;
    
    GeometricVector lightDirectionNormalized = lightDirection.normalizeVector();
    GeometricVector normalizeNormal = normal.normalizeVector();
    
    //compute vprime to compute the r and p vector necessary for reflection and refraction
    GeometricVector vprime = ray * (1/(fabs(normalizedRay*normalizeNormal)));
    vprime = vprime.normalizeVector();
    //reflection ray
    GeometricVector r = vprime + normalizeNormal*2;
    r = r.normalizeVector();
    
    
    //get all the parameters necessary for the lambertian reflection
    GeometricVector ks = poly->ks();
    GeometricVector Is = db->Is();
    GeometricVector Kd = poly->kd();
    GeometricVector Ka = poly->ka();
    GeometricVector Ia = db->Ia();
    GeometricVector R = normalizeNormal*(normalizeNormal*normalizedRay)*2 - normalizedRay;
    GeometricVector polyColor = poly->materialDiffuseColor(intersection);
    GeometricVector color = GeometricVector(3);
    //attenuation factor
    double fatt = 1 / (db->lightPosition()-intersection).computeNorm(); ;
    //compute the lambertian reflection for each component of the polygon color
    for (int i = 0; i < 3; i++) {
        color[i] = polyColor[i]*(Ka[i]*Ia[i]) + (polyColor[i]*((Kd[i]*Is[i])*((normalizeNormal*lightDirectionNormalized))) + polyColor[i]*((ks[i]*Is[i])*pow(normalizedRay*R,poly->exp())))*!inShadow;
    }
    //if the current depth is less than the maximum depth
    if (depth <= maxDepth) {
        //recursive call of the trace function, to compute the contribution to the color of the reflection vector
        rColor = trace(point, r, depth+1, rows, column);
        for (int i = 0; i < 3; i++) {
            rColor[i] = (rColor[i]*fatt)*ks[i];
        }
        //check if the polygon is refractive
        if (poly->isRefractive()) {
            //compute the kf parameter using a refraction coefficient of 1.6
            double kf = pow((vprime.computeNorm()*vprime.computeNorm()*1.6*1.6-((vprime+normalizeNormal).computeNorm()*(vprime+normalizeNormal).computeNorm())),-0.5);
            //compute the refraction vector
            GeometricVector p = (normalizeNormal+vprime)*kf;
            p = p.normalizeVector()-normalizeNormal;
            //recursive call of the trace function, to compute the contribution to the color of the refraction vector
            tColor = trace(point, p, depth+1, rows, column);
            for (int i = 0; i < 3; i++) {
                tColor[i] = (tColor[i]/(distance/10))*poly->kt()[i];
            }
        }
        //update the final color
        color = color + rColor +tColor;
    } else {
        //otherwis the color is th ambient light multiplied by the polygon color
        for (int i = 0; i < 3; i++) {
            color[i] = polyColor[i]*(Ka*Ia);
        }
    }
    //return the final color
    return color;
    
}
