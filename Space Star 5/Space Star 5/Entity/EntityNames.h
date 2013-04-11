
#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Viper_ship,
  ent_Fighter_ship,
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Viper_ship:

    return "Viper";

  case ent_Fighter_ship:
    
    return "Fighter"; 

  default:

    return "UNKNOWN!";
  }
}
#endif