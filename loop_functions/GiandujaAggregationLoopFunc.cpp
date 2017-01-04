#include "GiandujaAggregationLoopFunc.h"

/****************************************/
/****************************************/

GiandujaAggregationLoopFunction::GiandujaAggregationLoopFunction() {
  m_fRadius = 0.2;
  m_cCoordSpot1 = CVector2(0.5,-0.3);
  m_cCoordSpot2 = CVector2(-0.5,-0.3);
  m_unCostSpot1 = 0;
  m_fObjectiveFunction = 0;
}

/****************************************/
/****************************************/

GiandujaAggregationLoopFunction::GiandujaAggregationLoopFunction(const GiandujaAggregationLoopFunction& orig) {}

/****************************************/
/****************************************/

GiandujaAggregationLoopFunction::~GiandujaAggregationLoopFunction() {}

/****************************************/
/****************************************/

void GiandujaAggregationLoopFunction::Destroy() {}

/****************************************/
/****************************************/

argos::CColor GiandujaAggregationLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());
  Real d = (m_cCoordSpot1 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  d = (m_cCoordSpot2 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::WHITE;
  }

  return CColor::GRAY50;
}

/****************************************/
/****************************************/

void GiandujaAggregationLoopFunction::PostStep() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                         pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();
        if (fDistanceSpot1 >= m_fRadius) {
        m_unCostSpot1 += 1;
        }
    }
    m_fObjectiveFunction = (Real) m_unCostSpot1;
}

/****************************************/
/****************************************/

void GiandujaAggregationLoopFunction::PostExperiment() {
}

Real GiandujaAggregationLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

REGISTER_LOOP_FUNCTIONS(GiandujaAggregationLoopFunction, "gianduja_aggregation_loop_functions");
