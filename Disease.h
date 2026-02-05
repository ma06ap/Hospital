//
// Created by matin on 1/29/26.
//

#ifndef DISEASE_H
#define DISEASE_H

#include <string>

enum class DiseaseType {
    Cold,
    Cancer,
    HeartIssue,
    Fracture,
    Infection,
    Other
};

class Disease {
public:
    DiseaseType type;
    std::string symptoms;
    std::string patientDescription;

    Disease(DiseaseType t, std::string sym, std::string desc)
        : type(t), symptoms(sym), patientDescription(desc) {}
};

#endif // DISEASE_H