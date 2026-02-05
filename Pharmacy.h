//
// Created by matin on 1/29/26.
//

#ifndef PHARMACY_H
#define PHARMACY_H

#include "Room.h"

class Pharmacy : public Room {
public:
    Pharmacy() : Room(100) {}

    bool enterStaff(Personnel* p) override {

        if (p->role != Role::Nurse) return false;
        if (staffList.size() >= 2) return false;

        staffList.push_back(p);
        return true;
    }
};

#endif // PHARMACY_H
