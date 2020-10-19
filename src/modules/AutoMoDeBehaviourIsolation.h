#pragma once

#include "AutoMoDeBehaviour.h"

namespace argos {

class AutoMoDeBehaviourIsolation : public AutoMoDeBehaviour
{
public:
    AutoMoDeBehaviourIsolation();
    AutoMoDeBehaviourIsolation(AutoMoDeBehaviourIsolation const& behaviour);
    virtual ~AutoMoDeBehaviourIsolation();

    virtual void ControlStep();
    virtual void Reset();
    virtual void ResumeStep();
    virtual void Init();

    virtual AutoMoDeBehaviourIsolation* Clone();

    virtual bool Succeeded();
    virtual bool Failed();

private:
    void UpdateCurrentNeighborsCount();

    UInt32 m_min_neighbors;
    UInt32 m_max_neighbors;
    UInt32 m_current_neighbors_count;
    UInt32 m_repulsion_parameter;
};

};
