/**
  * @file <loop-functions/example/AggregationTwoSpotsFarFunc.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef EXAMPLE_AGGREGATION_LOOP_FUNC
#define EXAMPLE_AGGREGATION_LOOP_FUNC

#include "../../src/core/AutoMoDeLoopFunctions.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

using namespace argos;

class AggregationTwoSpotsFarFunction: public AutoMoDeLoopFunctions {
  public:
    AggregationTwoSpotsFarFunction();
    AggregationTwoSpotsFarFunction(const AggregationTwoSpotsFarFunction& orig);
    virtual ~AggregationTwoSpotsFarFunction();

    virtual void Destroy();
    virtual void Reset();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();

    Real GetObjectiveFunction();

    /*
     * Returns a vector containing a random position inside a circle of radius
     * m_fDistributionRadius and centered in (0,0).
     */
    virtual CVector3 GetRandomPosition();

  private:
    Real m_fRadius;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;

    UInt32 m_unScoreSpot1;
    UInt32 m_unScoreSpot2;
    Real m_fObjectiveFunction;
};

#endif
