#include "AutoMoDeBehaviourMeeting.h"

using namespace argos;

AutoMoDeBehaviourMeeting::AutoMoDeBehaviourMeeting() :
    AutoMoDeBehaviour()
{
    m_strLabel = "Meeting";
}

AutoMoDeBehaviourMeeting::AutoMoDeBehaviourMeeting(
    AutoMoDeBehaviourMeeting const& behaviour)
{
    m_strLabel = behaviour.GetLabel();
	m_bLocked = behaviour.IsLocked();
	m_bOperational = behaviour.IsOperational();
	m_unIndex = behaviour.GetIndex();
	m_unIdentifier = behaviour.GetIdentifier();
	m_mapParameters = behaviour.m_mapParameters;
	Init();
}

AutoMoDeBehaviourMeeting::~AutoMoDeBehaviourMeeting()
{}

void AutoMoDeBehaviourMeeting::ControlStep()
{
    CVector2 attraction(0, CRadians::ZERO);
    CCI_EPuckRangeAndBearingSensor::SReceivedPacket cRabReading =
        m_pcRobotDAO->GetAttractionVectorToSignalMatchingNeighbors(
            m_attraction_parameter, m_signal);

    // if no robot found
    if (cRabReading.Range == 0.0f) {
        // no movement and stop here
        m_pcRobotDAO->SetWheelsVelocity(0.0, 0.0);
    }
    else {
	    attraction = CVector2(cRabReading.Range, cRabReading.Bearing);

        // Obstacle avoidance
        CVector2 sDirectionVector(0,CRadians::ZERO);
        CVector2 sProxVector(0,CRadians::ZERO);
        sProxVector = CVector2(m_pcRobotDAO->GetProximityReading().Value,
                               m_pcRobotDAO->GetProximityReading().Angle);
        sDirectionVector = attraction - 6*sProxVector;

        if (sDirectionVector.Length() < 0.1) {
	        sDirectionVector = CVector2(1.0, CRadians::ZERO);
        }

        m_pcRobotDAO->SetWheelsVelocity(
            ComputeWheelsVelocityFromVector(sDirectionVector));
    }

	m_bLocked = false;
}

void AutoMoDeBehaviourMeeting::Reset()
{
	m_bOperational = false;
	ResumeStep();
}

void AutoMoDeBehaviourMeeting::ResumeStep()
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

void AutoMoDeBehaviourMeeting::Init()
{
    FindParam(m_mapParameters, m_distance_min, "dmin", m_strLabel);
    FindParam(m_mapParameters, m_signal, "sig", m_strLabel);
    FindParam(m_mapParameters, m_attraction_parameter, "att", m_strLabel);

    if(m_signal <= 0 || m_signal > 7)
    {
        LOGERR << "[FATAL] Invalid values for sig (" << m_signal << ") "
               << "in behaviour " << m_strLabel << " (should be in [1,7])"
               << std::endl;
        THROW_ARGOSEXCEPTION("Bad parameter value");
    }
}

AutoMoDeBehaviourMeeting* AutoMoDeBehaviourMeeting::Clone()
{
    return new AutoMoDeBehaviourMeeting(*this);
}

bool AutoMoDeBehaviourMeeting::Succeeded()
{
    int match_count = m_pcRobotDAO->GetNumberSignalMatchingNeighbors(m_signal);
    if(match_count == 0)
        return false;

    CCI_EPuckRangeAndBearingSensor::SReceivedPacket center_of_mass =
        m_pcRobotDAO->GetSignalMatchingNeighborsCenterOfMass(m_signal);
    return center_of_mass.Range <= m_distance_min;
}

bool AutoMoDeBehaviourMeeting::Failed()
{
    int match_count = m_pcRobotDAO->GetNumberSignalMatchingNeighbors(m_signal);
    return match_count == 0;
}

