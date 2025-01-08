/**
  * @file <src/modules/AutoMoDeBehaviourGoToPatch.h>
  *
  * @author Jeanne Szpirer - <jszpirer@ulb.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_GOTOPATCH_H
#define AUTOMODE_BEHAVIOUR_GOTOPATCH_H

#include "AutoMoDeBehaviour.h"

namespace argos {
	class AutoMoDeBehaviourGoToPatch: public AutoMoDeBehaviour {
		public:
			AutoMoDeBehaviourGoToPatch();
			AutoMoDeBehaviourGoToPatch(AutoMoDeBehaviourGoToPatch* pc_behaviour);
			virtual ~AutoMoDeBehaviourGoToPatch();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

			virtual AutoMoDeBehaviourGoToPatch* Clone();

		private:
			UInt8 m_unAttractionParameter;
			UInt8 m_unColorParameter;
            		CColor m_cColorEmiterParameter;
	};
}

#endif
