#include "Error.h"
#include <string>


Error::Error(char const* message) : message(message) {};

char const* Error::what() const {
     return message;
}