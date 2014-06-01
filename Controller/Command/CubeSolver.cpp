#include "CubeSolver.h"

namespace busybin
{
  /**
   * Init.
   * @param pWorld Pointer to the world (must remain in scope).
   * @param pWorldWnd The world window, used to get the current width/heigth.
   * @param pMover Pointer to the CubeMover command.
   */
  CubeSolver::CubeSolver(World* pWorld, WorldWindow* pWorldWnd, CubeMover* pMover) :
    Command(pWorld, pWorldWnd), threadPool(1), solving(false)
  {
    // Listen for keypress events.
    pWorldWnd->onKeypress(bind(&CubeSolver::onKeypress, ref(*this), _1, _2, _3, _4));
    this->pMover = pMover;
  }

  /**
   * Fires when a key is pressed
   * @param window The window (same as this->pWindow).
   * @param key The key code.
   * @param scancode The platform-dependent scan code of the key.
   * @param action GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT.
   * @param mods Modifiers like alt.
   */
  void CubeSolver::onKeypress(int key, int scancode, int action, int mods)
  {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE && !this->solving)
    {
      this->solving = true;
      this->pMover->disable();

      // Get a copy of the underlying RC model.
      this->cube = dynamic_cast<RubiksCube&>(this->getWorld()->at("RubiksCube")).getRawModel();

      // Fire off a thread to solve the cube.
      this->threadPool.addJob(bind(&CubeSolver::solveCube, this));
    }
  }

  /**
   * Solve the cube.  This is run in a separate thread.
   */
  void CubeSolver::solveCube()
  {
    RubiksCubeView cubeView;
    MoveStore      moveStore(this->cube);
    CubeSearcher   searcher(this->cube);
    vector<string> allMoves;
    vector<string> goalMoves;
    Goal1          goal1;
    Goal2          goal2;

    // Display the intial cube model.
    cubeView.render(this->cube);

    // Try to achieve the goals.
    searcher.find(goal1, goalMoves);
    cout << "Found goal 1." << endl;
    allMoves.insert(allMoves.end(), goalMoves.begin(), goalMoves.end());
    for (string move : goalMoves)
      moveStore.getMoveFunc(move)();
    goalMoves.clear();

    searcher.find(goal2, goalMoves);
    cout << "Found goal 2." << endl;
    allMoves.insert(allMoves.end(), goalMoves.begin(), goalMoves.end());
    for (string move : goalMoves)
      moveStore.getMoveFunc(move)();
    goalMoves.clear();

    // Print the moves.
    for (string move : allMoves)
      cout << move << ' ';
    cout << endl;

    // Display the cube model.
    cubeView.render(this->cube);

    // Done solving - re-enable movement.
    this->solving = false;
    this->pMover->enable();
  }
}
