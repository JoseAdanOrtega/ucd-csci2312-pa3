//
// Created by Jose on 3/27/2016.
//

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>
#include "Cluster.h"
#include "Exceptions.h"

using std::cout;
using std::endl;

namespace Clustering
{

    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {
        //Left empty
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c): __dimensions(d), __c(c), __p(d) {
        __valid = false;
    }

//**********************************************************************************************

    const Point Cluster::Centroid::get() const {
        return __p;
    }
    void Cluster::Centroid::set(const Point &p) {
        __p = p;
        __valid = true;
    }
    bool Cluster::Centroid::isValid() const
    {
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid)
    {
        __valid = valid;
    }

//**********************************************************************************************

    void Cluster::Centroid::compute() {
        if(__c.__size > 0) {
            Point p(__dimensions);
            LNodePtr temp = __c.__points;
            int i = 0;

            while (temp != nullptr) {
                p += temp->point / __c.getSize();
                i++;
                temp = temp->next;
            }
            set(p);
        }

        else if (__c.__size == 0) {
            assert(__c.__points == nullptr);
            __valid = true;
            toInfinity();
        }
    }
    bool Cluster::Centroid::equal(const Point &p) const {
        if (p == __p) {
            return true;
        }
        return false;
    }
    void Cluster::Centroid::toInfinity() {
        for (int i = 0; i < __p.getDims(); ++i) {
            __p[i] = std::numeric_limits<double>::max();
        }
    }

//**********************************************************************************************

    unsigned int Cluster::__idGenerator = 0;

    void Cluster::__del() {
        while (__points != nullptr && __points->next != nullptr) {
            int i = 1;
            LNodePtr temp;
            temp = __points->next;
            delete __points;
            __points = temp;

            i++;
        }

        if (__points != nullptr && __points->next == nullptr) {
            delete __points;
        }
    }

    void Cluster::__cpy(LNodePtr pts) {
        if (pts == nullptr) {
            return;
        }

        this->__points = new LNode(pts->point, nullptr);
        this->__size++;

        pts = pts->next;
        while (pts != nullptr) {
            this->add(pts->point);
            pts = pts->next;
        }
    }

//**********************************************************************************************

    bool Cluster::__in(const Point &p) const {
        if (__dimensionality != p.getDims()) {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }

        if (__size == 0) {
            return false;
        }

        LNodePtr curr = __points;

        while (curr->next != nullptr) {
            if (curr->point.getId() == p.getId()) {
                return true;
            }

            else
                curr = curr->next;
        }

        if (curr->point.getId() == p.getId()) {
            return true;
        }

        return false;
    }

//**********************************************************************************************

    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';

    Cluster::Cluster(unsigned int d): __size(0), __points(nullptr), __dimensionality(d), centroid(d, *this) {
        __id = __idGenerator;
        __idGenerator++;
    }

    Cluster::Cluster(const Cluster &c) :__size(0), __dimensionality(c.__dimensionality), centroid(c.__dimensionality, c) {
        if (c.__size == 0) {
            __points = nullptr;
            __id = c.__id;
        }
        else if (c.__size == 1) {
            __points = new LNode(c.__points->point, nullptr);
            __size++;
            __id = c.__id;
        }

        else if (c.__size > 1) {
            LNodePtr temp = c.__points;
            __cpy(temp);
            __id = c.__id;
        }

        centroid.compute();
        assert(__size == c.__size);
    }

//**********************************************************************************************

    Cluster& Cluster::operator=(const Cluster &c) {

        if (this == &c) {
            return *this;
        }

        else {
            __del();
            LNodePtr temp = c.__points;
            __size = 0;
            __cpy(temp);
            centroid.compute();
            assert(this->__size == c.__size);
            __id = c.__id;
            return *this;
        }
    }

    Cluster::~Cluster() {
    }

    unsigned int Cluster::getSize() const {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const {
        return __id;
    }

//**********************************************************************************************

    void Cluster::add(const Point &pass) {
        if (pass.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, pass.getDims());
        }

        LNodePtr temp = new LNode(pass, nullptr);
        LNodePtr pre = __points;
        LNodePtr cur = __points;

        if (__size == 0) {
            __points = temp;
            __size++;
        }

        else if (__size == 1) {
            if (temp->point < cur->point) {
                LNodePtr foo = __points;
                temp->next = foo;
                __points = temp;

                __size++;
            }

            else {
                __points->next = temp;
                __size++;
            }
        }

        else if (__size > 1) {
            int i = 0;

            while (cur->next != nullptr) {
                if (temp->point < cur->point) {
                    if (cur == __points) {
                        LNodePtr foo = __points;
                        temp->next = foo;
                        __points = temp;

                        break;
                    }

                    else {
                        temp->next = cur;
                        pre->next = temp;

                        break;
                    }
                }

                cur = cur->next;

                if (i > 0) {
                    pre = pre->next;
                }
                i++;
            }

            if (cur->next == nullptr) {
                cur->next = temp;
            }

            __size++;

            centroid.setValid(false);
        }
    }

    const Point& Cluster::remove(const Point &pass) {
        if (pass.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, pass.getDims());
        }

        int i = 0;

        LNodePtr temp;
        LNodePtr pre = __points;
        LNodePtr cur = __points;

        while (cur != nullptr) {
            if (cur->point == pass) {
                if (cur == __points) {
                    __points = __points->next;
                    delete cur;

                    --__size;

                    break;
                }
                temp = cur;
                cur = cur->next;
                pre->next = cur;
                delete temp;

                --__size;

                break;
            }

            cur = cur->next;
            if (i > 0) {
                pre = pre->next;
            }

            i++;
        }

        if (__size == 0) {
            centroid.setValid(false);
        }

        return pass;
    }

    bool Cluster::contains(const Point &p) const {
        if (__dimensionality != p.getDims()) {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }

        if (__in(p)) {
            return true;
        }

        return false;
    }
    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
        LNodePtr curr = __points;
        Point inf(__dimensionality);

        for (int i = 0; i < __dimensionality; ++i) {
            inf[i] = std::numeric_limits<double>::max();
        }

        if (k >= __size) {
            for (int i = 0; i < __size; ++i) {
                *pointArray[i] = curr->point;
                curr = curr->next;
            }
            for (int i = __size; i < k; ++i) {
                *pointArray[i] = inf;
            }
        }

        else if (k < __size) {
            for (int i = 0; i < k; ++i) {
                *pointArray[i] = curr->point;
                curr = curr->next;
            }
        }
    }

//**********************************************************************************************

    const Point& Cluster::operator[](unsigned int index) const {
        if (__size <= 0) {
            throw EmptyClusterEx();
        }

        if (index >= __size) {
            throw OutOfBoundsEx(__size, index);
        }

        LNodePtr curr = __points;

        int i = 1;

        if (index == 0) {
            return __points->point;
        }

        curr = curr->next;

        while (curr->next != nullptr) {
            if (i == index) {
                return curr->point;
            }

            curr = curr->next;

            i++;
        }
        return curr->point;
    }

//**********************************************************************************************

    Cluster& Cluster::operator+=(const Point &rhs) {
        if (rhs.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, rhs.getDims());
        }

        this->add(rhs);

        centroid.setValid(false);

        return *this;
    }

    Cluster& Cluster::operator-=(const Point &p) {
        if (p.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        if (this->contains(p)) {
            this->remove(p);
            centroid.setValid(false);
        }

        return *this;
    }

//**********************************************************************************************

    Cluster& Cluster::operator+=(const Cluster &c) {
        LNodePtr temp = c.__points;

        while (temp != nullptr) {
            if (!this->__in(temp->point)) {
                this->add(temp->point);
            }

            temp = temp->next;
        }

        return *this;
    }

    Cluster& Cluster::operator-=(const Cluster &c) {
        LNodePtr temp = c.__points;
        while (temp != nullptr) {
            if (__in(temp->point)) {
                remove(temp->point);
                centroid.setValid(false);
            }

            temp = temp->next;
        }

        if (__size == 0) {
            centroid.setValid(false);
        }
        return *this;
    }

//**********************************************************************************************

    std::ostream& operator<<(std::ostream &os, const Cluster &c) {
        LNodePtr curr = c.__points;
        if (c.__size == 0) {
            return os;
        }

        else {
            while (curr->next != nullptr)
            {
                os << curr->point << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << c.__id << endl;
                curr = curr->next;
            }
            os << curr->point;
            os << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << c.__id;
            return os;
        }
    }

    std::istream& operator>>(std::istream &in, Cluster &c) {

        std::string wholeLine;

        while (getline(in, wholeLine)) {
            Point p(c.__dimensionality);
            std::stringstream lineStream(wholeLine);
            std::string value;

            int i = 0;
            double d = 0.0;

            while (getline(lineStream, value, Point::POINT_VALUE_DELIM)) {
                try {
                    if (i >= c.__dimensionality) {
                        p.rewindIdGen();

                        throw OutOfBoundsEx(c.__dimensionality, i);
                    }

                    d = stod(value);

                    p.setValue(i, d);
                }

                catch (OutOfBoundsEx &ex) {
                    std::cerr << "Caught Exception: " << ex << endl;
                    p.rewindIdGen();
                }

                i++;
            }

            try {
                if (i != c.__dimensionality) {
                    throw DimensionalityMismatchEx(c.__dimensionality, i);
                }

                c.add(p);
            }

            catch (DimensionalityMismatchEx &ex) {
                std::cerr << "Caught Exception: " << ex << endl;

            }

        }

        return in;
    }

//**********************************************************************************************

    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }

        if (lhs.__size != rhs.__size) {
            return false;
        }

        LNodePtr leftCur = lhs.__points;
        LNodePtr rightCur = rhs.__points;

        while (leftCur != nullptr) {
            if (leftCur->point != rightCur->point ) {
                return false;
            }

            leftCur = leftCur->next;
            rightCur = rightCur->next;
        }

        return true;
    }

    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }

        if (lhs == rhs) {
            return false;
        }

        return true;
    }

//**********************************************************************************************

    const Cluster operator+(const Cluster &lhs, const Point &rhs) {
        if (lhs.__dimensionality != rhs.getDims()) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }

        Cluster c(lhs);
        c.add(rhs);
        return c;
    }

    const Cluster operator-(const Cluster &lhs, const Point &rhs) {
        if (lhs.__dimensionality != rhs.getDims()) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }

        Cluster c(lhs);

        if (c.contains(rhs)) {
            c.remove(rhs);
        }
        return c;
    }

//**********************************************************************************************

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }

        Cluster c(lhs);
        c += rhs;

        return c;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }

        Cluster c(lhs);
        c -= rhs;

        return c;
    }

//**********************************************************************************************

    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to) {
    }


    void Cluster::Move::perform() {
        __to.add(__from.remove(__p));
    }
}