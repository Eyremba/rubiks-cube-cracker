#include "MoveStore.h"

namespace busybin
{
  /**
   * Init.
   * @param cube A RubiksCube reference for storing the move
   *        string->function map.
   */
  MoveStore::MoveStore(RubiksCube& cube) : 
    moves
    ({{
      "L", "L'", "L2",
      "R", "R'", "R2",
      "U", "U'", "U2",
      "D", "D'", "D2",
      "F", "F'", "F2",
      "B", "B'", "B2"
    }}),
    inverseMoves
    ({
      {"L", "L'"}, {"L'", "L"}, {"L2", "L2"},
      {"R", "R'"}, {"R'", "R"}, {"R2", "R2"},
      {"U", "U'"}, {"U'", "U"}, {"U2", "U2"},
      {"D", "D'"}, {"D'", "D"}, {"D2", "D2"},
      {"F", "F'"}, {"F'", "F"}, {"F2", "F2"},
      {"B", "B'"}, {"B'", "B"}, {"B2", "B2"}
    })
  {
    // Set up the move map.
    this->moveMap["L"]  = bind(&RubiksCube::l,      &cube);
    this->moveMap["L'"] = bind(&RubiksCube::lPrime, &cube);
    this->moveMap["L2"] = bind(&RubiksCube::l2,     &cube);

    this->moveMap["R"]  = bind(&RubiksCube::r,      &cube);
    this->moveMap["R'"] = bind(&RubiksCube::rPrime, &cube);
    this->moveMap["R2"] = bind(&RubiksCube::r2,     &cube);

    this->moveMap["U"]  = bind(&RubiksCube::u,      &cube);
    this->moveMap["U'"] = bind(&RubiksCube::uPrime, &cube);
    this->moveMap["U2"] = bind(&RubiksCube::u2,     &cube);

    this->moveMap["D"]  = bind(&RubiksCube::d,      &cube);
    this->moveMap["D'"] = bind(&RubiksCube::dPrime, &cube);
    this->moveMap["D2"] = bind(&RubiksCube::d2,     &cube);

    this->moveMap["F"]  = bind(&RubiksCube::f,      &cube);
    this->moveMap["F'"] = bind(&RubiksCube::fPrime, &cube);
    this->moveMap["F2"] = bind(&RubiksCube::f2,     &cube);

    this->moveMap["B"]  = bind(&RubiksCube::b,      &cube);
    this->moveMap["B'"] = bind(&RubiksCube::bPrime, &cube);
    this->moveMap["B2"] = bind(&RubiksCube::b2,     &cube);
  }

  /**
   * Return the list of available moves.
   */
  const array<string, 18>& MoveStore::getMoves() const
  {
    return this->moves;
  }

  /**
   * Get a move by index.
   * @param ind The move index.
   */
  string MoveStore::getMove(unsigned ind) const
  {
    if (ind >= this->moves.size())
      throw RubiksCubeException("Index out of bounds in MoveStore::getMove.");

    return this->moves.at(ind);
  }

  /**
   * Get the inverse of a move (e.g. the inverse of L is L').
   * @param move The move for which the inverse shall be returned.
   */
  string MoveStore::getInverseMove(const string& move) const
  {
    if (!this->inverseMoves.count(move))
      throw RubiksCubeException("Index out of bounds in MoveStore::getInverseMove.");

    return this->inverseMoves.at(move);
  }

  /**
   * Return the number of available moves.
   */
  unsigned MoveStore::getNumMoves() const
  {
    return this->moves.size();
  }

  /**
   * Get a move map with the string representation of a move
   * to the corresponding move function in the cube passed
   * to the constructor.
   */
  const MoveStore::moveMap_t& MoveStore::getMoveMap() const
  {
    return this->moveMap;
  }

  /**
   * Get the move function in the cube passed to the constructor
   * based on the string representation.
   * @param string The string representation of the move.
   */
  MoveStore::moveFunc_t& MoveStore::getMoveFunc(const string& move)
  {
    if (!this->moveMap.count(move))
      throw RubiksCubeException("Invalid move in MoveStore::getMoveFunc.");

    return this->moveMap[move];
  }

  /**
   * Get the inverse move function corresponding to move.
   * @param string The string representation of the move for which the inverse
   *        move function shall be returned.
   */
  MoveStore::moveFunc_t& MoveStore::getInverseMoveFunc(const string& move)
  {
    return this->moveMap[this->getInverseMove(move)];
  }

  /**
   * Make sure the string representation of a move is valid.
   * @param string The string representation of the move.
   */
  bool MoveStore::isValidMove(const string& move) const
  {
    return this->moveMap.count(move) == 1;
  }
}
