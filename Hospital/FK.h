//
// Created by matin on 1/29/26.
//

#ifndef FK_H
#define FK_H

#include "DataManagement.h"

template <typename T>
class FK {
private:
    int relatedID;

public:
    FK(int id = -1) : relatedID(id) {}

    // Operator() overloading -> Functor behavior
    T* operator()() {
        return DataManagement<T>::getItemByID(relatedID);
    }

    void setID(int id) { relatedID = id; }
    int getID() const { return relatedID; }
};

#endif // FK_H