/**
  * @file <loop-functions/example/PwLoopFunc.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef SYN_MTC_LOOP_FUNC
#define SYN_MTC_LOOP_FUNC

#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include "../../src/core/AutoMoDeLoopFunctions.h"

using namespace argos;

class SynMtcLoopFunction: public AutoMoDeLoopFunctions {
  public:
    SynMtcLoopFunction();
    SynMtcLoopFunction(const SynMtcLoopFunction& orig);
    virtual ~SynMtcLoopFunction();

    virtual void Destroy();

    virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);
    virtual void PostExperiment();
    virtual void PostStep();
    virtual void Reset();
    virtual void Init(TConfigurationNode& t_tree);

    Real GetObjectiveFunction();

    CVector3 GetRandomPosition();

    Real GetStepScore();

    void ArenaControl(UInt32 unClock);
    void ArenaControlSelector(UInt32 unClock);
    void ArenaConfigOne();
    void ArenaConfigTwo();
    void ArenaConfigThree();


  private:
    CVector2 m_cCoordSpot1;
    CVector2 m_cCoordSpot2;
    CVector2 m_cCoordSpot3;
    Real m_fRadiusSpot;
    Real m_fObjectiveFunction;
    Real m_fRandomIndex;    
    CColor m_cSynColor;

};

#endif
