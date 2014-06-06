#ifndef _BUSYBIN_GOAL3_H_
#define _BUSYBIN_GOAL3_H_

#include "Goal.h"
#include "../RubiksCubeModel.h"

namespace busybin
{
  /**
   * Goal 3 - solve a 2x2x2 cube.
   */
  class Goal3 : public Goal
  {
  public:
    bool isSatisfied(RubiksCubeModel& cube);
  };
}

#endif
