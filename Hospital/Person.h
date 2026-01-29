//
// Created by matin on 1/29/26.
//

#ifndef HOSPITAL_PERSON_H
#define HOSPITAL_PERSON_H

#include <string>

class Person {
protected:
    std::string name;
    std::string nationalityCode;
    int id; // Unique identifier for each person
public:
    // Constructor
    Person(const std::string& name, const std::string& nationalityCode);

    // Destructor
    virtual ~Person() = default;

    // Getters
    std::string getName() const;
    std::string getNationalityCode() const;

    // Setters
    void setName(const std::string& name);
    void setNationalityCode(const std::string& nationalityCode);

    // Static validation functions
    static bool isValidName(const std::string& name);
    static bool isValidNationalityCode(const std::string& code);
};

#endif //HOSPITAL_PERSON_H
