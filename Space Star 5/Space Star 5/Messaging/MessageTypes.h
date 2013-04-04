#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

//Will work on making enumerator for message types
//will need to have message that weapon has been fired
//enemy has been destroyed, player destroyed.
//may not even use a messaging system who knows.

enum message_type
{
  shootAtEnemy,
  shootAtPlayer,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  /*case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";*/

  default:

    return "Not recognized!";
  }
}

#endif