#pragma once

#include "AutoMoDeBehaviour.h"

namespace argos {

class AutoMoDeBehaviourAcknowledgment : public AutoMoDeBehaviour
{
public:
    AutoMoDeBehaviourAcknowledgment();
    AutoMoDeBehaviourAcknowledgment(AutoMoDeBehaviourAcknowledgment const& behaviour);
    virtual ~AutoMoDeBehaviourAcknowledgment();

    virtual void ControlStep();
    virtual void Reset();
    virtual void ResumeStep();
    virtual void Init();

    virtual AutoMoDeBehaviourAcknowledgment* Clone();

    virtual bool Succeeded();
    virtual bool Failed();

private:
    void SendSignal();

    UInt8 m_signal;
    UInt8 m_max_ticks;
    UInt8 m_current_waiting_ticks;
};

};
