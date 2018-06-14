/**
  * @file <loop-functions/example/AggregationLoopFunc.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AGGREGATION_TWO_SPOTS_LOOP_FUNC
#define AGGREGATION_TWO_SPOTS_LOOP_FUNC

#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include "../../src/core/AutoMoDeLoopFunctions.h"


using namespace argos;

class EndtimeDiffLoopFunction: public AutoMoDeLoopFunctions {
  public:
    EndtimeDiffLoopFunction();
    EndtimeDiffLoopFunction(const EndtimeDiffLoopFunction& orig);
    virtual ~EndtimeDiffLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void Reset();

    Real GetObjectiveFunction();

    CVector3 GetRandomPosition();

  private:
    Real m_fRadius;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;

    int m_unScoreSpot1;
    int m_unScoreSpot2;
    int m_fObjectiveFunction;

};

#endif
