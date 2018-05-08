/*
 * @file <src/core/AutoMoDeSwarmConfiguration.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 *
 * @brief This class contains the information about the swarm of robots.
 *
 *
 */


#ifndef AUTOMODE_SWARM_CONFIGURATION_H
#define AUTOMODE_SWARM_CONFIGURATION_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <algorithm>
#include <map>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {
	class AutoMoDeSwarmConfiguration {
		public:
			/*
			 *  Class constructor.
       */
			AutoMoDeSwarmConfiguration();

			/*
			 * Class destructor.
       */
			virtual ~AutoMoDeSwarmConfiguration();

      void ParseSwarmConfiguration(const std::string& str_fsm_config);

      void ParseSwarmConfiguration(std::vector<std::string>& vec_fsm_config);

      const UInt32& GetNumberOfRobots() const;

      const Real& GetRabActuatorRange() const;

      const UInt8& GetRabSensorIndex() const;

      const UInt32& GetSwarmPrice() const;

      const Real& GetRabBatteryConsumption() const;

    private:

      UInt32 m_unNumberOfRobots;

			bool m_bRabEquipped;

      UInt8 m_unRabSensorIndex;

      Real m_fRabActuatorRange;

			Real m_unRabBatteryConsumption;

			std::map<UInt32, UInt32> m_mapRABSensorBatteryConsumption;

			std::map<Real, UInt32> m_mapRABRangeBatteryConsumption;

  };
}

#endif
