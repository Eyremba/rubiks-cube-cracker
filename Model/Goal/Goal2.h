#ifndef _BUSYBIN_GOAL2_H_
#define _BUSYBIN_GOAL2_H_

#include "Goal.h"
#include "../RubiksCubeModel.h"

namespace busybin
{
  /**
   * Goal 2 - solve any adjacent edge.
   */
  class Goal2 : public Goal
  {
  public:
    bool isSatisfied(const RubiksCubeModel& cube);
  };
}

#endif

