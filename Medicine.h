//
// Created by matin on 1/29/26.
//

#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>

enum class MedicineUnit {
    Tablet,
    Syrup,
    Ampoule,
    Serum
};

class Medicine {
public:
    std::string name;
    MedicineUnit unit;
    int quantity;


    Medicine(std::string n, MedicineUnit u, int q)
        : name(n), unit(u), quantity(q) {}
};

#endif // MEDICINE_H
