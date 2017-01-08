#ifndef GIANDUJA_AGGREGATION_LOOP_FUNC
#define GIANDUJA_AGGREGATION_LOOP_FUNC

#include "../src/core/AutoMoDeLoopFunctions.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
#include <argos3/plugins/simulator/entities/light_entity.h>

using namespace argos;

class GiandujaAggregationLoopFunction: public AutoMoDeLoopFunctions {
  public:
    GiandujaAggregationLoopFunction();
    GiandujaAggregationLoopFunction(const GiandujaAggregationLoopFunction& orig);
    virtual ~GiandujaAggregationLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void PostStep();
    void PositionRobots();

    Real GetObjectiveFunction();
  private:
    Real m_fRadius;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;

    UInt32 m_unCostSpot1;
    Real m_fObjectiveFunction;
};

#endif
