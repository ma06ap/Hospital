//
// Created by matin on 1/29/26.
//

#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <algorithm>

class Person {
protected:
    std::string name;
    std::string nationalityCode;

public:
    Person(std::string n, std::string nc) : name(n), nationalityCode(nc) {}


    virtual ~Person() {}

    std::string getName() const { return name; }
    std::string getNationalCode() const { return nationalityCode; }


    static bool checkName(const std::string& n) {
        return !n.empty();
    }

    static bool checkNationalCode(const std::string& nc) {
        if (nc.length() != 10) return false;
        return std::all_of(nc.begin(), nc.end(), ::isdigit);
    }
};

#endif // PERSON_H
