//
// Created by matin on 1/29/26.
//

#ifndef AUTOINCREMENT_H
#define AUTOINCREMENT_H

template <typename T>
class AutoIncrement {
protected:
    static int lastID;
    int id;

public:
    AutoIncrement() {
        lastID++;
        id = lastID;
    }

    virtual ~AutoIncrement() {} // Virtual destructor for safety

    int getID() const { return id; }
};

// مقداردهی اولیه متغیر استاتیک
template <typename T>
int AutoIncrement<T>::lastID = 0;

#endif // AUTOINCREMENT_H