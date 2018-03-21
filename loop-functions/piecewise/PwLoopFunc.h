/**
  * @file <loop-functions/example/PwLoopFunc.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef PW_LOOP_FUNC
#define PW_LOOP_FUNC

#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include "../../src/core/AutoMoDeLoopFunctions.h"

using namespace argos;

class PwLoopFunction: public AutoMoDeLoopFunctions {
  public:
    PwLoopFunction();
    PwLoopFunction(const PwLoopFunction& orig);
    virtual ~PwLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void PostStep();
    virtual void Reset();

    Real GetObjectiveFunction();

    CVector3 GetRandomPosition();

    void ArenaControl();

    Real PwFunctionZero();

    Real PwFunctionOne();

  private:
    Real m_fObjectiveFunction;

};

#endif
