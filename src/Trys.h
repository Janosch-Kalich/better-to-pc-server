#ifndef TO_PC_SERVER_TRYS_H
#define TO_PC_SERVER_TRYS_H

#endif //TO_PC_SERVER_TRYS_H

namespace Trys
{
  extern int remaining;
  extern int failed;
  extern bool locked;

  void fail();

  void reset();

  void unlock();
}