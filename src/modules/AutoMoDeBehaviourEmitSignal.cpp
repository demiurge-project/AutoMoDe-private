#include "AutoMoDeBehaviourEmitSignal.h"

using namespace argos;

AutoMoDeBehaviourEmitSignal::AutoMoDeBehaviourEmitSignal() :
    AutoMoDeBehaviour()
{
    m_strLabel = "EmitSignal";
}

AutoMoDeBehaviourEmitSignal::AutoMoDeBehaviourEmitSignal(
    AutoMoDeBehaviourEmitSignal const& behaviour)
{
    m_strLabel = behaviour.GetLabel();
	m_bLocked = behaviour.IsLocked();
	m_bOperational = behaviour.IsOperational();
	m_unIndex = behaviour.GetIndex();
	m_unIdentifier = behaviour.GetIdentifier();
	m_mapParameters = behaviour.m_mapParameters;
	Init();
}

AutoMoDeBehaviourEmitSignal::~AutoMoDeBehaviourEmitSignal()
{}

void AutoMoDeBehaviourEmitSignal::ControlStep()
{
    m_pcRobotDAO->SetSignalToSend(m_signal);

    m_pcRobotDAO->SetWheelsVelocity(CVector2(0.0, 0.0));
}

void AutoMoDeBehaviourEmitSignal::Reset()
{
	m_bOperational = false;
	ResumeStep();
}

void AutoMoDeBehaviourEmitSignal::ResumeStep()
{
	m_bOperational = true;
}

void AutoMoDeBehaviourEmitSignal::Init()
{
    std::map<std::string, Real>::const_iterator signal_it = m_mapParameters.find("sig");

    if(signal_it != m_mapParameters.end())
    {
        m_signal = static_cast<UInt8>(signal_it->second);
    }
    else
	{
		LOGERR << "[FATAL] Missing sig parameter for the following behaviour:"
		       << m_strLabel << std::endl;
		THROW_ARGOSEXCEPTION("Missing Parameter");
	}

	if(m_signal < 0 || m_signal > 6)
    {
        LOGERR << "[FATAL] Invalid values for sig (" << m_signal << ") "
               << "in behaviour " << m_strLabel << " (should be in [0,6])"
               << std::endl;
        THROW_ARGOSEXCEPTION("Bad parameter value");
    }
}

AutoMoDeBehaviourEmitSignal* AutoMoDeBehaviourEmitSignal::Clone()
{
    return new AutoMoDeBehaviourEmitSignal(*this);
}

bool AutoMoDeBehaviourEmitSignal::Succeeded()
{
    // Succeeded() is always called before ControlStep().
    // However we want to write the signal and then return success,
    // so we call ControlStep() from here.
    ControlStep();

    return true;
}

bool AutoMoDeBehaviourEmitSignal::Failed()
{
    return false;
}

