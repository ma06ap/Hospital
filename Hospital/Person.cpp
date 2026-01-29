//
// Created by matin on 1/29/26.
//

#include "Person.h"

// Constructor
Person::Person(const std::string& name, const std::string& nationalityCode)
    : name(name), nationalityCode(nationalityCode) {
}

// Getters
std::string Person::getName() const {
    return name;
}

std::string Person::getNationalityCode() const {
    return nationalityCode;
}

// Setters
void Person::setName(const std::string& name) {
    this->name = name;
}

void Person::setNationalityCode(const std::string& nationalityCode) {
    this->nationalityCode = nationalityCode;
}

// Static validation functions
bool Person::isValidName(const std::string& name) {
    return !name.empty();
}

bool Person::isValidNationalityCode(const std::string& code) {
    return code.length() == 10;
}
