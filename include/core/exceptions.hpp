#pragma once
#include <stdexcept>
#include <string>

namespace astar {

    class BadValueException : public std::runtime_error {
    public:
        explicit BadValueException(const std::string msg) 
            : std::runtime_error(msg) {}
    };

    class NodeNotExistException : public std::runtime_error {
    public:
        explicit NodeNotExistException(const std::string msg) 
            : std::runtime_error(msg) {}
    };

    class EmptyPqException : public std::runtime_error {
    public:
        explicit EmptyPqException(const std::string msg) 
            : std::runtime_error(msg) {}
    };


}