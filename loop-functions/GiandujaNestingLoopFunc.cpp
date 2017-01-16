#include "GiandujaNestingLoopFunc.h"

/****************************************/
/****************************************/

GiandujaNestingLoopFunction::GiandujaNestingLoopFunction() {
  m_fRadius = 0.1;
  m_cCoordSpot1 = CVector2(0.6,0.8);
  m_cCoordSpot2 = CVector2(-0.5,0.5);
  m_CCoordRect1 = CVector2(0.8,-0.2);
  m_CCoordRect2 = CVector2(-0.8,-1);
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

  // d = (m_cCoordSpot2 - vCurrentPoint).Length();
  // if (d <= m_fRadius) {
  //   return CColor::BLACK;
  // }

  if ( (vCurrentPoint.GetX()<=m_CCoordRect1.GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2.GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2.GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1.GetY()) ) {
    return CColor::GREEN;
  }

  return CColor::GRAY50;
}


void GiandujaNestingLoopFunction::PositionRobots() {
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
       //Real fPosX = b * m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
       //Real fPosY = b * m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));
       Real fPosX = m_CCoordRect2.GetX() + a*fabs(m_CCoordRect2.GetX() - m_CCoordRect1.GetX());
       Real fPosY = m_CCoordRect2.GetY() + b*fabs(m_CCoordRect2.GetY() - m_CCoordRect1.GetY());
       //Real fPosY = -1.0 + a * 0.8;
       //Real fPosX = -0.8 + b * 1.6;

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
