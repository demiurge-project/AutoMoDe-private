#pragma once

#include "AutoMoDeBehaviour.h"

namespace argos {

class AutoMoDeBehaviourEmitSignal : public AutoMoDeBehaviour
{
public:
    AutoMoDeBehaviourEmitSignal();
    AutoMoDeBehaviourEmitSignal(AutoMoDeBehaviourEmitSignal const& behaviour);
    virtual ~AutoMoDeBehaviourEmitSignal();

    virtual void ControlStep();
    virtual void Reset();
    virtual void ResumeStep();
    virtual void Init();

    virtual AutoMoDeBehaviourEmitSignal* Clone();

    virtual bool Succeeded();
    virtual bool Failed();

private:
    UInt8 m_signal;
};

};
