/*
 * @file <src/core/AutoMoDeCamRabBuffer.cpp>
 *
 * @author Fernando Mendiburu - <fmendibu@ulb.ac.be>
 *
 * @package AutoMoDe-Mate
 *
 * @license MIT License
 */


#include "AutoMoDeCamRabBuffer.h"

namespace argos {

  /****************************************/
  /****************************************/

  AutoMoDeCamRabBuffer::AutoMoDeCamRabBuffer() {
    m_unCurrentTime = 0;
  }

  /****************************************/
  /****************************************/

  AutoMoDeCamRabBuffer::~AutoMoDeCamRabBuffer() {}

  /****************************************/
  /****************************************/

  void AutoMoDeCamRabBuffer::SetTimeLife(const UInt32& un_max_time_to_live) {
    m_unMaxTimeToLive = un_max_time_to_live;
  }

  /****************************************/
  /****************************************/

  void AutoMoDeCamRabBuffer::Update() {
    if (m_unCurrentTime >= m_unMaxTimeToLive) {
      UInt32 i = 0;
      while (i < m_vecBufferElements.size()) {
        if (m_vecBufferElements.at(i).second < (m_unCurrentTime - m_unMaxTimeToLive)) {
          m_vecBufferElements.erase(m_vecBufferElements.begin() + i);
        } else {
          i += 1;
        }
      }
    }
    m_unCurrentTime += 1;
  }

  /****************************************/
	/****************************************/

  void AutoMoDeCamRabBuffer::AddMessage(CCI_EPuckVirtualCamrabSensor::SReading* c_readings) {
    m_vecBufferElements.push_back(std::make_pair(*c_readings, m_unCurrentTime));
  }

  /****************************************/
  /****************************************/

  std::vector<CCI_EPuckVirtualCamrabSensor::SReading*> AutoMoDeCamRabBuffer::GetMessages(){
    std::vector<CCI_EPuckVirtualCamrabSensor::SReading*> vecCamRabMessages;
    std::vector<std::pair<CCI_EPuckVirtualCamrabSensor::SReading, UInt32> >::iterator it;
    for (it = m_vecBufferElements.begin(); it != m_vecBufferElements.end(); it++) {
      vecCamRabMessages.push_back(&(*it).first);
    }
    return vecCamRabMessages;
  }

  /****************************************/
  /****************************************/

  void AutoMoDeCamRabBuffer::Reset() {
    m_vecBufferElements.clear();
    m_unCurrentTime = 0;
  }

}
