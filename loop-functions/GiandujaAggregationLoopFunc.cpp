#include "GiandujaAggregationLoopFunc.h"

/****************************************/
/****************************************/

GiandujaAggregationLoopFunction::GiandujaAggregationLoopFunction() {
  m_fRadius = 0.3;
  m_cCoordSpot1 = CVector2(0.5,0.5);
  m_cCoordSpot2 = CVector2(-0.5,0.5);
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

void GiandujaAggregationLoopFunction::PositionRobots() {
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
        //    if (b < a) {
        //      temp = a;
        //      a = b;
        //      b = temp;
        //    }
           //Real fPosX = m_CCoordRect2.GetX() + a*fabs(m_CCoordRect2.GetX() - m_CCoordRect1.GetX());
           //Real fPosY = m_CCoordRect2.GetY() + b*fabs(m_CCoordRect2.GetY() - m_CCoordRect1.GetY());
           Real fPosY = -1.0 + a * 0.8;
           Real fPosX = -0.8 + b * 1.6;

           bPlaced = MoveEntity((*pcEpuck).GetEmbodiedEntity(),
                                CVector3(fPosX, fPosY, 0),
                                CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
                                CRadians::ZERO,CRadians::ZERO),false);

        } while(!bPlaced && unTrials < 100);

        if(!bPlaced) {
           THROW_ARGOSEXCEPTION("Can't place robot #" << i);
        }
    }
    /********************/
    // /* Place a light at a random spot to prevent robots to exploit (anti)phototaxis behaviours as RW */
    CLightEntity& cLight = dynamic_cast<CLightEntity&>(GetSpace().GetEntity("light0"));
    /* Consider the light only if it has non zero intensity */
    cLight.SetIntensity(m_pcRng->Uniform(CRange<Real>(0.05f, 0.8f)));

    Real pos = m_pcRng->Uniform(CRange<Real>(-1.0f, 1.0f));

    cLight.MoveTo(CVector3(0, pos, 0.4),CQuaternion().FromEulerAngles(m_pcRng->Uniform(CRange<CRadians>(CRadians::ZERO,CRadians::TWO_PI)),
    CRadians::ZERO,CRadians::ZERO));

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
