//
// Created by matin on 1/29/26.
//

#ifndef PERSONNEL_H
#define PERSONNEL_H

#include "Person.h"
#include "AutoIncrement.h"

enum class Role {
    Doctor,
    Nurse
};

class Personnel : public Person, public AutoIncrement<Personnel> {
public:
    std::string personnelCode;
    Role role;

    Personnel(std::string n, std::string nc, std::string pc, Role r)
        : Person(n, nc), personnelCode(pc), role(r) {}

    static bool checkPersonnelCode(const std::string& pc) {
        if (pc.length() != 7) return false;
        return std::all_of(pc.begin(), pc.end(), ::isdigit);
    }
};

#endif // PERSONNEL_H