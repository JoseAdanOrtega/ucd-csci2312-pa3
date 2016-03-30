//
// Created by Jose on 3/25/2016.
//

#include "Point.h"
#include <cmath>

namespace Clustering {

    unsigned int Point::__idGen = 0;

    Point::Point(unsigned int dim){
        __id = __idGen++;
        __values = new double[dim];
        __dim = dim;

        for(int i = 0; i < dim; ++i){
            __values[i] = 0.0;
        }
    }

//**********************************************************************************************

    Point::Point(unsigned int dim, double * value){
        __id = __idGen++;
        __values = value;
        __dim = dim;
    }

//**********************************************************************************************

    Point::Point(const Point & P1){
        __id = P1.__id;
        __dim = P1.getDims();
        __values = new double[__dim];
        for(int i = 0; i < __dim; ++i){
            __values[i] = P1.__values[i];
        }
    }

//**********************************************************************************************

    Point& Point::operator=(const Point & other){
        if(this != &other){
            __id = other.__id;
            delete[] __values;
            __dim = other.getDims();
            __values = other.__values;
        }
        return *this;
    }

//**********************************************************************************************

    Point::~Point(){
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
        __values[dim] = val;
    }

//**********************************************************************************************

    double Point::getValue(unsigned int dim) const{
        if(dim <= getDims()){
            return __values[dim];
        }else {
            return 0;
        }
    }

//**********************************************************************************************

    double Point::distanceTo(const Point & other) const{
        double dist = 0.0;
        for (int i = 0; i < __dim; ++i){
            dist = dist + ((__values[i]-other.getValue(i)) * (__values[i]-other.getValue(i)));
        }
        return sqrt(dist);
    }

//**********************************************************************************************

    const Point Point::operator*(double mult) const { // prevent (p1 * 2) = p2;
        Point *newP = new Point(__dim);

        for (int i = 0; i < __dim; ++i) {
            newP->__values[i] = __values[i] * mult;
        }

        return *newP;
    }

//**********************************************************************************************

    const Point Point::operator/(double div) const{ // p3 = p2 / 2;
        Point *newP = new Point(__dim);

        for (int i = 0; i < __dim; ++i) {
            newP->__values[i] = __values[i] / div;
        }

        return *newP;
    }

//**********************************************************************************************

    double& Point::operator[](int index){
        return __values[index];
    }

//**********************************************************************************************

    bool operator==(const Point & P1, const Point & P2){

        bool pass = true;

        if(P1.__id != P2.__id){
            return false;
        }

        if (P1.__dim != P2.__dim){
            return false;
        }

        for (int i = 0; i < P1.getDims(); ++i){
            if(P1.__values[i] != P2.__values[i]){
                pass = false;
            }
        }
        return pass;
    }

//**********************************************************************************************

    bool operator!=(const Point & P1, const Point & P2){
        return !(P1 == P2);
    }

//**********************************************************************************************

    bool operator<(const Point & P1, const Point & P2) {

        if (P1.__dim < P2.__dim) {
            return true;
        }
        if (P1.__dim > P2.__dim) {
            return false;
        }

        for (int i = 0; i < P1.getDims(); ++i) {
            if (P1.__values[i] < P2.__values[i]) {
                return true;
            }else if(P1.__values[i] > P2.__values[i]){
                return false;
            }
        }
        return false;
    }

//**********************************************************************************************

    bool operator>(const Point & P1, const Point &P2){

        if(P1.__dim > P2.__dim){
            return true;
        }
        if(P1.__dim < P2.__dim){
            return false;
        }

        for(int i = 0; i < P1.getDims(); ++i){
            if(P1.__values[i] > P2.__values[i]){
                return true;
            }else if(P1.__values[i] < P2.__values[i]){
                return false;
            }
        }
        return false;
    }

//**********************************************************************************************

    bool operator<=(const Point & P1, const Point &P2){
        if (P1<P2) {
            return true;
        }
        if (P1>P2) {
            return false;
        }
        return true;
    }

//**********************************************************************************************

    bool operator>=(const Point & P1, const Point &P2){
        if (P1>P2) {
            return true;
        }
        if (P1<P2) {
            return false;
        }
        return true;
    }

//**********************************************************************************************

    Point& operator+=(Point & P1, const Point & P2){
        Point *newP = new Point(P2);

        for (int i = 0; i < P1.__dim; ++i){
            P1.__values[i] = P1.__values[i] + newP->getValue(i);
        }
        delete newP;
        return P1;
    }

//**********************************************************************************************

    Point &operator-=(Point & P1, const Point & P2){
        Point *newP = new Point(P2);

        for (int i = 0; i < P1.__dim; ++i){
            P1.__values[i] = P1.__values[i] - newP->getValue(i);
        }
        delete newP;
        return P1;
    }

//**********************************************************************************************

    Point & Point::operator*=(double mult){ // p *= 6; p.operator*=(6);
        for (int i = 0; i < __dim; ++i){
            __values[i] = __values[i] * mult;
        }
        return *this;
    }

//**********************************************************************************************

    Point & Point::operator/=(double div){
        for (int i = 0; i < __dim; ++i){
            __values[i] = __values[i] / div;
        }
        return *this;
    }

//**********************************************************************************************

    const Point operator+(const Point & P1, const Point & P2){
        Point *newP = new Point(P1);
        *newP += P2;
        return *newP;
    }

//**********************************************************************************************

    const Point operator-(const Point & P1, const Point & P2){
        Point *newP = new Point(P1);
        *newP -= P2;
        return *newP;
    }

//**********************************************************************************************

    std::ostream &operator<<(std::ostream &out, const Point &P1){
        int i = 0;
        for( ; i < P1.getDims()-1; ++i){
            out << P1.getValue(i);
            out << ", ";
        }
        out << P1.getValue(i);

        return out;
    }

//**********************************************************************************************

    std::istream &operator>>(std::istream &in, Point &P1){
        int index = 0;

        while ((in.peek() != '\n') || (in.peek() != '\r')){
            in >> P1[index];
            if((in.peek() == '\n') || (in.peek() == '\r') || (in.eof())){
                return in;
            }
            in.ignore(100, ',');
            index++;
        }
        return in;
    }
}