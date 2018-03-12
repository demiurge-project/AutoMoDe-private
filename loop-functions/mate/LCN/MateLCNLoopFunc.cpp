/**
  * @file <loop-functions/mate/LCN/MateLCNLoopFunc.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package AutoMoDe-Mate
  *
  * @license MIT License
  */

#include "MateLCNLoopFunc.h"

/****************************************/
/****************************************/

MateLCNLoopFunction::MateLCNLoopFunction() {
  m_fSensingRange = 0.25;
  m_fCommunicationDistance = 0.35;
  m_unNumberPoints = 100000;
  m_cArenaCenter = CVector2(0,0);
  m_fObjectiveFunction = 0;
  PerformanceVector.begin();

}

/****************************************/
/****************************************/

MateLCNLoopFunction::MateLCNLoopFunction(const MateLCNLoopFunction& orig) {}

/****************************************/
/****************************************/

MateLCNLoopFunction::~MateLCNLoopFunction() {}

/****************************************/
/****************************************/

void MateLCNLoopFunction::Destroy() {}

/****************************************/
/****************************************/

void MateLCNLoopFunction::Reset() {
  AutoMoDeLoopFunctions::Reset();
}

/****************************************/
/****************************************/

argos::CColor MateLCNLoopFunction::GetFloorColor(const argos::CVector2& c_position_on_plane) {
  return CColor::GRAY50;
}

/****************************************/
/****************************************/

void MateLCNLoopFunction::PostExperiment() {
  m_fObjectiveFunction = ComputeObjectiveFunction();
  PerformanceVector.push_back(m_fObjectiveFunction);
  LOG << "Metrica: " << m_fObjectiveFunction << std::endl;
  LOG << "Mean: " << ComputeMeanVector() << std::endl;
}

/****************************************/
/****************************************/

void MateLCNLoopFunction::PostStep() {
    if(!(GetSpace().GetSimulationClock() % 50)) {
      m_fObjectiveFunction = ComputeObjectiveFunction();

      PerformanceVector.push_back(m_fObjectiveFunction);

      LOG << "Metrica: " << m_fObjectiveFunction << std::endl;
      LOG << "Mean: " << ComputeMeanVector() << std::endl;
      LOG << "Clock: " << GetSpace().GetSimulationClock() << std::endl;
    }
}

/****************************************/
/****************************************/

Real MateLCNLoopFunction::GetObjectiveFunction() {
  return m_fObjectiveFunction;
}

/****************************************/
/****************************************/

void MateLCNLoopFunction::AddNeighs(std::vector<CNetAgent> &agents, std::vector<CNetAgent>::iterator ag) {
	for (std::vector<CNetAgent>::iterator neigh = agents.begin(); neigh != agents.end(); ++neigh) {
		if (neigh->unClusterID != 0)
         continue;
		if ( Distance(ag->cPosition, neigh->cPosition) < m_fCommunicationDistance ) {
			neigh->unClusterID = ag->unClusterID;
			AddNeighs(agents, neigh);
		}
	}
}

/****************************************/
/****************************************/

Real MateLCNLoopFunction::ComputeMeanVector() {

    Real fMean = 0;

    if (!PerformanceVector.empty()) {
        Real fSum = std::accumulate(PerformanceVector.begin(), PerformanceVector.end(), 0.0);
        fMean = fSum / PerformanceVector.size();
    }

    return fMean;
}

/****************************************/
/****************************************/

std::vector<MateLCNLoopFunction::CNetAgent> MateLCNLoopFunction::PickAgents() {

    std::vector<CNetAgent> agents;
    CSpace::TMapPerType m_tEPuckEntityMap = GetSpace().GetEntitiesByType("epuck");

    CVector2 cEpuckPosition;

    /* create a vector with the agents positions (using the objects CNetAgents) */
    for (CSpace::TMapPerType::iterator itEPuckEntity = m_tEPuckEntityMap.begin(); itEPuckEntity != m_tEPuckEntityMap.end(); itEPuckEntity++) {
       CEPuckEntity* pcEpuck = any_cast<CEPuckEntity*> (itEPuckEntity->second);
       cEpuckPosition.Set(pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                          pcEpuck->GetEmbodiedEntity().GetOriginAnchor().Position.GetY());
       agents.push_back(CNetAgent(cEpuckPosition));
    }

    return agents;
}

/****************************************/
/****************************************/

UInt32 MateLCNLoopFunction::DetermineBiggestGroup(std::vector<CNetAgent> &agents) {

    /* Cluster the agents in groups with maximum distance MAX_COMM_DIST */
    UInt32 maxUsedID = 0;
    for (std::vector<CNetAgent>::iterator ag = agents.begin(); ag != agents.end(); ++ag){
       if (ag->unClusterID != 0)
          continue;
       ag->unClusterID = ++maxUsedID;
       AddNeighs(agents, ag);
    }

    /* Determine the biggest group */
    size_t maxGroupSize = 0;
    UInt32 biggestGroupID = 0;
    for (UInt32 i = 1; i <= maxUsedID; i++){
       size_t size = 0;
       for (std::vector<CNetAgent>::iterator ag = agents.begin(); ag != agents.end(); ++ag){
          if (ag->unClusterID == i) {
             size++;
          }
       }

       if (maxGroupSize < size){
          maxGroupSize = size;
          biggestGroupID = i;
       }
    }

    return biggestGroupID;
}


/****************************************/
/****************************************/

Real MateLCNLoopFunction::ComputeCoverageRatio(std::vector<CNetAgent> &agents, UInt32 GroupID) {

    /* create a vector of positions of only the elements in the group */
    /* and at the same time, calculate the extreme coordinates of the group */
    std::vector<CVector2> vGroup;
    Real min_x = std::numeric_limits<double>::max();
    Real min_y = std::numeric_limits<double>::max();
    Real max_x = 0;
    Real max_y = 0;
    for (std::vector<CNetAgent>::iterator ag = agents.begin(); ag != agents.end(); ++ag){
       if (ag->unClusterID == GroupID) {
          min_x = (ag->cPosition.GetX() < min_x)? ag->cPosition.GetX() : min_x;
          min_y = (ag->cPosition.GetY() < min_y)? ag->cPosition.GetY() : min_y;
          max_x = (ag->cPosition.GetX() > max_x)? ag->cPosition.GetX() : max_x;
          max_y = (ag->cPosition.GetY() > max_y)? ag->cPosition.GetY() : max_y;
          vGroup.push_back(ag->cPosition);
       }
    }

    /* calculate the bounding box sizes (ranges) */
    CRange<Real> width( min_x - m_fSensingRange, max_x + m_fSensingRange);
    CRange<Real> height(min_y - m_fSensingRange, max_y + m_fSensingRange);

    /* Monte-Carlo sampling to estimate the ratio of the bounding box that is covered by the sensing range */
    Real avg = 0;
    for (size_t i = 0; i < (UInt32)Ceil((width.GetSpan()*height.GetSpan())*m_unNumberPoints); i++) {
       Real rx = m_pcRng->Uniform(width);
       Real ry = m_pcRng->Uniform(height);
       CVector2 rndPoint(rx, ry);

       for (std::vector<CVector2>::iterator pos = vGroup.begin(); pos != vGroup.end(); ++pos) {
          if (Distance(rndPoint, *pos) <= m_fSensingRange && Distance(rndPoint, m_cArenaCenter) < m_fDistributionRadius) {
             avg++;
             break;
          }
       }
    }

    /* Compute the coverage-ratio */
    avg = avg / Ceil((width.GetSpan()*height.GetSpan())*m_unNumberPoints);
    /* Multiply the area size (bounding box) by the coverage-ratio */
    Real performance = width.GetSpan() * height.GetSpan() * avg;

    return performance;
}

/****************************************/
/****************************************/




/****************************************/
/****************************************/

Real MateLCNLoopFunction::ComputeObjectiveFunction() {

  /* Push all agents in a vector */
  std::vector<CNetAgent> agents = PickAgents();

  /* Determine the biggest group ID*/
  UInt32 unBiggestGroupID = DetermineBiggestGroup(agents);

  /* Compute the coverage-ratio of the biggest group*/
  Real performance = ComputeCoverageRatio(agents,unBiggestGroupID);


  return performance;
}

/****************************************/
/****************************************/

CVector3 MateLCNLoopFunction::GetRandomPosition() {

    CVector3 cPosition;

    cPosition.FromSphericalCoords(
                m_pcRng->Uniform(CRange<Real>(-m_fDistributionRadius, m_fDistributionRadius)),     // distance from origin
                CRadians::PI_OVER_TWO,                                                             // angle with Z axis
                m_pcRng->Uniform(CRadians::UNSIGNED_RANGE)                                         // rotation around Z
                );

    return CVector3(cPosition.GetX(), cPosition.GetY(), 0);
}

REGISTER_LOOP_FUNCTIONS(MateLCNLoopFunction, "mate_lcn_loop_functions");
