#ifndef GIANDUJA_NESTING_HIDE_LOOP_FUNC
#define GIANDUJA_NESTING_HIDE_LOOP_FUNC

#include "../src/core/AutoMoDeLoopFunctions.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

using namespace argos;

class GiandujaNestingHideLoopFunction: public AutoMoDeLoopFunctions {
  public:
    GiandujaNestingHideLoopFunction();
    GiandujaNestingHideLoopFunction(const GiandujaNestingHideLoopFunction& orig);
    virtual ~GiandujaNestingHideLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void PostStep();
    virtual void Init(TConfigurationNode& t_tree);
    void PositionRobots();

    Real GetObjectiveFunction();
  private:
    Real m_fRadius;
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;
    CVector2 m_CCoordRect1;
    CVector2 m_CCoordRect2;
    CVector2 m_CCoordRect2Pos[4];
    CVector2 m_CCoordRect1Pos[4];
    CVector2 m_cCoordRectSpot;
    Real m_cCoordRectSize;
    UInt32 m_unTBar;
    UInt32 m_unTime;
    UInt8 m_unStart;

    UInt8 m_unState;
    UInt32 m_unCostI;
    UInt32 m_unCostO;
    Real m_fObjectiveFunction;
};

#endif
