/**
  * @file <loop-functions/IcraLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "SynMtcLoopFunc.h"

/****************************************/
/****************************************/

SynMtcLoopFunction::SynMtcLoopFunction() {
    m_fObjectiveFunction = 0;
    m_fRandomIndex = 0;
    m_cCoordSpot1 = CVector2(0.54, 0.54);
    m_cCoordSpot2 = CVector2(0.765, 0.00);
    m_cCoordSpot3 = CVector2(0.54,-0.54);
    m_fRadiusSpot = 0.125;
}

/****************************************/
/****************************************/

SynMtcLoopFunction::SynMtcLoopFunction(const SynMtcLoopFunction& orig) {
}

/****************************************/
/****************************************/

SynMtcLoopFunction::~SynMtcLoopFunction() {}

/****************************************/
/****************************************/

void SynMtcLoopFunction::Destroy() {
}

/****************************************/
/****************************************/

argos::CColor SynMtcLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {

    if (c_position_on_plane.GetX() <= -0.375)
        return CColor::WHITE;
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

void SynMtcLoopFunction::Init(TConfigurationNode& t_tree) {
    AutoMoDeLoopFunctions::Init(t_tree);
    m_fRandomIndex = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::Reset() {
    AutoMoDeLoopFunctions::Reset();
    m_fObjectiveFunction = 0;
    m_fRandomIndex = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::PostStep() {
    UInt32 unClock = GetSpace().GetSimulationClock();

    if (unClock == 3)
        m_fObjectiveFunction += GetStepScore(true);

    if (unClock == 400)
        m_fObjectiveFunction += GetStepScore(false);

    if (unClock == 800)
        m_fObjectiveFunction += GetStepScore(false);

    if (unClock == 1200)
        m_fObjectiveFunction += GetStepScore(false);

    ArenaControl(unClock);

}

/****************************************/
/****************************************/

void SynMtcLoopFunction::PostExperiment() {
    LOG << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real SynMtcLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::ArenaControl(UInt32 unClock) {

    if (unClock == 1)
        ArenaControlSelector(unClock);

    if (unClock == 400)
        ArenaControlSelector(unClock);

    if (unClock == 800)
        ArenaControlSelector(unClock);

    return;
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::ArenaControlSelector(UInt32 unClock) {

    Real fSelector = 0.167;

    if (m_fRandomIndex < (fSelector)) {
        if (unClock == 1)
            ArenaConfigOne();
        if (unClock == 400)
            ArenaConfigTwo();
        if (unClock == 800)
            ArenaConfigThree();
        return;
    }

    if (m_fRandomIndex < (2*fSelector)) {
        if (unClock == 1)
            ArenaConfigOne();
        if (unClock == 400)
            ArenaConfigThree();
        if (unClock == 800)
            ArenaConfigTwo();
        return;
    }

    if (m_fRandomIndex < (3*fSelector)) {
        if (unClock == 1)
            ArenaConfigTwo();
        if (unClock == 400)
            ArenaConfigOne();
        if (unClock == 800)
            ArenaConfigThree();
        return;
    }

    if (m_fRandomIndex < (4*fSelector)) {
        if (unClock == 1)
            ArenaConfigTwo();
        if (unClock == 400)
            ArenaConfigThree();
        if (unClock == 800)
            ArenaConfigOne();
        return;
    }

    if (m_fRandomIndex < (5*fSelector)) {
        if (unClock == 1)
            ArenaConfigThree();
        if (unClock == 400)
            ArenaConfigOne();
        if (unClock == 800)
            ArenaConfigTwo();
        return;
    }

    if (m_fRandomIndex < (6*fSelector)) {
        if (unClock == 1)
            ArenaConfigThree();
        if (unClock == 400)
            ArenaConfigTwo();
        if (unClock == 800)
            ArenaConfigOne();
        return;
    }

    return;
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::ArenaConfigOne () {
    m_pcArena->SetArenaColor(CColor::BLACK);
    m_pcArena->SetBoxColor(2,1,CColor::GREEN);
    m_pcArena->SetBoxColor(2,7,CColor::GREEN);
    m_pcArena->SetBoxColor(2,8,CColor::GREEN);
    m_cSynColor = CColor::YELLOW;

    return;
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::ArenaConfigTwo () {
    m_pcArena->SetArenaColor(CColor::BLACK);
    m_pcArena->SetBoxColor(2,1,CColor::BLUE);
    m_pcArena->SetBoxColor(2,7,CColor::BLUE);
    m_pcArena->SetBoxColor(2,8,CColor::BLUE);
    m_cSynColor = CColor::CYAN;

    return;
}

/****************************************/
/****************************************/

void SynMtcLoopFunction::ArenaConfigThree () {
    m_pcArena->SetArenaColor(CColor::BLACK);
    m_pcArena->SetBoxColor(2,1,CColor::RED);
    m_pcArena->SetBoxColor(2,7,CColor::RED);
    m_pcArena->SetBoxColor(2,8,CColor::RED);
    m_cSynColor = CColor::MAGENTA;

    return;
}

/****************************************/
/****************************************/

Real SynMtcLoopFunction::GetStepScore(bool bInit) {

    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CColor cEpuckColor = CColor::BLACK;
    Real fScore = 0;

    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {

        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);

        cEpuckColor = pcEpuck->GetLEDEquippedEntity().GetLED(10).GetColor();

        if (!bInit) {
            if (cEpuckColor == m_cSynColor && m_tMemColors[pcEpuck] != cEpuckColor)
                fScore-=1;

            if (cEpuckColor == m_cSynColor && m_tMemColors[pcEpuck] == cEpuckColor)
                fScore+= (1 - (Real)m_unPwConfig);

            if (cEpuckColor != m_cSynColor && m_tMemColors[pcEpuck] == cEpuckColor)
                fScore+= (0 + (Real)m_unPwConfig);

            if (cEpuckColor != m_cSynColor && m_tMemColors[pcEpuck] == cEpuckColor)
                fScore+=2;
        }
        m_tMemColors[pcEpuck] = cEpuckColor;
    }

    return fScore;
}

/****************************************/
/****************************************/

CVector3 SynMtcLoopFunction::GetRandomPosition() {
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

REGISTER_LOOP_FUNCTIONS(SynMtcLoopFunction, "syn_mtc_loop_function");
