/**
  * @file <src/modules/AutoMoDeBehaviourAttractionColor.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_ATTRACTION_COLOR_H
#define AUTOMODE_BEHAVIOUR_ATTRACTION_COLOR_H

#include "AutoMoDeBehaviour.h"

namespace argos {
    class AutoMoDeBehaviourAttractionColor: public AutoMoDeBehaviour {
		public:
            AutoMoDeBehaviourAttractionColor();
            AutoMoDeBehaviourAttractionColor(AutoMoDeBehaviourAttractionColor* pc_behaviour);
            virtual ~AutoMoDeBehaviourAttractionColor();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

            virtual AutoMoDeBehaviourAttractionColor* Clone();

        private:
            Real m_fDistanceWeightParameter;
            CColor m_cColorEmiterParameter;
            CColor m_cColorReceiverParameter;
            UInt8 m_unAttractionParameter;
	};
}

#endif
