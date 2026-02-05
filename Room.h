//
// Created by matin on 1/29/26.
//

#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <deque>
#include <queue>
#include "Personnel.h"
#include "Patient.h"

class Room {
protected:
    std::vector<Personnel*> staffList;
    std::deque<Patient*> activePatients;
    std::queue<Patient*> waitingQueue;
    int capacity;

public:
    Room(int cap) : capacity(cap) {}
    virtual ~Room() {}

    virtual bool enterStaff(Personnel* p) = 0;

    void exitStaff(Personnel* p) {

        auto it = std::find(staffList.begin(), staffList.end(), p);
        if (it != staffList.end()) {
            staffList.erase(it);
        }
    }

    void addPatient(Patient* p) {
        if (activePatients.size() < capacity) {
            activePatients.push_back(p);
        } else {
            waitingQueue.push(p);
        }
    }

    void removePatient(Patient* p) {

        auto it = std::find(activePatients.begin(), activePatients.end(), p);
        if (it != activePatients.end()) {
            activePatients.erase(it);
            if (!waitingQueue.empty()) {
                activePatients.push_back(waitingQueue.front());
                waitingQueue.pop();
            }
        }
    }


    std::deque<Patient*> getActivePatients() { return activePatients; }
    std::queue<Patient*> getWaitingQueue() { return waitingQueue; }
};

#endif // ROOM_H
