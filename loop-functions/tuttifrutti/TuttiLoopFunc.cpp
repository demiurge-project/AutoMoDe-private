/**
  * @file <loop-functions/IcraLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "TuttiLoopFunc.h"

/****************************************/
/****************************************/

TuttiLoopFunction::TuttiLoopFunction() {
    m_unClock = 0;
    m_unStopTime = 0;
    m_fObjectiveFunction = 0;
}

/****************************************/
/****************************************/

TuttiLoopFunction::TuttiLoopFunction(const TuttiLoopFunction& orig) {
}

/****************************************/
/****************************************/

TuttiLoopFunction::~TuttiLoopFunction() {}

/****************************************/
/****************************************/

void TuttiLoopFunction::Destroy() {

    m_tRobotStates.clear();
}

/****************************************/
/****************************************/

void TuttiLoopFunction::Init(TConfigurationNode& t_tree) {

    AutoMoDeLoopFunctions::Init(t_tree);
    InitRobotStates();

}

/****************************************/
/****************************************/

void TuttiLoopFunction::Reset() {
    AutoMoDeLoopFunctions::Reset();

    m_unClock = 0;
    m_unStopTime = 0;
    m_fObjectiveFunction = 0;

    m_tRobotStates.clear();

    InitRobotStates();
}

/****************************************/
/****************************************/

void TuttiLoopFunction::PostStep() {

    m_unClock = GetSpace().GetSimulationClock();

    ScoreControl();
    ArenaControl();

}

/****************************************/
/****************************************/

void TuttiLoopFunction::PostExperiment() {
    LOG << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real TuttiLoopFunction::GetObjectiveFunction() {
    return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

void TuttiLoopFunction::ArenaControl() {

    if (m_unClock == 1) {

        if (m_unEvalTask == 0) {
            m_pcArena->SetArenaColor(CColor::BLACK);
            m_unStopTime = GetRandomTime(400, 600);
        }
        if (m_unEvalTask == 1) {
            m_pcArena->SetArenaColor(CColor::BLACK);
        }
        if (m_unEvalTask == 2) {
            m_pcArena->SetArenaColor(CColor::BLACK);
        }
    }
    else if (m_unEvalTask == 0 && m_unClock == m_unStopTime)
    {
        m_pcArena->SetBoxColor(2,2,CColor::RED);
    }


    return;
}

/****************************************/
/****************************************/

void TuttiLoopFunction::ScoreControl(){

     if (m_unEvalTask < 3)
         m_fObjectiveFunction += GetScore(m_unEvalTask);
    else {
        m_fObjectiveFunction = 888888;
    }
}

/****************************************/
/****************************************/

Real TuttiLoopFunction::GetScore(UInt32 unTask) {

    Real unScore = 0;

    switch (unTask){
    case 0:
        unScore = GetStopScore();
        break;
    case 1:
        unScore = GetAllBlackScore();
        break;
    case 2:
        unScore = -GetForageScore();
        break;
    default:
        unScore = 999999;
        break;
    }

    return unScore;
}

/****************************************/
/****************************************/

Real TuttiLoopFunction::GetStopScore() {

    UpdateRobotPositions();

    Real unScore = 0;
    TRobotStateMap::iterator it;
    for (it = m_tRobotStates.begin(); it != m_tRobotStates.end(); ++it) {
        Real d = (it->second.cPosition - it->second.cLastPosition).Length();
        if (d > 0.0005)
            unScore+=1;
    }

    return unScore;
}

/****************************************/
/****************************************/

Real TuttiLoopFunction::GetAllBlackScore() {

    UpdateRobotPositions();

    bool bInSource;
    Real unScore = 0;
    TRobotStateMap::iterator it;
    for (it = m_tRobotStates.begin(); it != m_tRobotStates.end(); ++it) {
        bInSource = IsRobotInSource(it->second.cPosition);
        if (!bInSource)
            unScore+=1;
    }

    return unScore;
}

/****************************************/
/****************************************/

Real TuttiLoopFunction::GetForageScore() {

    UpdateRobotPositions();

    bool bInNest;
    bool bInSource;
    Real unScore = 0;
    TRobotStateMap::iterator it;

    for (it = m_tRobotStates.begin(); it != m_tRobotStates.end(); ++it) {

        if (it->second.bItem == true){
            bInNest = IsRobotInNest(it->second.cPosition);
            if (bInNest) {
                unScore+=1;
                it->second.bItem = false;
            }
        }
        else {
            bInSource = IsRobotInSource(it->second.cPosition);
            if (bInSource) {
                it->second.bItem = true;
            }
        }
    }

    return unScore;
}

/****************************************/
/****************************************/

argos::CColor TuttiLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {

    if (c_position_on_plane.GetX() >= -0.125 && c_position_on_plane.GetX() <= 0.125){
        if (c_position_on_plane.GetY() >= 0.65033 || c_position_on_plane.GetY() <= -0.65033)
            return CColor::BLACK;
        else if (c_position_on_plane.GetY() >= -0.125 && c_position_on_plane.GetY() <= 0.125)
            return CColor::WHITE;
    }

    else if (c_position_on_plane.GetY() >= -0.125 && c_position_on_plane.GetY() <= 0.125){
        if (c_position_on_plane.GetX() >= 0.65033 || c_position_on_plane.GetX() <= -0.65033)
            return CColor::BLACK;
    }
    else if (c_position_on_plane.GetY() <=  c_position_on_plane.GetX() + 0.1767766953 &&
             c_position_on_plane.GetY() >=  c_position_on_plane.GetX() - 0.1767766953){
        if ( c_position_on_plane.GetY() >= -c_position_on_plane.GetX() + 0.9267766094 ||
             c_position_on_plane.GetY() <= -c_position_on_plane.GetX() - 0.9267766094)
            return CColor::BLACK;
    }

    else if (c_position_on_plane.GetY() <= -c_position_on_plane.GetX() + 0.1767766953 &&
             c_position_on_plane.GetY() >= -c_position_on_plane.GetX() - 0.1767766953){
        if ( c_position_on_plane.GetY() >=  c_position_on_plane.GetX() + 0.9267766094 ||
             c_position_on_plane.GetY() <=  c_position_on_plane.GetX() - 0.9267766094)
            return CColor::BLACK;
    }

    return CColor::GRAY50;
}

/****************************************/
/****************************************/

bool TuttiLoopFunction::IsRobotInNest (CVector2 tRobotPosition) {

    if (tRobotPosition.GetX() >= -0.16 && tRobotPosition.GetX() <= 0.16 &&
        tRobotPosition.GetY() >= -0.16 && tRobotPosition.GetY() <= 0.16)
        return true;

    return false;
}


/****************************************/
/****************************************/

bool TuttiLoopFunction::IsRobotInSource (CVector2 tRobotPosition){

    if (tRobotPosition.Length() >= 0.6) {

        if (tRobotPosition.GetX() >= -0.16 && tRobotPosition.GetX() <= 0.16){
            if (tRobotPosition.GetY() >= 0.61533)
                return true;
            else if (tRobotPosition.GetY() <= -0.61533)
                return true;
        }
        else if (tRobotPosition.GetY() >= -0.16 && tRobotPosition.GetY() <= 0.16){
            if (tRobotPosition.GetX() >= 0.61533)
                return true;
            else if (tRobotPosition.GetX() <= -0.61533)
                return true;
        }
        else if (tRobotPosition.GetY() <=  tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >=  tRobotPosition.GetX() - 0.22450640303){
            if ( tRobotPosition.GetY() >= -tRobotPosition.GetX() + 0.87727913472)
                return true;
            else if (tRobotPosition.GetY() <= -tRobotPosition.GetX() - 0.87727913472)
                return true;
        }

        else if (tRobotPosition.GetY() <= -tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >= -tRobotPosition.GetX() - 0.22450640303){
            if ( tRobotPosition.GetY() >=  tRobotPosition.GetX() + 0.87727913472)
                return true;
            else if (tRobotPosition.GetY() <=  tRobotPosition.GetX() - 0.87727913472)
                return true;
        }
    }

    return false;
}

/****************************************/
/****************************************/

UInt32 TuttiLoopFunction::IsRobotInSourceID (CVector2 tRobotPosition){

    UInt32 unSourceId = 0;

    if (tRobotPosition.Length() >= 0.6) {

        if (tRobotPosition.GetX() >= -0.16 && tRobotPosition.GetX() <= 0.16){
            if (tRobotPosition.GetY() >= 0.61533)
                unSourceId = 2;
            else if (tRobotPosition.GetY() <= -0.61533)
                unSourceId = 6;
        }
        else if (tRobotPosition.GetY() >= -0.16 && tRobotPosition.GetY() <= 0.16){
            if (tRobotPosition.GetX() >= 0.61533)
                unSourceId = 8;
            else if (tRobotPosition.GetX() <= -0.61533)
                unSourceId = 4;
        }
        else if (tRobotPosition.GetY() <=  tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >=  tRobotPosition.GetX() - 0.22450640303){
            if ( tRobotPosition.GetY() >= -tRobotPosition.GetX() + 0.87727913472)
                unSourceId = 1;
            else if (tRobotPosition.GetY() <= -tRobotPosition.GetX() - 0.87727913472)
                unSourceId = 5;
        }

        else if (tRobotPosition.GetY() <= -tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >= -tRobotPosition.GetX() - 0.22450640303){
            if ( tRobotPosition.GetY() >=  tRobotPosition.GetX() + 0.87727913472)
                unSourceId = 3;
            else if (tRobotPosition.GetY() <=  tRobotPosition.GetX() - 0.87727913472)
                unSourceId = 7;
        }
    }

    return unSourceId;
}

/****************************************/
/****************************************/

void TuttiLoopFunction::UpdateRobotPositions() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                           pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        m_tRobotStates[pcEpuck].cLastPosition = m_tRobotStates[pcEpuck].cPosition;
        m_tRobotStates[pcEpuck].cPosition = cEpuckPosition;
    }
}

/****************************************/
/****************************************/

void TuttiLoopFunction::InitRobotStates() {

    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                           pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        m_tRobotStates[pcEpuck].cLastPosition = cEpuckPosition;
        m_tRobotStates[pcEpuck].cPosition = cEpuckPosition;
        m_tRobotStates[pcEpuck].bItem = false;
    }
}

/****************************************/
/****************************************/

CVector3 TuttiLoopFunction::GetRandomPosition() {
  Real temp;
  Real a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  Real b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  Real c = m_pcRng->Uniform(CRange<Real>(-1.0f, 1.0f));
  Real d = m_pcRng->Uniform(CRange<Real>(-1.0f, 1.0f));
  // If b < a, swap them
  if (b < a) {
    temp = a;
    a = b;
    b = temp;
  }
  m_fDistributionRadius = 0.4;
  Real fPosX = (c * m_fDistributionRadius / 2) + m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
  Real fPosY = (d * m_fDistributionRadius / 2) + m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));

  return CVector3(fPosX, fPosY, 0);
}

/****************************************/
/****************************************/

UInt32 TuttiLoopFunction::GetRandomTime(UInt32 unMin, UInt32 unMax) {
  UInt32 unFailureAt = m_pcRng->Uniform(CRange<UInt32>(unMin, unMax));
  return unFailureAt;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(TuttiLoopFunction, "tutti_loop_function");
