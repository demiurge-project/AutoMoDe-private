/**
  * @file <loop-functions/AggregationTwoSpotsLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @license MIT License
  */

#include "AnytimeSelectionDiffLoopFunc.h"

/****************************************/
/****************************************/

AnytimeSelectionDiffLoopFunction::AnytimeSelectionDiffLoopFunction() {
  m_fRadius = 0.3;
  m_cCoordSpot1 = CVector2(0,0.5);
  m_cCoordSpot2 = CVector2(0,-0.5);
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  m_fObjectiveFunction = 0;
}

/****************************************/
/****************************************/

AnytimeSelectionDiffLoopFunction::AnytimeSelectionDiffLoopFunction(const AnytimeSelectionDiffLoopFunction& orig) {}

/****************************************/
/****************************************/

AnytimeSelectionDiffLoopFunction::~AnytimeSelectionDiffLoopFunction() {}

/****************************************/
/****************************************/

void AnytimeSelectionDiffLoopFunction::Destroy() {}

/****************************************/
/****************************************/

argos::CColor AnytimeSelectionDiffLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
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

void AnytimeSelectionDiffLoopFunction::Reset() {
  m_fObjectiveFunction = 0;
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  AutoMoDeLoopFunctions::Reset();
}

/****************************************/
/****************************************/
void AnytimeSelectionDiffLoopFunction::PostStep() {
  CSpace::TMapPerType cEntities = GetSpace().GetEntitiesByType("controller");
  CSpace::TMapPerType& tEpuckMap = GetSpace().GetEntitiesByType("epuck");
  CVector2 cEpuckPosition(0,0);
  m_unScoreSpot1 = 0;
  m_unScoreSpot2 = 0;
  for (CSpace::TMapPerType::iterator it = tEpuckMap.begin(); it != tEpuckMap.end(); ++it) {
    CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*>(it->second);

    CControllableEntity* pcEntity = &(pcEpuck->GetControllableEntity());
    try {
      AutoMoDeController& cController = dynamic_cast<AutoMoDeController&> (pcEntity->GetController());
      if (! cController.IsBatteryDown()) {

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
    } catch (std::exception& ex) {
      LOGERR << "Error while casting: " << ex.what() << std::endl;
    }
  }
  m_fObjectiveFunction += Abs(m_unScoreSpot1 - m_unScoreSpot2);

  //LOG << "Score = " << m_fObjectiveFunction << std::endl;
}

/****************************************/
/****************************************/

Real AnytimeSelectionDiffLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

CVector3 AnytimeSelectionDiffLoopFunction::GetRandomPosition() {
  Real temp;
  Real a = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  Real  b = m_pcRng->Uniform(CRange<Real>(0.0f, 1.0f));
  // If b < a, swap them
  if (b < a) {
    temp = a;
    a = b;
    b = temp;
  }
  Real fPosX = b * m_fDistributionRadius * cos(2 * CRadians::PI.GetValue() * (a/b));
  Real fPosY = b * m_fDistributionRadius * sin(2 * CRadians::PI.GetValue() * (a/b));

  return CVector3(fPosX, fPosY, 0);
}

REGISTER_LOOP_FUNCTIONS(AnytimeSelectionDiffLoopFunction, "anytime_selection_diff_lf");
