/*
 * @file <src/core/AutoMoDeCamRabBuffer.h>
 *
 * @author Fernando Mendiburu - <fmendibu@ulb.ac.be>
 *
 * @package AutoMoDe-Mate
 *
 * @license MIT License
 *
 * @brief Container that keep CamRab messages for a fixed
 *        period of time. After the time to live of a message is exceded, the
 *        message is discarded.
 */

#ifndef AUTOMODE_CAMRAB_BUFFER_H
#define AUTOMODE_CAMRAB_BUFFER_H

#include <vector>
#include <algorithm>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_sensor.h>

namespace argos {
  class AutoMoDeCamRabBuffer {
    public:
      /*
			 * Class constructor.
       */
      AutoMoDeCamRabBuffer();

      /*
			 * Class destructor.
       */
      ~AutoMoDeCamRabBuffer();

      /*
			 * Setter for the time to live of the messages contained in the buffer.
       */
      void SetTimeLife(const UInt32& un_max_time_to_live);

      /*
			 * Removes the messages that are too old.
       */
      void Update();

      /*
             * Add a message to the CamRab buffer.
       */
      void AddMessage(CCI_EPuckVirtualCamrabSensor::SReading* c_readings);

      /*
             * Returns all the CamRab messages of the buffer.
       */
      std::vector<CCI_EPuckVirtualCamrabSensor::SReading*> GetMessages();

      /*
			 * Clears the content of the buffer.
       */
      void Reset();


    private:
      /*
			 * Current time step.
       */
      UInt32 m_unCurrentTime;

      /*
			 * Time to live of the messages of the buffer.
       */
      UInt32 m_unMaxTimeToLive;

      /*
             * List of pairs <CamRab message, timestamp>. Represents the buffer.
       */
      std::vector<std::pair<CCI_EPuckVirtualCamrabSensor::SReading, UInt32> > m_vecBufferElements;
  };
}

#endif
