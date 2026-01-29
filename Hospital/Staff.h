//
// Created by matin on 1/29/26.
//

#ifndef HOSPITAL_STAFF_H
#define HOSPITAL_STAFF_H

#include "Person.h"

enum StaffRole {
    Doctor,
    Nurse,
    Employee
};

class Staff : public Person {
private:
    std::string staffCode;
    StaffRole role;

public:
    // Constructor
    Staff(const std::string& name, const std::string& nationalityCode,
          const std::string& staffCode, StaffRole role);

    // Destructor
    virtual ~Staff() = default;

    // Getters
    std::string getStaffCode() const;
    StaffRole getRole() const;

    // Setters
    void setStaffCode(const std::string& staffCode);
    void setRole(StaffRole role);

    // Static validation function
    static bool isValidStaffCode(const std::string& code);
};

#endif //HOSPITAL_STAFF_H
