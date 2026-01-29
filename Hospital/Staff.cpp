//
// Created by matin on 1/29/26.
//

#include "Staff.h"
#include <algorithm>
#include <cctype>

// Constructor
Staff::Staff(const std::string& name, const std::string& nationalityCode,
             const std::string& staffCode, StaffRole role)
    : Person(name, nationalityCode), staffCode(staffCode), role(role) {
}

// Getters
std::string Staff::getStaffCode() const {
    return staffCode;
}

StaffRole Staff::getRole() const {
    return role;
}

// Setters
void Staff::setStaffCode(const std::string& staffCode) {
    this->staffCode = staffCode;
}

void Staff::setRole(StaffRole role) {
    this->role = role;
}

// Static validation function
bool Staff::isValidStaffCode(const std::string& code) {
    // Check if length is exactly 7
    if (code.length() != 7) {
        return false;
    }

    // Check if all characters are digits (0-9)
    return std::all_of(code.begin(), code.end(), [](char c) {
        return std::isdigit(c);
    });
}
