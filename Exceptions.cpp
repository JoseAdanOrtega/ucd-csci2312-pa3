//
// Created by Jose on 3/25/2016.
//

#include "Exceptions.h"

namespace Clustering {

    Clustering::OutOfBoundsEx(unsigned int c, int r){
         __current = c;
         __rhs = r;
         __name = "OutBoundsEx";
    }

    unsigned int OutOfBoundsEx::getCurrent() const{
        return __current;
    }

    int OutOfBoundsEx::getRhs() const{
        return __rhs;
    }

    std::string OutOfBoundsEx::getName() const{
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex){
        os << ex.getName();
        return os;
    }

//**********************************************************************************************

    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __current = c;
        __rhs = r;
        __name = "DimensionalityMismatchEx";
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

    std::string DimensionalityMismatchEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {
        os << ex.getName();
        return os;
    }

//**********************************************************************************************

    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEX";
    }

    std::string ZeroClustersEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex){
        os << ex.getName();
        return os;
    }

//**********************************************************************************************

    DataFileOpenEx::DataFileOpenEx(std::string filename) {
        __name = "DataFileOpenEX";
        __filename = filename;
    }

    std::string DataFileOpenEx::getFilename() const {
        return __filename;
    }

    std::string DataFileOpenEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex){
        os << ex.getName() << "(filename=" << ex.getFilename() << ')';
        return os;
    }

//**********************************************************************************************

    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }

    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex){
        os << ex.getName();
        return os;
    }

//**********************************************************************************************

    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }

    std::string EmptyClusterEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex){
        os << ex.getName();
        return os;
    }
}