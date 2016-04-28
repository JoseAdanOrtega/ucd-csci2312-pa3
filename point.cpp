//
// Created by Jose on 3/25/2016.
//
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include "Point.h"
#include "Exceptions.h"
#include "Cluster.h"

namespace Clustering {

    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

    void Point::rewindIdGen() {
        __idGen--;
    }

//**********************************************************************************************

    Point::Point(unsigned int dim){
        if(dim == 0)
            throw ZeroDimensionsEx();

        __dim = dim;
        __values = new double[__dim];

        for(int i = 0; i < dim; ++i)
            __values[i] = 0.0;

        __id = __idGen++;
    }

//**********************************************************************************************

    Point::Point(unsigned int dim, double * value){
        __dim = dim;
        __values = value;
        __id = __idGen++;
    }

//**********************************************************************************************

    Point::Point(const Point & P1){
        __dim = P1.__dim;
        __values = new double[__dim];

        for(int i = 0; i < __dim; ++i){
            __values[i] = P1.__values[i];
        }
        __id = P1.__id;
    }

//**********************************************************************************************

    Point& Point::operator=(const Point & other){
        if(__dim != other.__dim)
            throw DimensionalityMismatchEx(__dim,other.__dim);

        __id = other.__id;
        __dim = other.getDims();
        delete[] __values;
        __values = new double[__dim];

        for (int i = 0; i < __dim; ++i)
            __values[i] = other.getValue(i);

        return *this;
    }

//**********************************************************************************************

    Point::~Point(){
        delete []__values;
    }

//**********************************************************************************************

    int Point::getId() const{
        return __id;
    }

//**********************************************************************************************

    unsigned int Point::getDims() const{
        return __dim;
    }

//**********************************************************************************************

    void Point::setValue(unsigned int dim, double val){
        if(dim >= __dim)
            throw OutOfBoundsEx(__dim, dim);

        __values[dim] = val;
    }

//**********************************************************************************************

    double Point::getValue(unsigned int dim) const{
        if(dim >= __dim)
            throw OutOfBoundsEx(__dim,dim);

            return __values[dim];
    }

//**********************************************************************************************

    double Point::distanceTo(const Point &other) const{
        if(__dim != other.getDims())
            throw DimensionalityMismatchEx(__dim, other.getDims());

        double dist = 0.0;
        for (int i = 0; i < __dim; ++i){
            dist = dist + ((__values[i]-other.getValue(i)) * (__values[i]-other.getValue(i)));
        }

        return sqrt(dist);
    }

//**********************************************************************************************

    Point & Point::operator*=(double mult){ // p *= 6; p.operator*=(6);
        for (int i = 0; i < __dim; ++i){
            __values[i] *= mult;
        }

        return *this;
    }

//**********************************************************************************************

    Point & Point::operator/=(double div){
        for (int i = 0; i < __dim; ++i){
            __values[i] /= div;
        }

        return *this;
    }

//**********************************************************************************************

    const Point Point::operator*(double mult) const { // prevent (p1 * 2) = p2;
        Point p(*this);
        p *= mult;

        return p;
    }

//**********************************************************************************************

    const Point Point::operator/(double div) const{ // p3 = p2 / 2;
        Point p(*this);
        p /= div;

        return p;
    }

//**********************************************************************************************

    double& Point::operator[](unsigned int index){
        if(index >= __dim)
            throw OutOfBoundsEx(__dim, index);

        return __values[index];
    }

//**********************************************************************************************

    const double& Point::operator[](unsigned int index) const {
        if (index >= __dim)
            throw OutOfBoundsEx(__dim, index);

        return __values[index];
    }

//**********************************************************************************************

    Point& operator+=(Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        for (int i = 0; i < P1.__dim; ++i){
            P1.__values[i] += P2.__values[i];
        }
        return P1;
    }

//**********************************************************************************************


    Point &operator-=(Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        for (int i = 0; i < P1.__dim; ++i){
            P1.__values[i] -= P2.__values[i];
        }
        return P1;
    }

//**********************************************************************************************

    const Point operator+(const Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        Point *newP = new Point(P1);
        *newP += P2;
        return *newP;
    }

//**********************************************************************************************

    const Point operator-(const Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        Point *newP = new Point(P1);
        *newP -= P2;
        return *newP;
    }

//**********************************************************************************************
    bool operator==(const Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        for (int i = 0; i < P1.__dim; ++i) {
            if(P1.__values[i] != P2.__values[i])
                return false;
        }

        return true;
    }

//**********************************************************************************************

    bool operator!=(const Point & P1, const Point & P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        return !(P1 == P2);
    }

//**********************************************************************************************

    bool operator<(const Point & P1, const Point & P2) {
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        for (int i = 0; i < P1.getDims(); ++i) {
            if (P1.__values[i] < P2.__values[i])
                return true;

            else if(P1.__values[i] > P2.__values[i])
                return false;
        }
        return false;
    }

//**********************************************************************************************

    bool operator>(const Point & P1, const Point &P2){
        if (P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        for(int i = 0; i < P1.getDims(); ++i){
            if(P1.__values[i] > P2.__values[i])
                return true;
        }
        return false;
    }

//**********************************************************************************************

    bool operator<=(const Point & P1, const Point &P2){
        if(P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        if (P1 > P2)
            return false;
        else
            return true;
    }

//**********************************************************************************************

    bool operator>=(const Point & P1, const Point &P2){
        if(P1.__dim != P2.__dim)
            throw DimensionalityMismatchEx(P1.__dim, P2.__dim);

        if (P1 < P2)
            return false;
        else
            return true;
    }


//**********************************************************************************************

    std::ostream &operator<<(std::ostream &out, const Point &P1){
        int i = 0;

        for( ; i < P1.getDims()-1; ++i)
            out << P1.__values[i] << ", ";

        out << P1.__values[i];

        return out;
    }

//**********************************************************************************************

    std::istream &operator>>(std::istream &in, Point &P1){

    }
}