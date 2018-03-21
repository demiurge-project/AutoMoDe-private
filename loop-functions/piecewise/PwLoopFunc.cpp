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
  AutoMoDeLoopFunctions::Reset();
}

/****************************************/
/****************************************/

void PwLoopFunction::PostStep() {
    ArenaControl();

    UInt32 unClock = GetSpace().GetSimulationClock();
    if (m_unPwConfig == 0 && unClock < 1200  )
        m_fObjectiveFunction+=PwFunctionZero();

    if (m_unPwConfig == 0 && unClock >= 1200  )
        m_fObjectiveFunction+=PwFunctionOne();

    if (m_unPwConfig == 1 && unClock < 1200  )
        m_fObjectiveFunction+=PwFunctionOne();

    if (m_unPwConfig == 1 && unClock >= 1200  )
        m_fObjectiveFunction+=PwFunctionZero();

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

void PwLoopFunction::ArenaControl() {
    UInt32 unClock = GetSpace().GetSimulationClock();

    if (unClock == 1 && m_unPwConfig == 0)
        m_pcArena->SetArenaColor(CColor::GREEN);

    if (unClock == 1200 && m_unPwConfig == 0)
        m_pcArena->SetArenaColor(CColor::RED);

    if (unClock == 1 && m_unPwConfig == 1)
        m_pcArena->SetArenaColor(CColor::RED);

    if (unClock == 1200 && m_unPwConfig == 1)
        m_pcArena->SetArenaColor(CColor::GREEN);


    return;
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionZero() {
    return 0;
}

/****************************************/
/****************************************/

Real PwLoopFunction::PwFunctionOne() {
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
