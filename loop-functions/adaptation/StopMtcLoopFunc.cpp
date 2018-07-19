/**
  * @file <loop-functions/IcraLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "StopMtcLoopFunc.h"

/****************************************/
/****************************************/

StopMtcLoopFunction::StopMtcLoopFunction() {
    m_fObjectiveFunction = 0;
    m_fRandomIndex = 0;
    m_cCoordSpot1 = CVector2(0.54, 0.54);
    m_cCoordSpot2 = CVector2(0.54, 0.00);
    m_cCoordSpot3 = CVector2(0.54,-0.54);
    m_fSafeDist = 0.16;
    m_fRadiusSpot = 0.125;
    m_bStopSignal = false;
    m_bInit = false;
}

/****************************************/
/****************************************/

StopMtcLoopFunction::StopMtcLoopFunction(const StopMtcLoopFunction& orig) {
}

/****************************************/
/****************************************/

StopMtcLoopFunction::~StopMtcLoopFunction() {}

/****************************************/
/****************************************/

void StopMtcLoopFunction::Destroy() {
}

/****************************************/
/****************************************/

argos::CColor StopMtcLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {

    if (c_position_on_plane.GetX() <= -0.375)
        return CColor::BLACK;
    else {
        Real d = (m_cCoordSpot1 - c_position_on_plane).Length();
        if ( d <= m_fRadiusSpot)
            return CColor::WHITE;
        else{
            d = (m_cCoordSpot2 - c_position_on_plane).Length();
            if (d <= m_fRadiusSpot)
                return CColor::WHITE;
            else {
                d = (m_cCoordSpot3 - c_position_on_plane).Length();
                if (d <= m_fRadiusSpot)
                    return CColor::WHITE;
            }
        }
    }

    return CColor::GRAY50;
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::Init(TConfigurationNode& t_tree) {
    AutoMoDeLoopFunctions::Init(t_tree);
    m_fRandomIndex = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::Reset() {
    AutoMoDeLoopFunctions::Reset();
    m_fObjectiveFunction = 0;
    m_bInit = false;
    m_bStopSignal = false;
    m_fRandomIndex = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::PostStep() {
    UInt32 unClock = GetSpace().GetSimulationClock();
    ArenaControl(unClock);
    m_fObjectiveFunction += GetStepScore(unClock);
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::PostExperiment() {

    if (!m_bStopSignal)
        m_unStopTime = 1800;

    Real fTimeScore = (Real)m_unStopTime * (Real)m_unNumberRobots;
    m_fObjectiveFunction += fTimeScore;

    LOG << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real StopMtcLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::ArenaControl(UInt32 unClock) {

    if (unClock == 1 && m_fRandomIndex <= 0.5)
        ArenaConfigOne();

    if (unClock == m_unPwTime && m_fRandomIndex <= 0.5)
        ArenaConfigTwo();

    if (unClock == 1 && m_fRandomIndex > 0.5)
        ArenaConfigTwo();

    if (unClock == m_unPwTime && m_fRandomIndex > 0.5)
        ArenaConfigOne();

    return;
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::ArenaConfigOne () {
    m_pcArena->SetArenaColor(CColor::BLACK);
    m_pcArena->SetWallColor(1,CColor::GREEN);
    m_cStopColor = CColor::YELLOW;

    return;
}

/****************************************/
/****************************************/

void StopMtcLoopFunction::ArenaConfigTwo () {
    m_pcArena->SetArenaColor(CColor::BLACK);
    m_pcArena->SetWallColor(7,CColor::BLUE);
    m_cStopColor = CColor::CYAN;

    return;
}

/****************************************/
/****************************************/

Real StopMtcLoopFunction::GetStepScore(UInt32 unClock) {

    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    CColor cEpuckColor = CColor::BLACK;
    Real fScore = 0;
    Real d1, d2, dm;

    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {

        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);

        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                           pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        if (m_bInit) {

            dm = (cEpuckPosition - m_tMemPositions[pcEpuck]).Length();

            if (!m_bStopSignal) {

                cEpuckColor = pcEpuck->GetLEDEquippedEntity().GetLED(10).GetColor();
                d1 = (cEpuckPosition - m_cCoordSpot1).Length();
                d2 = (cEpuckPosition - m_cCoordSpot1).Length();

                if (dm <= 0.0005)
                    fScore+=1;

                if ( d1 <= m_fSafeDist || d2 <= m_fSafeDist)
                    if (cEpuckColor == m_cStopColor) {
                        m_bStopSignal = true;
                        m_unStopTime = unClock;
                    }
            }
            else
                if (dm > 0.0005)
                    fScore+=1;
        }
        else
            m_bInit = true;

        m_tMemPositions[pcEpuck] = cEpuckPosition;
    }

    return fScore;
}

/****************************************/
/****************************************/

CVector3 StopMtcLoopFunction::GetRandomPosition() {
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

  return CVector3((fPosX * 0.8) -0.60 , fPosY*2, 0);
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(StopMtcLoopFunction, "stop_mtc_loop_function");
