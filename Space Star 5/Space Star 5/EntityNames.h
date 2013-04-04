#pragma once
#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  entBaseShip
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case entBaseShip:
	  return "Base Ship";
  default:
    return "UNKNOWN!";
  }
}

#endif