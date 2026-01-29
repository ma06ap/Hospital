//
// Created by matin on 1/29/26.
//

#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>

enum class MedicineUnit {
    Tablet,
    Syrup,
    Ampoule, // تزریقی
    Serum    // تزریقی
};

class Medicine {
public:
    std::string name;
    MedicineUnit unit;
    int quantity;
    // double price; // در صورت نیاز بعدا اضافه شود

    Medicine(std::string n, MedicineUnit u, int q)
        : name(n), unit(u), quantity(q) {}
};

#endif // MEDICINE_H