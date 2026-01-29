//
// Created by matin on 1/29/26.
//

#ifndef PHARMACY_H
#define PHARMACY_H

#include "Room.h"

class Pharmacy : public Room {
public:
    Pharmacy() : Room(100) {} // ظرفیت بالا (صف بیرون ندارد)

    bool enterStaff(Personnel* p) override {
        // شرط ۱: فقط پرستار
        if (p->role != Role::Nurse) return false;
        // شرط ۲: حداکثر ۲ نفر
        if (staffList.size() >= 2) return false;

        staffList.push_back(p);
        return true;
    }
};

#endif // PHARMACY_H