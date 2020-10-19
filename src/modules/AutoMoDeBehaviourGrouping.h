#pragma once

#include "AutoMoDeBehaviour.h"

namespace argos {

class AutoMoDeBehaviourGrouping : public AutoMoDeBehaviour
{
public:
    AutoMoDeBehaviourGrouping();
    AutoMoDeBehaviourGrouping(AutoMoDeBehaviourGrouping const& behaviour);
    virtual ~AutoMoDeBehaviourGrouping();

    virtual void ControlStep();
    virtual void Reset();
    virtual void ResumeStep();
    virtual void Init();

    virtual AutoMoDeBehaviourGrouping* Clone();

    virtual bool Succeeded();
    virtual bool Failed();

private:
    void UpdateCurrentNeighborsCount();

    UInt32 m_min_neighbors;
    UInt32 m_max_neighbors;
    UInt32 m_current_neighbors_count;
    UInt32 m_attraction_parameter;
};

};
