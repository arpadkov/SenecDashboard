#pragma once

#include <exception>

struct InvalidCredentialsException : public std::exception {
    const char* what() const throw () {
        return "Login credentials are invalid, please check login file";
    }
};

struct NetworkErrorException : public std::exception {
    const char* what() const throw () {
        return "Did not get valid HTTP response, check network connection";
    }
};

struct IncorrectLoginFileException : public std::exception {
    const char* what() const throw () {
        return "Login file is either not found or not in correct format";
    }
};

struct IncorrectDashboardResponse : public std::exception {
    const char* what() const throw () {
        return "Received Dashboard data not in correct format";
    }
};
