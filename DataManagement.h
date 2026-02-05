//
// Created by matin on 1/29/26.
//

#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <vector>
#include <algorithm>
#include <functional>

template <typename T>
class DataManagement {
private:
    static std::vector<T*> dataStore;

public:
    static void create(T* obj) {
        dataStore.push_back(obj);
    }

    static T* getItemByID(int id) {

        auto it = std::find_if(dataStore.begin(), dataStore.end(),
            [id](T* obj) { return obj->getID() == id; });

        if (it != dataStore.end()) {
            return *it;
        }
        return nullptr;
    }


    static std::vector<T*> filter(std::function<bool(T*)> predicate) {
        std::vector<T*> result;
        for (auto obj : dataStore) {
            if (predicate(obj)) {
                result.push_back(obj);
            }
        }
        return result;
    }

    static std::vector<T*> getAll() {
        return dataStore;
    }
};

template <typename T>
std::vector<T*> DataManagement<T>::dataStore;

#endif // DATAMANAGEMENT_H
