/**
  * @file <loop-functions/PwStopLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "PwLoopFunc.h"

/****************************************/
/****************************************/

PwLoopFunction::PwLoopFunction() {
  m_fObjectiveFunction = 0;

}

/****************************************/
/****************************************/

PwLoopFunction::PwLoopFunction(const PwLoopFunction& orig) {
}

/****************************************/
/****************************************/

PwLoopFunction::~PwLoopFunction() {}

/****************************************/
/****************************************/

void PwLoopFunction::Destroy() {
}

/****************************************/
/****************************************/

argos::CColor PwLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  return CColor::GRAY50;
}


/****************************************/
/****************************************/

void PwLoopFunction::Reset() {
  m_fObjectiveFunction = 0;
  m_tPositions.clear();
  m_tMemPositions.clear();
  AutoMoDeLoopFunctions::Reset();
}

/****************************************/
/****************************************/

void PwLoopFunction::PostStep() {
    UInt32 unClock = GetSpace().GetSimulationClock();

    ArenaControl(unClock);
    GetRobotPositions();
    m_fObjectiveFunction += GetMissionScore(unClock);

    LOG << "Score: " << m_fObjectiveFunction << std::endl; // TODO Remove for Opt
}

/****************************************/
/****************************************/

void PwLoopFunction::PostExperiment() {

  LOG << "End Score: " << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real PwLoopFunction::GetObjectiveFunction() {

  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

void PwLoopFunction::ArenaControl(UInt32 unClock) {

    if (unClock == 1 && m_unPwConfig == 0)
        m_pcArena->SetArenaColor(CColor::GREEN);

    if (unClock == m_unPwTime && m_unPwConfig == 0)
        m_pcArena->SetArenaColor(CColor::BLUE);

    if (unClock == 1 && m_unPwConfig == 1)
        m_pcArena->SetArenaColor(CColor::BLUE);

    if (unClock == m_unPwTime && m_unPwConfig == 1)
        m_pcArena->SetArenaColor(CColor::GREEN);


    return;
}

/****************************************/
/****************************************/

Real PwLoopFunction::GetMissionScore(UInt32 unClock){

    UInt32 unScore;

    if (m_unPwExp == 0 && m_unPwConfig == 0){
        unScore = PwFunctionMove(unClock,1,m_unPwTime,false);
        unScore = PwFunctionStop(unClock,m_unPwTime,(2*m_unPwTime));
        return unScore;
    }

    if (m_unPwExp == 0 && m_unPwConfig == 1){
        unScore = PwFunctionStop(unClock,1,m_unPwTime);
        unScore = PwFunctionMove(unClock,m_unPwTime,(2*m_unPwTime),false);
        return unScore;
    }

    if (m_unPwExp == 1 && m_unPwConfig == 0){
        unScore = PwFunctionAgg(unClock,1,m_unPwTime,false);
        unScore = PwFunctionAgg(unClock,m_unPwTime,(2*m_unPwTime),true);
        return unScore;
    }

    if (m_unPwExp == 1 && m_unPwConfig == 1){
        unScore = PwFunctionAgg(unClock,1,m_unPwTime,true);
        unScore = PwFunctionAgg(unClock,m_unPwTime,(2*m_unPwTime),false);
        return unScore;
    }

    if (m_unPwExp == 2 && m_unPwConfig == 0){
        unScore = PwFunctionMove(unClock,1,m_unPwTime,true);
        unScore = PwFunctionAgg(unClock,m_unPwTime,(2*m_unPwTime),false);
        return unScore;
    }

    if (m_unPwExp == 2 && m_unPwConfig == 1){
        unScore = PwFunctionAgg(unClock,1,m_unPwTime,false);
        unScore = PwFunctionMove(unClock,m_unPwTime,(2*m_unPwTime),true);
        return unScore;
    }

    if (m_unPwExp == 3 && m_unPwConfig == 0){
        unScore = PwFunctionAgg(unClock,1,m_unPwTime,false);
        unScore = PwFunctionFlee(unClock,m_unPwTime,(2*m_unPwTime));
        return unScore;
    }

    if (m_unPwExp == 3 && m_unPwConfig == 1){
        unScore = PwFunctionFlee(unClock,1,m_unPwTime);
        unScore = PwFunctionAgg(unClock,m_unPwTime,(2*m_unPwTime),false);
        return unScore;
    }



    return 0;
}

/****************************************/
/****************************************/

void PwLoopFunction::GetRobotPositions() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                           pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        m_tPositions[pcEpuck] = cEpuckPosition;
    }
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionStop(UInt32 unClock, UInt32 unInitTime, UInt32 unEndTime) {

    if (unClock >= unInitTime && unClock <= unEndTime){
        UInt32 unScore;
        for (TPosMap::iterator it = m_tPositions.begin(); it != m_tPositions.end(); ++it) {


        }
        return 0;
    }
    else
        return 0;

    return 0;
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionMove(UInt32 unClock, UInt32 unInitTime, UInt32 unEndTime, bool bCheckColor) {
    return 0;
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionAgg(UInt32 unClock, UInt32 unInitTime, UInt32 unEndTime, bool bCheckColor) {
    return 0;
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionFlee(UInt32 unClock, UInt32 unInitTime, UInt32 unEndTime) {
    return 0;
}

/****************************************/
/****************************************/

CVector3 PwLoopFunction::GetRandomPosition() {
  Real temp;
  Real a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  Real b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  // If b < a, swap them
  if (b < a) {
    temp = a;
    a = b;
    b = temp;
  }
  Real fPosX = b * m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
  Real fPosY = b * m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));

  return CVector3(fPosX, fPosY, 0);
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(PwLoopFunction, "pw_loop_function");
