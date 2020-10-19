#pragma once

#include "AutoMoDeCondition.h"

namespace argos {

class AutoMoDeConditionReceivingSignal: public AutoMoDeCondition {
public:
	AutoMoDeConditionReceivingSignal();
	virtual ~AutoMoDeConditionReceivingSignal();

	AutoMoDeConditionReceivingSignal(AutoMoDeConditionReceivingSignal* pc_condition);
	virtual AutoMoDeConditionReceivingSignal* Clone();

	virtual bool Verify();
	virtual void Reset();
	virtual void Init();

private:
	UInt8 m_signal;
};

}
