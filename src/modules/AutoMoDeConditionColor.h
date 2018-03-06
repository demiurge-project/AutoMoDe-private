/**
  * @file <src/modules/AutoMoDeConditionColor.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_CONDITION_COLOR_H
#define AUTOMODE_CONDITION_COLOR_H

#include "AutoMoDeCondition.h"

namespace argos {
    class AutoMoDeConditionColor: public AutoMoDeCondition {
		public:
            AutoMoDeConditionColor();
            virtual ~AutoMoDeConditionColor();

            AutoMoDeConditionColor(AutoMoDeConditionColor* pc_condition);
            virtual AutoMoDeConditionColor* Clone();

			virtual bool Verify();
			virtual void Reset();
			virtual void Init();

		private:
            CColor m_cColorParameter;
			Real m_fProbability;
            Real m_fDistance;
            Real m_fParameterEta;
            UInt32 m_unParameterXi;
	};
}

#endif
