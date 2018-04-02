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

    // Collecting relevant values
    std::vector<std::string>::iterator it;
		try {
			it = std::find(tokens.begin(), tokens.end(), "--robots");
			m_unNumberOfRobots = atoi((*(it+1)).c_str());
			it = std::find(tokens.begin(), tokens.end(), "--rabsi");
			m_unRabSensorIndex = atoi((*(it+1)).c_str());
			it = std::find(tokens.begin(), tokens.end(), "--rabar");
			m_fRabActuatorRange = strtod((*(it+1)).c_str(), NULL);
		} catch (std::exception e) {
			LOGERR << e.what() << std::endl;
			THROW_ARGOSEXCEPTION("Error while parsing Hardware Modules configuration");
		}
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
      it = std::find(vec_fsm_config.begin(), vec_fsm_config.end(), "--rabar");
      m_fRabActuatorRange = strtod((*(it+1)).c_str(), NULL);
    } catch (std::exception e) {
      LOGERR << e.what() << std::endl;
      THROW_ARGOSEXCEPTION("Error while parsing Hardware Modules configuration");
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

}
