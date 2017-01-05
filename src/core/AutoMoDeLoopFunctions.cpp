/*
 * @file <src/core/AutoMoDeLoopFunctions.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */


#include "AutoMoDeLoopFunctions.h"

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::Init(argos::TConfigurationNode& t_tree) {
  m_pcRng = CRandom::CreateRNG("argos");
  TConfigurationNode cParametersNode;
  try {
    cParametersNode = GetNode(t_tree, "params");
    GetNodeAttributeOrDefault(cParametersNode, "number_robots", m_unNumberRobots, (UInt32) 1);
    GetNodeAttributeOrDefault(cParametersNode, "dist_radius", m_fDistributionRadius, (Real) 0);
  } catch(std::exception e) {
    LOGERR << e.what() << std::endl;
  }
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::Reset() {
  RemoveRobots();
  PositionRobots();
}

/****************************************/
/****************************************/

AutoMoDeLoopFunctions::~AutoMoDeLoopFunctions() {}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::PositionRobots() {
  Real a;
  Real b;
  Real temp;

  CEPuckEntity* pcEpuck;
  UInt32 unTrials;
  bool bPlaced = false;

  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    pcEpuck = new CEPuckEntity(id.str().c_str(),
                               "automode",
                               CVector3(0,0,0),
                               CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
    AddEntity(*pcEpuck);
    // Choose position at random
    unTrials = 0;
    do {
       ++unTrials;
       a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
       b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
       // If b < a, swap them
       if (b < a) {
         temp = a;
         a = b;
         b = temp;
       }
       Real fPosX = b * m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
       Real fPosY = b * m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));
       bPlaced = MoveEntity((*pcEpuck).GetEmbodiedEntity(),
                            CVector3(fPosX, fPosY, 0),
                            CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
                            CRadians::ZERO,CRadians::ZERO),false);
    } while(!bPlaced && unTrials < 100);
    if(!bPlaced) {
       THROW_ARGOSEXCEPTION("Can't place robot #" << i);
    }
  }
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::RemoveRobots() {
  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    RemoveEntity(id.str().c_str());
  }
}
