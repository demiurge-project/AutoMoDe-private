/**
  * @file <src/modules/AutoMoDeConditionNeighborsState.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_CONDITION_NEIGHBORS_STATE_H
#define AUTOMODE_CONDITION_NEIGHBORS_STATE_H

#include "AutoMoDeCondition.h"

namespace argos {
    class AutoMoDeConditionNeighborsState: public AutoMoDeCondition {
		public:
            AutoMoDeConditionNeighborsState();
            virtual ~AutoMoDeConditionNeighborsState();

            AutoMoDeConditionNeighborsState(AutoMoDeConditionNeighborsState* pc_condition);
            virtual AutoMoDeConditionNeighborsState* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
            Real m_fParameterS;
			UInt8 m_unParameterXi;
	};
}

#endif
