/*
 * @file <src/core/AutoMoDeSwarmConfiguration.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDeSwarmConfiguration.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeSwarmConfiguration::AutoMoDeSwarmConfiguration() {
		m_unNumberOfRobots = 0;
		m_unRabBatteryConsumption = 0.0;
	}

	/****************************************/
	/****************************************/

	AutoMoDeSwarmConfiguration::~AutoMoDeSwarmConfiguration() {}

	/****************************************/
	/****************************************/

  void AutoMoDeSwarmConfiguration::ParseSwarmConfiguration(const std::string& str_fsm_config) {
    // Splitting string into vector of tokens
    std::istringstream iss(str_fsm_config);
    std::vector<std::string> tokens;
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(tokens));

		ParseSwarmConfiguration(tokens);
  }

  /****************************************/
  /****************************************/

  void AutoMoDeSwarmConfiguration::ParseSwarmConfiguration(std::vector<std::string>& vec_fsm_config) {
    // Collecting relevant values
    std::vector<std::string>::iterator it;
    try {
      it = std::find(vec_fsm_config.begin(), vec_fsm_config.end(), "--robots");
      m_unNumberOfRobots = atoi((*(it+1)).c_str());
      it = std::find(vec_fsm_config.begin(), vec_fsm_config.end(), "--rabsi");
      m_unRabSensorIndex = atoi((*(it+1)).c_str());
			if (m_unRabSensorIndex != 0) {
				m_bRabEquipped = true;
				it = std::find(vec_fsm_config.begin(), vec_fsm_config.end(), "--rabar");
	      if (it != vec_fsm_config.end()) {
	      	m_fRabActuatorRange = strtod((*(it+1)).c_str(), NULL);
				}
			} else {
				m_bRabEquipped = false;
			}
		} catch (std::exception e) {
      LOGERR << e.what() << std::endl;
      THROW_ARGOSEXCEPTION("Error while parsing Hardware Modules configuration");
    }

		/* RAB Sensor Battery Time (in seconds) */
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(0, 0));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(1, 20));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(2, 30));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(3, 40));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(4, 50));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(5, 60));
		m_mapRABSensorBatteryConsumption.insert(std::make_pair(6, 70));

		/* RAB Actuator Range Battery Time (in seconds) */
		m_mapRABRangeBatteryConsumption.insert(std::make_pair(0.6, 10));
		m_mapRABRangeBatteryConsumption.insert(std::make_pair(0.7, 12));
		m_mapRABRangeBatteryConsumption.insert(std::make_pair(0.8, 14));
		m_mapRABRangeBatteryConsumption.insert(std::make_pair(0.9, 16));
		m_mapRABRangeBatteryConsumption.insert(std::make_pair(1.0, 20));

		if (m_bRabEquipped) {
			m_unRabBatteryConsumption = (m_mapRABSensorBatteryConsumption.find(m_unRabSensorIndex)->second) + (m_mapRABRangeBatteryConsumption.find(m_fRabActuatorRange)->second);
			} else {
			m_unRabBatteryConsumption = 0.0;
		}
	}

  /****************************************/
  /****************************************/

  const UInt8& AutoMoDeSwarmConfiguration::GetRabSensorIndex() const {
    return m_unRabSensorIndex;
  }

  /****************************************/
  /****************************************/

  const Real& AutoMoDeSwarmConfiguration::GetRabActuatorRange() const {
    return m_fRabActuatorRange;
  }

  /****************************************/
  /****************************************/

  const UInt32& AutoMoDeSwarmConfiguration::GetNumberOfRobots() const {
		return m_unNumberOfRobots;
	}

	/****************************************/
	/****************************************/
  const Real& AutoMoDeSwarmConfiguration::GetRabBatteryConsumption() const {
	  return m_unRabBatteryConsumption;
	}
}
