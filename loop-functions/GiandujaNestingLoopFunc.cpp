#include "GiandujaNestingLoopFunc.h"

/****************************************/
/****************************************/

GiandujaNestingLoopFunction::GiandujaNestingLoopFunction() {
  m_fRadius = 0.2;
  m_cCoordSpot1 = CVector2(0.5,0.5);
  m_cCoordSpot2 = CVector2(-0.5,0.5);
  m_CCoordRect1 = CVector2(1,-0.7);
  m_CCoordRect2 = CVector2(-1,-1.3);
  m_unCost = 0;
  m_unState = 0;
  m_unTbar = 0;
  m_fObjectiveFunction = 0;
}

/****************************************/
/****************************************/

GiandujaNestingLoopFunction::GiandujaNestingLoopFunction(const GiandujaNestingLoopFunction& orig) {}

/****************************************/
/****************************************/

GiandujaNestingLoopFunction::~GiandujaNestingLoopFunction() {}

/****************************************/
/****************************************/

void GiandujaNestingLoopFunction::Destroy() {}

/****************************************/
/****************************************/

argos::CColor GiandujaNestingLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());
  Real d = (m_cCoordSpot1 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  d = (m_cCoordSpot2 - vCurrentPoint).Length();
  if (d <= m_fRadius) {
    return CColor::BLACK;
  }

  if ( (vCurrentPoint.GetX()<=m_CCoordRect1.GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2.GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2.GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1.GetY()) ) {
    return CColor::GREEN;
  }

  return CColor::GRAY50;
}

void GiandujaNestingLoopFunction::PositionRobots() {

}

/****************************************/
/****************************************/

void GiandujaNestingLoopFunction::PostStep() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    UInt8 un_trigger = 0;
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                         pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();
        Real fDistanceSpot2 = (m_cCoordSpot2 - cEpuckPosition).Length();

        if (m_unState == 0) {
            if ((fDistanceSpot1 <= m_fRadius) || (fDistanceSpot2 <= m_fRadius)) {
                un_trigger = 1;
            }
            if ( (cEpuckPosition.GetX()<=m_CCoordRect1.GetX()) && (cEpuckPosition.GetX()>=m_CCoordRect2.GetX()) && (cEpuckPosition.GetY()>=m_CCoordRect2.GetY()) && (cEpuckPosition.GetY()<=m_CCoordRect1.GetY()) ) { //inside nest
                m_unCost+=1;
            }
            m_unTbar +=1;
        }
        else if (m_unState == 1) {
            if ( !((cEpuckPosition.GetX()<=m_CCoordRect1.GetX()) && (cEpuckPosition.GetX()>=m_CCoordRect2.GetX()) && (cEpuckPosition.GetY()>=m_CCoordRect2.GetY()) && (cEpuckPosition.GetY()<=m_CCoordRect1.GetY())) ) { //outside nest
                m_unCost+=1;
            }
        }
    }

    if (m_unState == 0 && un_trigger == 1) {
        m_unState = 1;
    }

}

/****************************************/
/****************************************/

void GiandujaNestingLoopFunction::PostExperiment() {
    LOG<< m_unCost <<"::" << m_unTbar;
    m_fObjectiveFunction = (Real) m_unCost + m_unTbar;
}

Real GiandujaNestingLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

REGISTER_LOOP_FUNCTIONS(GiandujaNestingLoopFunction, "gianduja_nesting_loop_functions");
