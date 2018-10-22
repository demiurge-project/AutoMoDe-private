/**
  * @file <loop-functions/IcraLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "SeqLoopFunc.h"

/****************************************/
/****************************************/

SeqLoopFunction::SeqLoopFunction() {
    m_fObjectiveFunction = 0;
    m_fObjectiveFunctionBlack = 0;
    m_fObjectiveFunctionRed = 0;
    m_bBlackFirst = true;
    m_cArenaColor = CColor::BLACK;
}

/****************************************/
/****************************************/

SeqLoopFunction::SeqLoopFunction(const SeqLoopFunction& orig) {
}

/****************************************/
/****************************************/

SeqLoopFunction::~SeqLoopFunction() {}

/****************************************/
/****************************************/

void SeqLoopFunction::Destroy() {
    m_tRobotStates.clear();
}

/****************************************/
/****************************************/

argos::CColor SeqLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {

    if (c_position_on_plane.GetX() <= -0.375)
        return CColor::WHITE;

    else if (c_position_on_plane.GetX() >= -0.125 && c_position_on_plane.GetX() <= 0.125){
        if (c_position_on_plane.GetY() >= 0.65033 || c_position_on_plane.GetY() <= -0.65033)
            return CColor::BLACK;
    }

    else if (c_position_on_plane.GetX() >= 0.65033 && c_position_on_plane.GetY() >= -0.125 && c_position_on_plane.GetY() <= 0.125)
        return CColor::BLACK;

    else if (c_position_on_plane.GetY() >= -c_position_on_plane.GetX() + 0.9267766094 &&
             c_position_on_plane.GetY() <=  c_position_on_plane.GetX() + 0.1767766953 &&
             c_position_on_plane.GetY() >=  c_position_on_plane.GetX() - 0.1767766953)
        return CColor::BLACK;

    else if (c_position_on_plane.GetY() <=  c_position_on_plane.GetX() - 0.9267766094 &&
             c_position_on_plane.GetY() <= -c_position_on_plane.GetX() + 0.1767766953 &&
             c_position_on_plane.GetY() >= -c_position_on_plane.GetX() - 0.1767766953)
        return CColor::BLACK;

    return CColor::GRAY50;
}

/****************************************/
/****************************************/

void SeqLoopFunction::Init(TConfigurationNode& t_tree) {
    AutoMoDeLoopFunctions::Init(t_tree);

    m_bBlackFirst = SelectColorOrder(m_unColorOrder);

    InitRobotStates();

}

/****************************************/
/****************************************/

void SeqLoopFunction::Reset() {
    AutoMoDeLoopFunctions::Reset();

    m_fObjectiveFunction = 0;
    m_fObjectiveFunctionBlack = 0;
    m_fObjectiveFunctionRed = 0;

    m_bBlackFirst = SelectColorOrder(m_unColorOrder);

    m_tRobotStates.clear();
    InitRobotStates();
}

/****************************************/
/****************************************/

void SeqLoopFunction::PostStep() {

    UInt32 unClock = GetSpace().GetSimulationClock();

    ScoreControl(unClock);
    ArenaControl(unClock);

    /*
    if (m_unEvalTask == 1)
        LOG << m_fObjectiveFunctionBlack << std::endl;
    else if (m_unEvalTask == 2)
        LOG << m_fObjectiveFunctionRed << std::endl;
    else
        LOG << m_fObjectiveFunction << std::endl;
        */

}

/****************************************/
/****************************************/

void SeqLoopFunction::PostExperiment() {

    if (m_unEvalTask == 1)
        LOG << m_fObjectiveFunctionBlack << std::endl;
    else if (m_unEvalTask == 2)
        LOG << m_fObjectiveFunctionRed << std::endl;
    else
        LOG << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real SeqLoopFunction::GetObjectiveFunction() {

    if (m_unEvalTask == 1)
        return m_fObjectiveFunctionBlack;
    else if (m_unEvalTask == 2)
        return m_fObjectiveFunctionRed;
    else
        return m_fObjectiveFunction;

}

/****************************************/
/****************************************/

void SeqLoopFunction::ArenaControl(UInt32 unClock) {

    if (unClock == 1) {
        if (m_bBlackFirst)
            m_cArenaColor = CColor::BLACK;
        else
            m_cArenaColor = CColor::RED;
    }

    if (unClock == m_unTrnTime) {
        if (m_bBlackFirst)
            m_cArenaColor = CColor::RED;
        else
            m_cArenaColor = CColor::BLACK;
    }

    if (unClock == 1 || unClock == m_unTrnTime) {
        m_pcArena->SetArenaColor(m_cArenaColor);
        m_pcArena->SetBoxColor(2,1,CColor::GREEN);
        m_pcArena->SetBoxColor(2,2,CColor::GREEN);
        m_pcArena->SetBoxColor(1,3,CColor::BLUE);
        m_pcArena->SetBoxColor(3,3,CColor::BLUE);
        m_pcArena->SetBoxColor(2,4,CColor::BLUE);
        m_pcArena->SetBoxColor(1,5,CColor::BLUE);
        m_pcArena->SetBoxColor(3,5,CColor::BLUE);
        m_pcArena->SetBoxColor(2,6,CColor::GREEN);
        m_pcArena->SetBoxColor(2,7,CColor::GREEN);
        m_pcArena->SetBoxColor(2,8,CColor::GREEN);
    }

    return;
}

/****************************************/
/****************************************/

void SeqLoopFunction::ScoreControl(UInt32 unClock){

    if (m_unEvalTask == 1) {
        if (m_bBlackFirst){
            if (unClock <= m_unTrnTime){
                m_fObjectiveFunctionBlack += GetScore(m_unBlackTask);
                //LOG << "Black in Black first" << std::endl;
            }
        }
        else{
            if (unClock > m_unTrnTime){
                m_fObjectiveFunctionBlack += GetScore(m_unBlackTask);
                //LOG << "Black in Black second" << std::endl;
            }
        }
    }

    else if (m_unEvalTask == 2) {
        if (!m_bBlackFirst){
            if (unClock <= m_unTrnTime){
                m_fObjectiveFunctionRed += GetScore(m_unRedTask);
                //LOG << "Red in Red first" << std::endl;
            }
        }
        else{
            if (unClock > m_unTrnTime){
                m_fObjectiveFunctionRed += GetScore(m_unRedTask);
                //LOG << "Red in Red second" << std::endl;
            }
        }
    }

    else {
        m_fObjectiveFunction = 888888;
        m_fObjectiveFunctionBlack = 888888;
        m_fObjectiveFunctionRed = 888888;
    }
}

/****************************************/
/****************************************/

Real SeqLoopFunction::GetScore(UInt32 unTask) {

    Real unScore = 0;

    switch (unTask){
    case 0:
        unScore = GetStopScore();
        break;
    case 1:
        unScore = GetAllBlackScore();
        break;
    case 2:
        unScore = GetForageScore();
        break;
    case 3:
        unScore = GetMimicryScore();
        break;
    case 4:
        unScore = GetDistributeScore();
        break;
    case 5:
        unScore = GetAggregationScore();
        break;
    default:
        unScore = 999999;
        break;
    }

    return unScore;
}

/****************************************/
/****************************************/

Real SeqLoopFunction::GetStopScore() {

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

Real SeqLoopFunction::GetAllBlackScore() {

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

Real SeqLoopFunction::GetForageScore() {

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

Real SeqLoopFunction::GetMimicryScore() {

    UpdateRobotColors();

    Real unScore = 0;
    TRobotStateMap::iterator it;

    for (it = m_tRobotStates.begin(); it != m_tRobotStates.end(); ++it) {

        if (it->second.cColor != CColor::BLACK &&
                m_cArenaColor == CColor::BLACK)
            unScore+=1;

        else if (it->second.cColor != CColor::MAGENTA &&
                 m_cArenaColor == CColor::RED)
            unScore+=1;
    }

    return unScore;
}

/****************************************/
/****************************************/

bool SeqLoopFunction::IsRobotInNest (CVector2 tRobotPosition) {
    if (tRobotPosition.GetX() <= -0.34)
        return true;

    return false;
}

/****************************************/
/****************************************/

bool SeqLoopFunction::IsRobotInSource (CVector2 tRobotPosition){

    if (tRobotPosition.GetX() >= -0.16) {
        if (tRobotPosition.GetX() >= -0.16 && tRobotPosition.GetX() <= 0.16){
            if (tRobotPosition.GetY() >= 0.61533 || tRobotPosition.GetY() <= -0.61533)
                return true;
        }

        else if (tRobotPosition.GetX() >= 0.61533 && tRobotPosition.GetY() >= -0.16 && tRobotPosition.GetY() <= 0.16)
            return true;

        else if (tRobotPosition.GetY() >= -tRobotPosition.GetX() + 0.87727913472 &&
                 tRobotPosition.GetY() <=  tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >=  tRobotPosition.GetX() - 0.22450640303)
            return true;

        else if (tRobotPosition.GetY() <=  tRobotPosition.GetX() - 0.87727913472 &&
                 tRobotPosition.GetY() <= -tRobotPosition.GetX() + 0.22450640303 &&
                 tRobotPosition.GetY() >= -tRobotPosition.GetX() - 0.22450640303)
            return true;
    }

    return false;
}

/****************************************/
/****************************************/

void SeqLoopFunction::UpdateRobotPositions() {
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

void SeqLoopFunction::UpdateRobotColors() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CColor cEpuckColor;
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckColor = pcEpuck->GetLEDEquippedEntity().GetLED(10).GetColor();

        m_tRobotStates[pcEpuck].cColor = cEpuckColor;
    }
}

/****************************************/
/****************************************/

void SeqLoopFunction::InitRobotStates() {

    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                           pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        m_tRobotStates[pcEpuck].cLastPosition = cEpuckPosition;
        m_tRobotStates[pcEpuck].cPosition = cEpuckPosition;
        m_tRobotStates[pcEpuck].cColor = CColor::BLACK;
        m_tRobotStates[pcEpuck].bItem = false;
    }
}

/****************************************/
/****************************************/

bool SeqLoopFunction::SelectColorOrder(UInt32 un_ColorOrderParam) {

    Real fRandomIndex;
    bool bBlackFirst = true;
    if (un_ColorOrderParam == 0)
        fRandomIndex = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
    else{
        fRandomIndex = (un_ColorOrderParam * 0.5) - (0.5/2);
    }

    if (fRandomIndex > 0.5)
        bBlackFirst = false;

    return bBlackFirst;

}

/****************************************/
/****************************************/

CVector3 SeqLoopFunction::GetRandomPosition() {
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

REGISTER_LOOP_FUNCTIONS(SeqLoopFunction, "seq_loop_function");
