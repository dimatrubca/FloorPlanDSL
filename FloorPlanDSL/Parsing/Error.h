#pragma once
#include <iostream>

#pragma once
#ifndef _MYERROR_
#define _MYERROR_

class Error : public std::exception
{
public:
     Error(char const* message);
     virtual char const* what() const;

     static void Assert(bool predicate, std::string message) {
          if (!predicate) {
               throw new Error(message.c_str());
          }
     }
private:
     char const* message;
};
#endif