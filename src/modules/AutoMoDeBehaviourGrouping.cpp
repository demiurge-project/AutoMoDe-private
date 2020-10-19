#include "AutoMoDeBehaviourGrouping.h"

using namespace argos;

AutoMoDeBehaviourGrouping::AutoMoDeBehaviourGrouping() :
    AutoMoDeBehaviour()
{
    m_strLabel = "Grouping";
}

AutoMoDeBehaviourGrouping::AutoMoDeBehaviourGrouping(
    AutoMoDeBehaviourGrouping const& behaviour)
{
    m_strLabel = behaviour.GetLabel();
	m_bLocked = behaviour.IsLocked();
	m_bOperational = behaviour.IsOperational();
	m_unIndex = behaviour.GetIndex();
	m_unIdentifier = behaviour.GetIdentifier();
	m_mapParameters = behaviour.m_mapParameters;
	Init();
}

AutoMoDeBehaviourGrouping::~AutoMoDeBehaviourGrouping()
{}

void AutoMoDeBehaviourGrouping::ControlStep()
{
    // move only if within neighbors bounds
    if(! Succeeded() and ! Failed())
    {
        CVector2 sRabVector(0,CRadians::ZERO);
	    CCI_EPuckRangeAndBearingSensor::SReceivedPacket cRabReading =
	        m_pcRobotDAO->GetAttractionVectorToNeighbors(m_attraction_parameter);

	    if (cRabReading.Range > 0.0f) {
		    sRabVector = CVector2(cRabReading.Range, cRabReading.Bearing);
	    }

	    // Obstacle avoidance
	    CVector2 sDirectionVector(0,CRadians::ZERO);
	    CVector2 sProxVector(0,CRadians::ZERO);
	    sProxVector = CVector2(m_pcRobotDAO->GetProximityReading().Value,
	                           m_pcRobotDAO->GetProximityReading().Angle);
	    sDirectionVector = sRabVector - 6*sProxVector;

	    if (sDirectionVector.Length() < 0.1) {
		    sDirectionVector = CVector2(1.0, CRadians::ZERO);
	    }

	    m_pcRobotDAO->SetWheelsVelocity(
	        ComputeWheelsVelocityFromVector(sDirectionVector));
	}
	else
	{
        m_pcRobotDAO->SetWheelsVelocity(CVector2(0.0, 0.0));
	}

	m_bLocked = false;
}

void AutoMoDeBehaviourGrouping::Reset()
{
	m_bOperational = false;
	m_current_neighbors_count = 0;
	ResumeStep();
}

void AutoMoDeBehaviourGrouping::ResumeStep()
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

void AutoMoDeBehaviourGrouping::Init()
{
    FindParam(m_mapParameters, m_min_neighbors, "min", m_strLabel);
    FindParam(m_mapParameters, m_max_neighbors, "max", m_strLabel);
    FindParam(m_mapParameters, m_fSuccessProbabilityParameter, "p", m_strLabel);
    FindParam(m_mapParameters, m_attraction_parameter, "att", m_strLabel);

    if(m_min_neighbors > m_max_neighbors)
    {
        LOGERR << "[FATAL] Invalid values for min (" << m_min_neighbors << ") "
               << "and max (" << m_max_neighbors << ") neighbors count "
               << "in behaviour" << m_strLabel << std::endl;
        THROW_ARGOSEXCEPTION("Bad parameter value");
    }
}

AutoMoDeBehaviourGrouping* AutoMoDeBehaviourGrouping::Clone()
{
    return new AutoMoDeBehaviourGrouping(*this);
}

bool AutoMoDeBehaviourGrouping::Succeeded()
{
    UpdateCurrentNeighborsCount();

    // success if we passed the upper threshold
    return m_current_neighbors_count > m_max_neighbors;
}

bool AutoMoDeBehaviourGrouping::Failed()
{
    UpdateCurrentNeighborsCount();

    // failure if we go below the low threshold
    return m_current_neighbors_count < m_min_neighbors;
}

void AutoMoDeBehaviourGrouping::UpdateCurrentNeighborsCount()
{
    m_current_neighbors_count = m_pcRobotDAO->GetNumberNeighbors();
}


