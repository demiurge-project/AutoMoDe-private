/**
  * @file <src/modules/AutoMoDeBehaviourFormation.h>
  *
  * @author Fernando Mendiburu - <fmendiburu@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_FORMATION_H
#define AUTOMODE_BEHAVIOUR_FORMATION_H

#include "AutoMoDeBehaviour.h"

namespace argos {
    class AutoMoDeBehaviourFormation: public AutoMoDeBehaviour {
		public:
            AutoMoDeBehaviourFormation();
            AutoMoDeBehaviourFormation(AutoMoDeBehaviourFormation* pc_behaviour);
            virtual ~AutoMoDeBehaviourFormation();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

            virtual AutoMoDeBehaviourFormation* Clone();

		private:

            struct SCoesionParams {
              Real TargetDistance;  //cm
              Real Exponent;
              Real Gain;
            };

            /* Init Variables. */
            void InitializeVariables();

            /* The coesion interaction parameters. */
            SCoesionParams m_sCoesionParams;

//            /* The coesion pattern function. */
//            CVector2 Coesion(CCI_EPuckOmnidirectionalCameraSensor::SReadings sReadings);

//            /* The Lenard-Jones function. */
//            Real LJMagnitude(Real fDistance, Real fGain);

	};
}

#endif
