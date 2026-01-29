//
// Created by matin on 1/29/26.
//

#include "Patient.h"

// Constructor
Patient::Patient(const std::string& name, const std::string& nationalityCode, InsuranceType insurance)
    : Person(name, nationalityCode), insurance(insurance) {
}

// Getter
InsuranceType Patient::getInsurance() const {
    return insurance;
}

// Setter
void Patient::setInsurance(InsuranceType insurance) {
    this->insurance = insurance;
}
