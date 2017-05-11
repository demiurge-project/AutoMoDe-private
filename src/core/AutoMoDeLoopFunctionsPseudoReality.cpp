/*
 * @file <src/core/AutoMoDeLoopFunctions.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */


#include "AutoMoDeLoopFunctionsPseudoReality.h"

/****************************************/
/****************************************/

void AutoMoDeLoopFunctionsPseudoReality::Init(argos::TConfigurationNode& t_tree) {
  m_pcRng = CRandom::CreateRNG("argos");
  TConfigurationNode cParametersNode;
  try {
    cParametersNode = GetNode(t_tree, "params");
    GetNodeAttributeOrDefault(cParametersNode, "number_robots", m_unNumberRobots, (UInt32) 1);
    GetNodeAttributeOrDefault(cParametersNode, "dist_radius", m_fDistributionRadius, (Real) 0);
  } catch(std::exception e) {
    LOGERR << e.what() << std::endl;
  }

  PositionRobots();
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctionsPseudoReality::Reset() {
  MoveRobots();
}

/****************************************/
/****************************************/

AutoMoDeLoopFunctionsPseudoReality::~AutoMoDeLoopFunctionsPseudoReality() {}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctionsPseudoReality::PositionRobots() {
  CEPuckEntity* pcEpuck;
  bool bPlaced = false;
  UInt32 unTrials;
  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    pcEpuck = new CEPuckEntity(id.str().c_str(),
                               "automode_pr",
                               CVector3(0,0,0),
                               CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
    AddEntity(*pcEpuck);
    // Choose position at random
    unTrials = 0;
    do {
       ++unTrials;
       CVector3 cEpuckPosition = GetRandomPosition();
       bPlaced = MoveEntity((*pcEpuck).GetEmbodiedEntity(),
                            cEpuckPosition,
                            CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
                            CRadians::ZERO,CRadians::ZERO),false);
    } while(!bPlaced && unTrials < 100);
    if(!bPlaced) {
       THROW_ARGOSEXCEPTION("Can't place robot");
    }
  }
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctionsPseudoReality::MoveRobots() {
  CEPuckEntity* pcEpuck;
  bool bPlaced = false;
  UInt32 unTrials;
  CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
  for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
    pcEpuck = any_cast<CEPuckEntity*>(it->second);
    // Choose position at random
    unTrials = 0;
    do {
       ++unTrials;
       CVector3 cEpuckPosition = GetRandomPosition();
       bPlaced = MoveEntity(pcEpuck->GetEmbodiedEntity(),
                            cEpuckPosition,
                            CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
                            CRadians::ZERO,CRadians::ZERO),false);
    } while(!bPlaced && unTrials < 100);
    if(!bPlaced) {
       THROW_ARGOSEXCEPTION("Can't place robot");
    }
  }
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctionsPseudoReality::RemoveRobots() {
  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    RemoveEntity(id.str().c_str());
  }
}
