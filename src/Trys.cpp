#include "Trys.h"
#include "Toast.h"

namespace Trys
{
  int remaining = 5;
  int failed = 0;
  bool locked = false;

  void fail()
  {
    remaining--;
    failed++;

    if (remaining <= 0)
    {
      locked = true;
      remaining = 0;
      show_security_toast();
    }
  }

  void reset()
  {
    remaining = 5;
    failed = 0;
  }

  void unlock()
  {
    reset();
    locked = false;
  }
}