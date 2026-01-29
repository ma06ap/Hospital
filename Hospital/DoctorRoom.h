//
// Created by matin on 1/29/26.
//

#ifndef DOCTORROOM_H
#define DOCTORROOM_H

#include "Room.h"

class DoctorRoom : public Room {
public:
    DoctorRoom() : Room(2) {} // ظرفیت ۲ بیمار

    bool enterStaff(Personnel* p) override {
        // شرط ۱: فقط دکتر
        if (p->role != Role::Doctor) return false;
        // شرط ۲: فقط ۱ نفر
        if (staffList.size() >= 1) return false;

        staffList.push_back(p);
        return true;
    }
};

#endif // DOCTORROOM_H