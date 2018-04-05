/**
  * @file <loop-functions/example/AggregationPowerLoopFunc.h>
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

class AggregationTwoSpotsPowerLoopFunction: public AutoMoDeLoopFunctions {
  public:
    AggregationTwoSpotsPowerLoopFunction();
    AggregationTwoSpotsPowerLoopFunction(const AggregationTwoSpotsPowerLoopFunction& orig);
    virtual ~AggregationTwoSpotsPowerLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void Reset();
    void ExpStep();

    Real GetObjectiveFunction();

    CVector3 GetRandomPosition();

    virtual void PostStep();

    virtual bool IsExperimentFinished();

  private:
    Real m_fRadius;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;

    UInt32 m_unScoreSpot1;
    UInt32 m_unScoreSpot2;
    Real m_fObjectiveFunction;

    UInt32 m_unExpSteps; //Experiment Steps

    bool m_bExpFinished;

};

#endif
