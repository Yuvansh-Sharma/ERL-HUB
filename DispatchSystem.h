#ifndef DISPATCHSYSTEM_H
#define DISPATCHSYSTEM_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

namespace ReliefOps {
namespace Dispatch {

// a medical dispatch request
struct MedicalNeed {
    int id;
    int priority;    // 1 = critical, 2 = serious, 3 = minor
    int patients;
    string location;

    int getId() const { return id; }
};

// custom functor - sorts most urgent medical needs first
struct UrgentMedicalFirst {
    template <typename T>
    bool operator()(const T& a, const T& b) const {
        return a.priority < b.priority;    // lower number = more urgent
    }
};

// custom stream manipulators using <iomanip> for table output
inline ostream& hline(ostream& os) {
    os << setfill('-') << setw(70) << "-" << setfill(' ') << "\n";
    return os;
}

inline ostream& vsep(ostream& os) {
    os << " | ";
    return os;
}

// generic priority queue using vector + unordered_map
// NOTE: T must have getId(); used here with MedicalNeed
template <typename T>
class PriorityDispatchQueue {
private:
    vector<T> items;                     // items in priority order
    unordered_map<int, int> idToIndex;   // id -> position for O(1) lookup

    void rebuildIndex() {
        idToIndex.clear();
        for (int i = 0; i < (int)items.size(); i++)
            idToIndex[items[i].getId()] = i;
    }

public:
    void push(const T& item) {
        idToIndex[item.getId()] = (int)items.size();
        items.push_back(item);
    }

    bool contains(int id) const {
        return idToIndex.find(id) != idToIndex.end();
    }

    void sortByPriority() {
        sort(items.begin(), items.end(), UrgentMedicalFirst());
        rebuildIndex();
    }

    T popMostUrgent() {
        if (items.empty()) return T();
        sortByPriority();
        T top = items.front();
        items.erase(items.begin());
        rebuildIndex();
        return top;
    }

    int size() const { return (int)items.size(); }
    bool empty() const { return items.empty(); }

    // tabular display using the custom manipulators
    void printTable() const {
        cout << hline;
        cout << setw(5) << "ID" << vsep
             << setw(10) << "PRIORITY" << vsep
             << setw(10) << "PATIENTS" << vsep
             << "LOCATION" << "\n";
        cout << hline;
        for (const T& item : items) {
            cout << setw(5) << item.getId() << vsep
                 << setw(10) << item.priority << vsep
                 << setw(10) << item.patients << vsep
                 << item.location << "\n";
        }
        cout << hline;
    }
};

} // namespace Dispatch
} // namespace ReliefOps

#endif