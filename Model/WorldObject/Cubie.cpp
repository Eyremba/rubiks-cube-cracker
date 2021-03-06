#include "Cubie.h"

namespace busybin
{
  /**
   * Initialize the Cube.
   * @param pProgram The currently installed shader program.
   * @param pMatrixStack The World's MatrixStack.
   * @param name The name of this cubie.
   * @param position The position of this cubie.
   */
  Cubie::Cubie(Program* pProgram, MatrixStack* pMatrixStack,
    const string& name, const vec3& position) : 
    Cube(1.0f, pProgram, pMatrixStack, name, 
    {
      vec4(0.9f, 0.9f, 0.9f, 1.0f), // UP (W).
      vec4(0.0f, 0.5f, 0.0f, 1.0f), // LEFT (G).
      vec4(0.8f, 0.0f, 0.0f, 1.0f), // FRONT (R).
      vec4(0.0f, 0.0f, 0.5f, 1.0f), // RIGHT (B).
      vec4(1.0f, 0.4f, 0.0f, 1.0f), // BACK (O).
      vec4(0.8f, 0.8f, 0.0f, 1.0f)  // DOWN (Y).
    })
  {
    this->cubeRot.speed = 8.0f;
    this->translation   = translate(mat4(1.0f), position);
    this->superGlow     = false;
    this->setMaterial(unique_ptr<Material>(new GrayPlastic()));
  }

  /**
   * Draw the Cubie.
   * @param elapsed The number of elapsed seconds since the last pulse.
   */
  void Cubie::draw(double elapsed)
  {
    // Set up the MVP.
    this->getMatrixStack()->pushModel();
    this->getMatrixStack()->topModel() *= this->animateCubeRotation(elapsed) * this->translation;
    this->getProgram()->setMVP(*this->getMatrixStack());
    this->getMatrixStack()->popModel();

    // The translation is needed for the hidden sides.
    this->getProgram()->setUniform("cubieTranslation", this->translation);

    // Install the material.
    this->getProgram()->setUniform("material", this->getMaterial());

    // Setup super glow (debugging).
    this->getProgram()->setUniform("superGlow", this->superGlow);

    // Draw the object.
    this->getProgram()->drawArrays(this->getVAO(), this->vertices.size());
  }

  /**
   * Return the program (downcast).
   */
  RubiksCubeProgram* Cubie::getProgram() const
  {
    return dynamic_cast<RubiksCubeProgram*>(WorldObject::getProgram());
  }

  /**
   * Rotate the cubie.
   * @param rads The amount to rotate, in radians.
   * @param axis The axis of rotation.
   */
  void Cubie::rotate(float rads, const vec3& axis)
  {
    this->cubeRot.desired = normalize(glm::rotate(quat(), rads, axis) * this->cubeRot.desired);
  }

  /**
   * Animate the cube rotations.
   * @param elapsed The elapsed time since the last draw call.
   */
  mat4 Cubie::animateCubeRotation(double elapsed)
  {
    float cosTheta;

    // Spherical linear interpolation (SLERP) between the current and desired
    // orientations.  The orientation needs to be normalized or precision
    // will be lost over time, causing constantly animated cubes.
    this->cubeRot.orientation = normalize(slerp(this->cubeRot.orientation,
      this->cubeRot.desired, this->cubeRot.speed * (float)elapsed));

    // The cosine between the orientation and desired.
    cosTheta = dot(this->cubeRot.orientation, this->cubeRot.desired);

    //if (cosTheta > .999 && cosTheta < 1.001 || cosTheta)
    if (fabs(1 - fabs(cosTheta)) < .00001)
      this->cubeRot.orientation = this->cubeRot.desired;

    return mat4_cast(this->cubeRot.orientation);
  }

  /**
   * Enable super glow.  This is really for debugging - it highlights certain
   * cubies in the Rubik's Cube.
   * @param superGLow Whether or not to make this cube glow.
   */
  void Cubie::setSuperGlow(bool superGlow)
  {
    this->superGlow = superGlow;
  }
}

