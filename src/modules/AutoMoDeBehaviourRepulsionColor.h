/**
  * @file <src/modules/AutoMoDeBehaviourRepulsionColor.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_REPULSION_COLOR_H
#define AUTOMODE_BEHAVIOUR_REPULSION_COLOR_H

#include "AutoMoDeBehaviour.h"

namespace argos {
    class AutoMoDeBehaviourRepulsionColor: public AutoMoDeBehaviour {
		public:
            AutoMoDeBehaviourRepulsionColor();
            AutoMoDeBehaviourRepulsionColor(AutoMoDeBehaviourRepulsionColor* pc_behaviour);
            virtual ~AutoMoDeBehaviourRepulsionColor();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

            virtual AutoMoDeBehaviourRepulsionColor* Clone();

		private:
            Real m_fDistanceWeightParameter;
            CColor m_cColorEmiterParameter;
            CColor m_cColorReceiverParameter;
            UInt8 m_unRepulsionParameter;
	};
}


#endif
