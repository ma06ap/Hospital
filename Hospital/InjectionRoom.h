//
// Created by matin on 1/29/26.
//

#ifndef INJECTIONROOM_H
#define INJECTIONROOM_H

#include "Room.h"

class InjectionRoom : public Room {
public:
    InjectionRoom() : Room(3) {} // ظرفیت ۳ بیمار

    bool enterStaff(Personnel* p) override {
        // شرط ۱: فقط پرستار
        if (p->role != Role::Nurse) return false;
        // شرط ۲: فقط ۱ نفر
        if (staffList.size() >= 1) return false;

        staffList.push_back(p);
        return true;
    }
};

#endif // INJECTIONROOM_H