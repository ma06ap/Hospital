//
// Created by matin on 1/29/26.
//

#ifndef TREATMENT_H
#define TREATMENT_H

#include <vector>
#include "Medicine.h"

class Treatment {
public:
    std::vector<Medicine> medicines;
    bool gotMedicine;
    bool needsInjection;

    Treatment() : gotMedicine(false), needsInjection(false) {}

    void addMedicine(const Medicine& med) {
        medicines.push_back(med);
        if (med.unit == MedicineUnit::Ampoule || med.unit == MedicineUnit::Serum) {
            needsInjection = true;
        }
    }
};

#endif // TREATMENT_H
