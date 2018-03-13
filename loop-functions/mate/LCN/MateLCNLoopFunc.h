/*
 * Largest Covering Network
 *
 * @file <loop-functions/mate/LCN/MateLCNLoopFunc.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package AutoMoDe-Mate
 *
 * @license MIT License
 */

#ifndef MATE_LCN_LOOP_FUNC_H
#define MATE_LCN_LOOP_FUNC_H

#include "../../../src/core/AutoMoDeLoopFunctions.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
#include <limits>
#include <numeric>

using namespace argos;

class MateLCNLoopFunction : public AutoMoDeLoopFunctions {

   public:
      MateLCNLoopFunction();
      MateLCNLoopFunction(const MateLCNLoopFunction& orig);
      virtual ~MateLCNLoopFunction();

      virtual void Destroy();
      virtual void Reset();
      virtual void PostStep();
      virtual void PostExperiment();

      Real GetObjectiveFunction();

      virtual CColor GetFloorColor(const CVector2& c_position_on_plane);

      virtual CVector3 GetRandomPosition();   


      private:

      struct CNetAgent {
        CVector2 cPosition;
        UInt32 unClusterID;

        CNetAgent(CVector2 c_position) {
          cPosition = c_position;
          unClusterID = 0;
        }
      };

      Real ComputeObjectiveFunction();

      bool IsTimeToMeasure(UInt32 fTimeInSeconds);

      bool IsNearTheEndofTheExperiment(UInt32 unLengthExperimentInSeconds, UInt32 unTimeToEndInSeconds);

      void AddNeighs(std::vector<CNetAgent> &agents, std::vector<CNetAgent>::iterator ag);

      std::vector<MateLCNLoopFunction::CNetAgent> PickAgents();

      UInt32 DetermineBiggestGroup(std::vector<CNetAgent> &agents);

      Real ComputeCoverageRatio(std::vector<CNetAgent> &agents, UInt32 GroupID);

      Real ComputeMeanVector(std::vector<Real> vValues);

      std::vector<Real> PerformanceVector;

      Real m_fRadiusRobot;

      Real m_fObjectiveFunction;

      Real m_fSensingRange;

      Real m_fCommunicationDistance;

      UInt32 m_unNumberPoints;

      CVector2 m_cArenaCenter;

      UInt32 m_unLengthExperiment;
};

#endif
