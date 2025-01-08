/**
  * @file <src/modules/AutoMoDeBehaviourGoAwayFromPatch.h>
  *
  * @author Jeanne Szpirer - <jszpirer@ulb.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_GOAWAYFROM_PATCH_H
#define AUTOMODE_BEHAVIOUR_GOAWAYFROM_PATCH_H

#include "AutoMoDeBehaviour.h"

namespace argos {
    class AutoMoDeBehaviourGoAwayFromPatch: public AutoMoDeBehaviour {
		public:
            AutoMoDeBehaviourGoAwayFromPatch();
            AutoMoDeBehaviourGoAwayFromPatch(AutoMoDeBehaviourGoAwayFromPatch* pc_behaviour);
            virtual ~AutoMoDeBehaviourGoAwayFromPatch();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

            virtual AutoMoDeBehaviourGoAwayFromPatch* Clone();

		private:
            CColor m_cColorEmiterParameter;
            UInt8 m_unColor;
            Real m_unRepulsionParameter;
	};
}


#endif
