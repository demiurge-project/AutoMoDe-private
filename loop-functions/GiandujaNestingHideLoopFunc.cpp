#include "GiandujaNestingHideLoopFunc.h"

/****************************************/
/****************************************/

GiandujaNestingHideLoopFunction::GiandujaNestingHideLoopFunction() {
  m_fRadius = 0.07;
  m_cCoordSpot1 = CVector2(0.6,0.8);
  m_cCoordSpot2 = CVector2(-0.5,0.5);

  m_CCoordRect1Pos[0] = CVector2(1,0.8);
  m_CCoordRect1Pos[1] = CVector2(1,-0.2);
  m_CCoordRect1Pos[2] = CVector2(-0.4,-0.2);
  m_CCoordRect1Pos[3] = CVector2(-0.4,0.8);

  m_CCoordRect1 = CVector2(1.25,-0.5);

  m_CCoordRect2Pos[0] = CVector2(0.4,0.2);
  m_CCoordRect2Pos[1] = CVector2(0.4,-0.8);
  m_CCoordRect2Pos[2] = CVector2(-1,-0.8);
  m_CCoordRect2Pos[3] = CVector2(-1,0.2);

  m_CCoordRect2 = CVector2(-1.25,-1.25);

  m_cCoordRectSpot = CVector2(0.6,0.8);
  m_cCoordRectSize = 0.20;


  m_unCostI = 0;
  m_unCostO = 0;
  m_unState = 0;
  m_unTBar = 0;
  m_fObjectiveFunction = 0;
  m_unTime = 0;
  m_unStart = 0;

}

/****************************************/
/****************************************/

GiandujaNestingHideLoopFunction::GiandujaNestingHideLoopFunction(const GiandujaNestingHideLoopFunction& orig) {}

/****************************************/
/****************************************/

GiandujaNestingHideLoopFunction::~GiandujaNestingHideLoopFunction() {}

/****************************************/
/****************************************/

void GiandujaNestingHideLoopFunction::Destroy() {}

/****************************************/
/****************************************/

void GiandujaNestingHideLoopFunction::Init(TConfigurationNode& t_tree) {
    AutoMoDeLoopFunctions::Init(t_tree);
    Real a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
    Real b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
    m_cCoordSpot1 = CVector2(-0.6+a*1.2,0.5+b*0.3);

    m_cCoordRectSpot = CVector2(-0.6+a*1.2,0.5+b*0.3);
    m_unTBar = m_pcRng->Uniform(CRange<UInt32>(150, 1050));
    m_unStart = m_pcRng->Uniform(CRange<UInt32>(0, 4));

}

argos::CColor GiandujaNestingHideLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  CVector2 vCurrentPoint(c_position_on_plane.GetX(), c_position_on_plane.GetY());

  if ( (vCurrentPoint.GetX()<=m_cCoordRectSpot.GetX()+m_cCoordRectSize/2) && (vCurrentPoint.GetX()>=m_cCoordRectSpot.GetX()-m_cCoordRectSize/2) && (vCurrentPoint.GetY()>=m_cCoordRectSpot.GetY()-m_cCoordRectSize/2) && (vCurrentPoint.GetY()<=m_cCoordRectSpot.GetY()+m_cCoordRectSize/2) ) {
    return CColor::BLACK;
  }

  if ( (vCurrentPoint.GetX()<=m_CCoordRect1.GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2.GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2.GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1.GetY()) ) {
    return CColor::WHITE;
  }

  // if ( (vCurrentPoint.GetX()<=m_CCoordRect1Pos[m_unStart].GetX()) && (vCurrentPoint.GetX()>=m_CCoordRect2Pos[m_unStart].GetX()) && (vCurrentPoint.GetY()>=m_CCoordRect2Pos[m_unStart].GetY()) && (vCurrentPoint.GetY()<=m_CCoordRect1Pos[m_unStart].GetY()) ) {
  //   return CColor::GREEN;
  // }

  return CColor::GRAY50;
}


void GiandujaNestingHideLoopFunction::PositionRobots() {
  Real a;
  Real b;

  CEPuckEntity* pcEpuck;
  UInt32 unTrials;
  bool bPlaced = false;

  CBoxEntity& cBox = dynamic_cast<CBoxEntity&>(GetSpace().GetEntity("box0"));
  MoveEntity((cBox).GetEmbodiedEntity(),CVector3(m_cCoordSpot1.GetX(), m_cCoordSpot1.GetY(), 0),CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));

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

       Real fPosX = m_CCoordRect2Pos[m_unStart].GetX() + a*fabs(m_CCoordRect2Pos[m_unStart].GetX() - m_CCoordRect1Pos[m_unStart].GetX());
       Real fPosY = m_CCoordRect2Pos[m_unStart].GetY() + b*fabs(m_CCoordRect2Pos[m_unStart].GetY() - m_CCoordRect1Pos[m_unStart].GetY());

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

void GiandujaNestingHideLoopFunction::PostStep() {
    CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
    CVector2 cEpuckPosition(0,0);
    UInt8 un_trigger = 0;
    for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
        CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);
        cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                         pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());

        //Real fDistanceSpot1 = (m_cCoordSpot1 - cEpuckPosition).Length();

        if (m_unState == 0) {
            if (m_unTime >= m_unTBar) {
                un_trigger = 1;
                CBoxEntity& cBox = dynamic_cast<CBoxEntity&>(GetSpace().GetEntity("box0"));
                MoveEntity((cBox).GetEmbodiedEntity(),CVector3(2, 2, 1),CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
            }
            if ( (cEpuckPosition.GetX()<=m_CCoordRect1.GetX()) && (cEpuckPosition.GetX()>=m_CCoordRect2.GetX()) && (cEpuckPosition.GetY()>=m_CCoordRect2.GetY()) && (cEpuckPosition.GetY()<=m_CCoordRect1.GetY()) ) { //inside nest
                m_unCostI+=1;
            }
        }
        else if (m_unState == 1) {
            if ( !((cEpuckPosition.GetX()<=m_CCoordRect1.GetX()) && (cEpuckPosition.GetX()>=m_CCoordRect2.GetX()) && (cEpuckPosition.GetY()>=m_CCoordRect2.GetY()) && (cEpuckPosition.GetY()<=m_CCoordRect1.GetY())) ) { //outside nest
                m_unCostO+=1;
            }
        }
    }

    if (m_unState == 0 && un_trigger == 1) {
        m_unState = 1;
        LOG << "Let's discover the spot, triggering." << std::endl;
    }
    m_unTime+=1;

}

/****************************************/
/****************************************/

void GiandujaNestingHideLoopFunction::PostExperiment() {
    LOG<< "CostI :" << m_unCostI << " / CostO :" << m_unCostO << " / TBar:" << m_unTBar << std::endl;
    LOG<< m_unCostI + m_unCostO + m_unTBar << std::endl;
    m_fObjectiveFunction = (Real) m_unCostI + m_unCostO;

}

Real GiandujaNestingHideLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

REGISTER_LOOP_FUNCTIONS(GiandujaNestingHideLoopFunction, "gianduja_nesting_hide_loop_functions");
