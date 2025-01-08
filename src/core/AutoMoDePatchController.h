/*
 * @file <src/core/AutoMoDePatchController.h>
 *
 * @author Jeanne Szpirer - <jszpirer@ulb.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#ifndef AUTOMODE_PATCH_CONTROLLER_H
#define AUTOMODE_PATCH_CONTROLLER_H

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/demiurge/epuck-dao/EpuckDAO.h>
#include <argos3/demiurge/epuck-dao/ReferenceModel4Dot0.h>


#include "./AutoMoDeFiniteStateMachine.h"
#include "./AutoMoDeFsmBuilder.h"

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

namespace argos{
	class AutoMoDePatchController: public CCI_Controller {
		public:
			/*
			 * Class constructor.
			 */
			AutoMoDePatchController();

			/*
			 * Class desctructor.
			 */
			virtual ~AutoMoDePatchController();

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
			 *
			 */
			virtual void SetColor(UInt8 color);

		private:
			/*
			 * Function that contains all actuations required at the start of an experiment or during the entire experiment.
			 * Example of what you might add in the future: display LED colors, start omnidirectional camera, etc.
			 * This function needs to be called by Reset() in order for the experiment to properly restart.
			 */
			void InitializeActuation();

			/*
			 * Pointer to the object representing the state of the robot. This object is
			 * shared with the finite state object AutoMoDeFiniteStateMachine.
			 */
			EpuckDAO* m_pcRobotState;

			/*
			 * Time step variable.
			 */
			UInt32 m_unTimeStep;

			/*
			 * Integer part of the robot identifier.
			 */
			UInt32 m_unPatchID;

			/*
			 * String that contains the color of the patch.
			 */
			UInt8 m_color;

			/*
			 * Pointer to the robot range-and-bearing actuator.
			 */
			CCI_EPuckRangeAndBearingActuator* m_pcRabActuator;

			/*
			 * Pointer to the robot range-and-bearing sensor.
			 */
			CCI_EPuckRangeAndBearingSensor* m_pcRabSensor;
	};
}

#endif
