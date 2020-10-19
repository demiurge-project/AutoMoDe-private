#pragma once

#include "AutoMoDeBehaviour.h"

namespace argos {

class AutoMoDeBehaviourMeeting : public AutoMoDeBehaviour
{
public:
    AutoMoDeBehaviourMeeting();
    AutoMoDeBehaviourMeeting(AutoMoDeBehaviourMeeting const& behaviour);
    virtual ~AutoMoDeBehaviourMeeting();

    virtual void ControlStep();
    virtual void Reset();
    virtual void ResumeStep();
    virtual void Init();

    virtual AutoMoDeBehaviourMeeting* Clone();

    virtual bool Succeeded();
    virtual bool Failed();

private:
    UInt32 m_distance_min;
    UInt8 m_signal;
    UInt32 m_attraction_parameter;
};

};
