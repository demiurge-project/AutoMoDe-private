/*
 * Surface and Perimeter Coverage
 *
 * @file <loop-functions/example/MateSPCLoopFunc.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package AutoMoDe-Mate
 *
 * @license MIT License
 */

 #ifndef MATE_SPC_LOOP_FUNC_H
 #define MATE_SPC_LOOP_FUNC_H

 #include "../../../src/core/AutoMoDeLoopFunctions.h"
 #include <argos3/core/simulator/space/space.h>
 #include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

using namespace argos;

class MateSPCLoopFunction : public AutoMoDeLoopFunctions {

   public:
      MateSPCLoopFunction();
      MateSPCLoopFunction(const MateSPCLoopFunction& orig);
      virtual ~MateSPCLoopFunction();

      virtual void Destroy();
      virtual void Reset();
      virtual void PostExperiment();

      Real GetObjectiveFunction();

      virtual CColor GetFloorColor(const CVector2& c_position_on_plane);

      virtual CVector3 GetRandomPosition();

    private:
      Real ComputeObjectiveFunction();
      CVector2 RandomPointOnSquareArea();
      CVector2 RandomPointOnCirclePerimeter();
      bool IsOnSquareArea(CVector2 c_point);
      bool IsOnCirclePerimeter(CVector2 c_point);

      Real m_fRadiusRobot;

      Real m_fSideSquare;
      Real m_fRadiusCircle;

      CVector2 m_cCoordCircleSpot;
      CVector2 m_cCoordSquareSpot;

      UInt32 m_unNumberPoints;

      Real m_fObjectiveFunction;
      Real m_fDoptA;
      Real m_fDoptP;
};

#endif
