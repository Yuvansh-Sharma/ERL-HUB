#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

namespace ReliefOps {
namespace Exceptions {

class CargoOverweightException : public exception {
private:
    string message;
public:
    CargoOverweightException(const string& msg) { message = msg; }
    const char* what() const noexcept override { return message.c_str(); }
};

class FuelDepletionException : public exception {
private:
    string message;
public:
    FuelDepletionException(const string& msg) { message = msg; }
    const char* what() const noexcept override { return message.c_str(); }
};

class DamagedCorridorException : public exception {
private:
    string message;
public:
    DamagedCorridorException(const string& msg) { message = msg; }
    const char* what() const noexcept override { return message.c_str(); }
};

} // namespace Exceptions
} // namespace ReliefOps
#endif