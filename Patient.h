//
// Created by matin on 1/29/26.
//

#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include "AutoIncrement.h"
#include "Disease.h"
#include "Treatment.h"

class Patient : public Person, public AutoIncrement<Patient> {
public:
    Disease* disease;
    Treatment* treatment;
    bool isCured;

    Patient(std::string n, std::string nc)
        : Person(n, nc), disease(nullptr), treatment(nullptr), isCured(false) {
        treatment = new Treatment();
    }

    ~Patient() {
        if (disease) delete disease;
        if (treatment) delete treatment;
    }

    void setDisease(Disease* d) { disease = d; }

    void cure() {
        isCured = true;
        if (disease) {
            delete disease;
            disease = nullptr;
        }
    }


    std::string getStatus() {
        if (isCured) return "Cured";
        if (treatment && treatment->needsInjection) return "Need Injection";
        if (treatment && !treatment->gotMedicine) return "Need Medicine";
        return "Doctor Visit Needed";
    }
};

#endif // PATIENT_H
