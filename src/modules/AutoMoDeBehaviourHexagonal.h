/**
  * @file <src/modules/AutoMoDeBehaviourHexagonal.h>
  *
  * @author Fernando Mendiburu - <fmendibu@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe-Mate
  *
  * @license MIT License
  */


#ifndef AUTOMODE_BEHAVIOUR_HEXAGONAL_H
#define AUTOMODE_BEHAVIOUR_HEXAGONAL_H

#include "AutoMoDeBehaviour.h"

#include <algorithm>




namespace argos {
    class AutoMoDeBehaviourHexagonal: public AutoMoDeBehaviour {
		public:
            AutoMoDeBehaviourHexagonal();
            AutoMoDeBehaviourHexagonal(AutoMoDeBehaviourHexagonal* pc_behaviour);
            virtual ~AutoMoDeBehaviourHexagonal();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();


            virtual AutoMoDeBehaviourHexagonal* Clone();


        private:


            struct SHexagonalParams {
              Real TargetDistance;  //cm
              Real ActualGain;
              Real InitialGain;
              Real FinalGain;
              Real Exponent;
              Real CounterHex;
            };

            /* Init Variables. */
            void InitializeVariables();

            /* The Hexagonal interaction parameters. */
            SHexagonalParams m_sHexagonalParams;

            /* The Hexagonal pattern function. */
            CVector2 Hexagonal(CCI_EPuckVirtualCamrabSensor::TReadings sReadings);

            /* The Lenard-Jones function. */
            Real LJMagnitude(Real fDistance, Real fTargetDistance, Real fGain);

            /* The adaptative gain function. */
            void AdaptativeGain(Real fInitialGain, Real fFinalGain, UInt32 fInitialTimeinSecs, UInt32 fFinalTimeinSecs);

            void PrintSensor(CCI_EPuckVirtualCamrabSensor::TReadings sReadings);

	};
}


#endif
