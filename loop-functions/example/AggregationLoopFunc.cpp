/**
  * @file <loop-functions/example/AutoMoDeAggregationLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AggregationLoopFunc.h"

/****************************************/
/****************************************/

AggregationLoopFunction::AggregationLoopFunction() {
  m_fRadius = 0.3;
  m_cCoordSpot1 = CVector2(0.5,0);
  m_cCoordSpot2 = CVector2(-0.5,0);
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  m_fObjectiveFunction = 0;
}

/****************************************/
/****************************************/

AggregationLoopFunction::AggregationLoopFunction(const AggregationLoopFunction& orig) {}

/****************************************/
/****************************************/

AggregationLoopFunction::~AggregationLoopFunction() {}

/****************************************/
/****************************************/

void AggregationLoopFunction::Destroy() {}

/****************************************/
/****************************************/

argos::CColor AggregationLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());
  Real d = (m_cCoordSpot1 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  d = (m_cCoordSpot2 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  return CColor::GRAY50;
}

/****************************************/
/****************************************/

void AggregationLoopFunction::PostExperiment() {
  CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
  CVector2 cEpuckPosition(0,0);
  for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
    CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
    cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                       pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

    Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();
    Real fDistanceSpot2 = (m_cCoordSpot2 - cEpuckPosition).Length();
    if (fDistanceSpot1 <= m_fRadius) {
      m_unScoreSpot1 += 1;
    } else if (fDistanceSpot2 <= m_fRadius){
      m_unScoreSpot2 += 1;
    }
  }

  m_fObjectiveFunction = Max(m_unScoreSpot1, m_unScoreSpot2)/(Real) m_unNumberRobots;
}

Real AggregationLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

REGISTER_LOOP_FUNCTIONS(AggregationLoopFunction, "aggregation_loop_functions");
