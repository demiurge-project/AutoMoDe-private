/*
 * @file <src/core/AutoMoDeController.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#ifndef AUTOMODE_CONTROLLER_H
#define AUTOMODE_CONTROLLER_H

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_controller.h>

#include "./AutoMoDeFiniteStateMachine.h"
#include "./AutoMoDeFsmBuilder.h"
#include "./AutoMoDeRobotDAO.h"

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>

namespace argos{
	class AutoMoDeController: public CCI_Controller {
		public:
			/*
			 * Class constructor.
			 */
			AutoMoDeController();

			/*
			 * Class desctructor.
			 */
			virtual ~AutoMoDeController();

			/*
			 * Controller initializer.
			 */
			virtual void Init(TConfigurationNode& t_node);

			/*
			 * Core of the controller.
			 */
			virtual void ControlStep();

			/*
			 *
			 */
			virtual void Reset();

			/*
			 *
			 */
			virtual void Destroy();

			/*
			 * Setter for the AutoMoDeFiniteStateMachine.
			 */
			void SetFiniteStateMachine(AutoMoDeFiniteStateMachine* pc_fine_state_machine);

		private:
			/*
			 * Pointer to the finite state machine object that represents the behaviour
			 * of the robot.
			 */
			AutoMoDeFiniteStateMachine* m_pcFiniteStateMachine;

			/*
			 * Pointer to the object representing the state of the robot. This object is
			 * shared with the finite state object AutoMoDeFiniteStateMachine.
			 */
			AutoMoDeRobotDAO* m_pcRobotState;

			/*
			 * Time step variable.
			 */
			UInt32 m_unTimeStep;

			/*
			 * Integer part of the robot identifier.
			 */
			UInt32 m_unRobotID;

			/*
			 * String that contains the configuration of the finite state machine.
			 */
			std::string m_strFsmConfiguration;

			/*
			 * Flag that tells whether an history is maintained or not.
			 */
			bool m_bMaintainHistory;

			/*
			 * Flag that tells whether an URL containing a DOT description of the
			 * finite state machine is to be displayed or not.
			 */
			bool m_bPrintReadableFsm;

			/*
			 * The path to where the history shall be stored.
			 */
			std::string m_strHistoryFolder;

			/*
			 * Pointer to the object in charge of creating the AutoMoDeFiniteStateMachine.
			 */
			AutoMoDeFsmBuilder* m_pcFsmBuilder;

			/*
			 * Pointer to the robot wheels actuator.
			 */
			CCI_EPuckWheelsActuator* m_pcWheelsActuator;

			/*
			 * Pointer to the robot range-and-bearing actuator.
			 */
			CCI_EPuckRangeAndBearingActuator* m_pcRabActuator;

			/*
			 * Pointer to the robot RGB LEDs actuator.
			 */
			CCI_EPuckRGBLEDsActuator* m_pcLEDsActuator;

			/*
			 * Pointer to the robot proximity sensor.
			 */
			CCI_EPuckProximitySensor* m_pcProximitySensor;

			/*
			 * Pointer to the robot range-and-bearing sensor.
			 */
			CCI_EPuckRangeAndBearingSensor* m_pcRabSensor;

			/*
			 * Pointer to the robot light sensor.
			 */
			CCI_EPuckLightSensor* m_pcLightSensor;

			/*
			 * Pointer to the robot ground sensor.
			 */
			CCI_EPuckGroundSensor* m_pcGroundSensor;

			/*
			 * Pointer to the robot omnidirectional camera sensor.
			 */
			CCI_EPuckOmnidirectionalCameraSensor* m_pcCameraSensor;

			bool m_bFiniteStateMachineGiven;
	};
}

#endif
