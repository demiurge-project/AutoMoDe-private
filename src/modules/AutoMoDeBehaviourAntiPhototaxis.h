/*
 * File: AutoMoDeBehaviourAntiPhototaxis.h
 *
 */

#ifndef AUTOMODE_BEHAVIOUR_ANTI_PHOTOTAXIS_H
#define AUTOMODE_BEHAVIOUR_ANTI_PHOTOTAXIS_H

#include "AutoMoDeBehaviour.h"

namespace argos {
	class AutoMoDeBehaviourAntiPhototaxis: public AutoMoDeBehaviour {
		public:
			AutoMoDeBehaviourAntiPhototaxis();
			AutoMoDeBehaviourAntiPhototaxis(AutoMoDeBehaviourAntiPhototaxis* pc_behaviour);
			virtual ~AutoMoDeBehaviourAntiPhototaxis();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

			virtual AutoMoDeBehaviourAntiPhototaxis* Clone();

		private:
			UInt8 m_bBroadcastStateAndMessage;
	};
}

#endif
