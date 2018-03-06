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
    GetNodeAttributeOrDefault(cParametersNode, "number_edges", m_unNumberEdges, (UInt32) 3);
    GetNodeAttributeOrDefault(cParametersNode, "number_boxes_per_edge", m_unNumberBoxes, (UInt32) 1);
    GetNodeAttributeOrDefault(cParametersNode, "lenght_boxes", m_fLenghtBoxes, (Real) 0.20);
  } catch(std::exception e) {
    LOGERR << e.what() << std::endl;
  }

  m_fDistributionRadius = GetArenaRadious();
  PositionArena();
  PositionRobots();
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::Reset() {
  MoveRobots();
}

/****************************************/
/****************************************/

AutoMoDeLoopFunctions::~AutoMoDeLoopFunctions() {}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::PositionRobots() {
  CEPuckEntity* pcEpuck;
  bool bPlaced = false;
  UInt32 unTrials;
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

void AutoMoDeLoopFunctions::MoveRobots() {
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

void AutoMoDeLoopFunctions::RemoveRobots() {
  for(UInt32 i = 1; i < m_unNumberRobots + 1; ++i) {
    std::ostringstream id;
    id << "epuck" << i;
    RemoveEntity(id.str().c_str());
  }
}

/****************************************/
/****************************************/

void AutoMoDeLoopFunctions::PositionArena() {
  CArenaEntity* pcArena;
    pcArena = new CArenaEntity("arena",
                               CVector3(0,0,0),
                               CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO), // TODO
                               CVector3(0.01,m_fLenghtBoxes,0.08),
                               "leds",
                               m_unNumberBoxes,
                               m_unNumberEdges,
                               0.017f,
                               1.0f);   
    AddEntity(*pcArena);
    m_pcArena = pcArena;
}

/****************************************/
/****************************************/

Real AutoMoDeLoopFunctions::GetArenaRadious() {

    Real fRadious;
    fRadious =  (m_fLenghtBoxes*m_unNumberBoxes) / (2 * Tan(CRadians::PI / m_unNumberEdges));
    fRadious = fRadious - 0.10; // Avoids to place robots close the walls.

    return fRadious;
}

/****************************************/
/****************************************/

bool AutoMoDeLoopFunctions::IsEven(UInt32 unNumber) {
    bool even;
    if((unNumber%2)==0)
       even = true;
    else
       even = false;

    return even;
}

/****************************************/
/****************************************/






