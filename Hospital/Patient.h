//
// Created by matin on 1/29/26.
//

#ifndef HOSPITAL_PATIENT_H
#define HOSPITAL_PATIENT_H

#include "Person.h"

enum InsuranceType {
    None,
    Basic,
    Supplemental
};

class Patient : public Person {
private:
    InsuranceType insurance;

public:
    // Constructor
    Patient(const std::string& name, const std::string& nationalityCode, InsuranceType insurance);

    // Destructor
    virtual ~Patient() = default;

    // Getter
    InsuranceType getInsurance() const;

    // Setter
    void setInsurance(InsuranceType insurance);
};

#endif //HOSPITAL_PATIENT_H
