#include "AutoMoDeBehaviourAcknowledgment.h"

using namespace argos;

AutoMoDeBehaviourAcknowledgment::AutoMoDeBehaviourAcknowledgment() :
    AutoMoDeBehaviour()
{
    m_strLabel = "Acknowledgment";
}

AutoMoDeBehaviourAcknowledgment::AutoMoDeBehaviourAcknowledgment(
    AutoMoDeBehaviourAcknowledgment const& behaviour)
{
    m_strLabel = behaviour.GetLabel();
	m_bLocked = behaviour.IsLocked();
	m_bOperational = behaviour.IsOperational();
	m_unIndex = behaviour.GetIndex();
	m_unIdentifier = behaviour.GetIdentifier();
	m_mapParameters = behaviour.m_mapParameters;
	Init();
}

AutoMoDeBehaviourAcknowledgment::~AutoMoDeBehaviourAcknowledgment()
{}

void AutoMoDeBehaviourAcknowledgment::ControlStep()
{
    SendSignal();

    m_pcRobotDAO->SetWheelsVelocity(CVector2(0.0, 0.0));

    // we waited one tick more
    ++m_current_waiting_ticks;
}

void AutoMoDeBehaviourAcknowledgment::Reset()
{
	m_bOperational = false;
	ResumeStep();
}

void AutoMoDeBehaviourAcknowledgment::ResumeStep()
{
	m_bOperational = true;
}

template<typename ParamType>
void FindParam(std::map<std::string, Real> const& param_map,
               ParamType& param,
               std::string const& param_name,
               std::string const& behavior_name)
{
    std::map<std::string, Real>::const_iterator it = param_map.find(param_name);

	if (it != param_map.end())
	{
		param = it->second;
	}
	else
	{
		LOGERR << "[FATAL] Missing " << param_name
		       << " parameter for the following behaviour:"
		       << behavior_name << std::endl;
		THROW_ARGOSEXCEPTION("Missing Parameter");
	}
}

void AutoMoDeBehaviourAcknowledgment::Init()
{
    FindParam(m_mapParameters, m_signal, "sig", m_strLabel);
    FindParam(m_mapParameters, m_max_ticks, "tmax", m_strLabel);

    if(m_signal <= 0 || m_signal > 7)
    {
        LOGERR << "[FATAL] Invalid values for sig (" << m_signal << ") "
               << "in behaviour " << m_strLabel << " (should be in [1,7])"
               << std::endl;
        THROW_ARGOSEXCEPTION("Bad parameter value");
    }

    m_current_waiting_ticks = 0;
}

AutoMoDeBehaviourAcknowledgment* AutoMoDeBehaviourAcknowledgment::Clone()
{
    return new AutoMoDeBehaviourAcknowledgment(*this);
}

bool AutoMoDeBehaviourAcknowledgment::Succeeded()
{
    // we want this behavior to send the signal every time,
    // even if we already received a matching signal
    SendSignal();

    // then, check for matching signal
    int match_count = m_pcRobotDAO->GetNumberSignalMatchingNeighbors(m_signal);

    if(match_count > 0)
    {
        m_current_waiting_ticks = 0;
        return true;
    }

    return false;
}

bool AutoMoDeBehaviourAcknowledgment::Failed()
{
    if(m_current_waiting_ticks > m_max_ticks)
    {
        m_current_waiting_ticks = 0;
        return true;
    }

    return false;
}

void AutoMoDeBehaviourAcknowledgment::SendSignal()
{
    if(m_signal < 7)
        m_pcRobotDAO->SetSignalToSend(m_signal);
}

