/*
 * @file <src/core/AutoMoDeRobotDAO.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDeRobotDAO.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeRobotDAO::AutoMoDeRobotDAO() {
		m_pcRng = CRandom::CreateRNG("argos");
        m_pcCamRabMessageBuffer = AutoMoDeCamRabBuffer();
        m_pcCamRabMessageBuffer.SetTimeLife(3);
        m_fMaxVelocity = 10;                // [cm/s]
        m_fMaxOmega = 0.3f*m_fMaxVelocity;  // 4.5 [r/s] max
        m_fLengthEpuckAxis = 5.3f;          // [cm]
        m_fLeftWheelVelocity = 0;           // [cm/s]
        m_fRightWheelVelocity = 0;          // [cm/s]
	}

	/****************************************/
	/****************************************/

	AutoMoDeRobotDAO::~AutoMoDeRobotDAO() {}

	/****************************************/
	/****************************************/

	CCI_EPuckProximitySensor::TReadings AutoMoDeRobotDAO::GetProximityInput() const {
		return m_sProximityInput;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetProximityInput(CCI_EPuckProximitySensor::TReadings s_prox_input) {
		m_sProximityInput = s_prox_input;
	}

	/****************************************/
	/****************************************/

	CCI_EPuckLightSensor::TReadings AutoMoDeRobotDAO::GetLightInput() const{
		return m_sLightInput;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetLightInput(CCI_EPuckLightSensor::TReadings s_light_input) {
		m_sLightInput = s_light_input;
	}

	/****************************************/
	/****************************************/

	CCI_EPuckGroundSensor::SReadings AutoMoDeRobotDAO::GetGroundInput() const{
		return m_sGroundInput;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetGroundInput(CCI_EPuckGroundSensor::SReadings s_ground_input) {
		m_sGroundInput = s_ground_input;
	}

	/****************************************/
	/****************************************/

	const UInt8& AutoMoDeRobotDAO::GetNumberNeighbors() const {
		return m_unNumberNeighbors;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetNumberNeighbors(const UInt8& un_number_neighbors){
		m_unNumberNeighbors = un_number_neighbors;
	}

	/****************************************/
	/****************************************/

    std::vector<CCI_EPuckVirtualCamrabSensor::SReading*> AutoMoDeRobotDAO::GetCamRabMessages() {
        return m_pcCamRabMessageBuffer.GetMessages();
    }

	/****************************************/
	/****************************************/

    void AutoMoDeRobotDAO::SetCamRabMessages(CCI_EPuckVirtualCamrabSensor::TReadings s_packets) {
        std::map<UInt32, CCI_EPuckVirtualCamrabSensor::SReading*> mapRemainingMessages;
        std::map<UInt32, CCI_EPuckVirtualCamrabSensor::SReading*>::iterator mapIt;
        CCI_EPuckVirtualCamrabSensor::TReadings::iterator it;
        m_unNumberNeighbors = 0;

        for (it = s_packets.begin(); it < s_packets.end(); ++it) {
            if ((*it)->Data[1] != m_unRobotIdentifier) {

                if ((*it)->Distance != 0.0f){
                    mapRemainingMessages[(*it)->Data[1]] = (*it);
                }
                else {
                    /* if (*it)->Distance = 0, this measure could be between 30-70cm from de R&B measure */
                    (*it)->Distance = 50;
                    mapRemainingMessages[(*it)->Data[1]] = (*it);
                }

            }
        }
        for (mapIt = mapRemainingMessages.begin(); mapIt != mapRemainingMessages.end(); ++mapIt) {
            m_pcCamRabMessageBuffer.AddMessage((*mapIt).second);
            m_unNumberNeighbors += 1;
        }

        m_pcCamRabMessageBuffer.Update();

    }

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetWheelsVelocity(const Real& un_left_velocity, const Real& un_right_velocity) {
		m_fLeftWheelVelocity = un_left_velocity;
		m_fRightWheelVelocity = un_right_velocity;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetWheelsVelocity(const CVector2& c_velocity_vector) {
		m_fLeftWheelVelocity = c_velocity_vector.GetX();
		m_fRightWheelVelocity = c_velocity_vector.GetY();
	}

	/****************************************/
	/****************************************/

	const Real& AutoMoDeRobotDAO::GetRightWheelVelocity() const {
		return m_fRightWheelVelocity;
	}

	/****************************************/
	/****************************************/

	const Real& AutoMoDeRobotDAO::GetLeftWheelVelocity() const {
		return m_fLeftWheelVelocity;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::Reset() {
		m_fLeftWheelVelocity = 0;
		m_fRightWheelVelocity = 0;
        m_pcCamRabMessageBuffer.Reset();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeRobotDAO::SetRobotIdentifier(const UInt32& un_robot_id) {
		m_unRobotIdentifier = un_robot_id;
	}

	/****************************************/
	/****************************************/

	const UInt32& AutoMoDeRobotDAO::GetRobotIdentifier() const{
		return m_unRobotIdentifier;
	}

	/****************************************/
	/****************************************/

	CRandom::CRNG* AutoMoDeRobotDAO::GetRandomNumberGenerator() const {
		return m_pcRng;
	}

	/****************************************/
	/****************************************/

	const Real& AutoMoDeRobotDAO::GetMaxVelocity() const{
		return m_fMaxVelocity;
	}

    /****************************************/
    /****************************************/

    const Real& AutoMoDeRobotDAO::GetMaxOmega() const{
        return m_fMaxOmega;
    }

    /****************************************/
    /****************************************/

    const Real& AutoMoDeRobotDAO::GetLengthEpuckAxis() const{
        return m_fLengthEpuckAxis;
    }


}
