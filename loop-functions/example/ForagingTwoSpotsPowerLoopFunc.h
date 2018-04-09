/**
  * @file <loop-functions/example/ForagingPowerLoopFunc.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef FORAGING_TWO_SPOTS_LOOP_FUNC
#define FORAGING_TWO_SPOTS_LOOP_FUNC

#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include "../../src/core/AutoMoDeLoopFunctions.h"

using namespace argos;

class ForagingTwoSpotsPowerLoopFunction: public AutoMoDeLoopFunctions {
  public:
    ForagingTwoSpotsPowerLoopFunction();
    ForagingTwoSpotsPowerLoopFunction(const ForagingTwoSpotsPowerLoopFunction& orig);
    virtual ~ForagingTwoSpotsPowerLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostStep();
    virtual void Reset();

    Real GetObjectiveFunction();

    CVector3 GetRandomPosition();

    virtual bool IsExperimentFinished();

  private:
    Real m_fRadius;
    Real m_fNestLimit;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;

    bool m_bInitializationStep;

    Real m_fObjectiveFunction;

    UInt32 * m_punFoodData;

    UInt32 m_unExpSteps; //exptest Steps

    bool m_bExpFinished;

};

#endif
