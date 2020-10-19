#include "AutoMoDeConditionReceivingSignal.h"

using namespace argos;

AutoMoDeConditionReceivingSignal::AutoMoDeConditionReceivingSignal() {
	m_strLabel = "ReceivingSignal";
}

/****************************************/
/****************************************/

AutoMoDeConditionReceivingSignal::~AutoMoDeConditionReceivingSignal() {}

/****************************************/
/****************************************/

AutoMoDeConditionReceivingSignal::AutoMoDeConditionReceivingSignal(AutoMoDeConditionReceivingSignal* pc_condition) {
	m_strLabel = pc_condition->GetLabel();
	m_unIndex = pc_condition->GetIndex();
	m_unIdentifier = pc_condition->GetIndex();
	m_unFromBehaviourIndex = pc_condition->GetOrigin();
	m_unToBehaviourIndex = pc_condition->GetExtremity();
	m_mapParameters = pc_condition->GetParameters();
    Init();
}

/****************************************/
/****************************************/

AutoMoDeConditionReceivingSignal* AutoMoDeConditionReceivingSignal::Clone() {
	return new AutoMoDeConditionReceivingSignal(this);
}

/****************************************/
/****************************************/

bool AutoMoDeConditionReceivingSignal::Verify() {
	UInt32 count = m_pcRobotDAO->GetNumberSignalMatchingNeighbors(m_signal);
	return count > 0;
}

/****************************************/
/****************************************/

void AutoMoDeConditionReceivingSignal::Reset()
{}

/****************************************/
/****************************************/

void AutoMoDeConditionReceivingSignal::Init()
{
	std::map<std::string, Real>::const_iterator signal_it = m_mapParameters.find("sig");

    if(signal_it != m_mapParameters.end())
    {
        m_signal = static_cast<UInt8>(signal_it->second);
    }
    else
	{
		LOGERR << "[FATAL] Missing sig parameter for the following condition:"
		       << m_strLabel << std::endl;
		THROW_ARGOSEXCEPTION("Missing Parameter");
	}

	if(m_signal <= 0 || m_signal > 7)
	{
	    LOGERR << "[FATAL] sig parameter for condition " << m_strLabel
	           << " must be a integer in [1,7]" << std::endl;
		THROW_ARGOSEXCEPTION("Invalid Parameter");
	}
}
