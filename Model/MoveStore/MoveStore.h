#ifndef _BUSYBIN_MOVE_STORE_H_
#define _BUSYBIN_MOVE_STORE_H_

#include "../../Util/RubiksCubeException.h"
#include <array>
using std::array;
#include <string>
using std::string;
#include <map>
using std::map;
#include <functional>
using std::function;
using std::bind;

namespace busybin
{
  /**
   * Base class for the MoveStore classes.
   */
  class MoveStore
  {
  public:
    typedef function<void()>        moveFunc_t;
    typedef map<string, moveFunc_t> moveMap_t;
    typedef map<string, string>     invMove_t;

  private:
    array<string, 18> moves;
    moveMap_t         moveMap;
    invMove_t         inverseMoves;

  protected:
    virtual moveMap_t& getMoveMap() = 0;

  public:
    MoveStore();

    const array<string, 18>& getMoves() const;
    string getMove(unsigned ind) const;
    string getInverseMove(const string& move) const;
    unsigned getNumMoves() const;

    virtual const moveMap_t& getMoveMap() const = 0;
    bool isValidMove(const string& move) const;
    moveFunc_t& getMoveFunc(const string& move);
    moveFunc_t& getInverseMoveFunc(const string& move);
  };
}

#endif
