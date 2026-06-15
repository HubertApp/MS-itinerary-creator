#pragma once
#include <stdexcept>
#include <string>

namespace astar {

    class BadValueException : public std::runtime_error {
    public:
        explicit BadValueException(const std::string msg) 
            : std::runtime_error(msg) {}
    };


}