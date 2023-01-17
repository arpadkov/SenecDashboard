#include <exception>

struct MyException : public std::exception {
    const char* what() const throw () {
        return "C++ Exception";
    }
};