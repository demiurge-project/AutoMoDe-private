/**
  * @file <src/modules/AutoMoDeBehaviourHexagonal.cpp>
  *
  * @author Fernando Mendiburu - <fmendibu@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe-Mate
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourHexagonal.h"


namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourHexagonal::AutoMoDeBehaviourHexagonal() {
        m_strLabel = "Hexagonal";

	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourHexagonal::AutoMoDeBehaviourHexagonal(AutoMoDeBehaviourHexagonal* pc_behaviour) {
		m_strLabel = pc_behaviour->GetLabel();
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();

		Init();
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourHexagonal::~AutoMoDeBehaviourHexagonal() {}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourHexagonal* AutoMoDeBehaviourHexagonal::Clone() {
        return new AutoMoDeBehaviourHexagonal(this);
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourHexagonal::ControlStep() {

        CVector2 vProx(0,CRadians::ZERO);
        CVector2 vResult(0,CRadians::ZERO);


        if(m_pcRobotDAO->GetRobotIdentifier() == 1) {
            //PrintSensor(m_pcRobotDAO->GetCamRabMessages());
        }

        CVector2 vHexagonal = Hexagonal(m_pcRobotDAO->GetCamRabMessages());

        vProx = SumProximityReadings(m_pcRobotDAO->GetProximityInput());

        vResult = vHexagonal - 6*vProx;

        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(vResult));

        m_bLocked = false;
    }

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourHexagonal::Init() {

        m_sHexagonalParams.Exponent = 2;

        std::map<std::string, Real>::iterator itDist = m_mapParameters.find("dij");
        std::map<std::string, Real>::iterator itGain = m_mapParameters.find("ga");
        if ((itDist != m_mapParameters.end()) && (itGain != m_mapParameters.end())) {
            m_sHexagonalParams.TargetDistance = itDist->second;
            m_sHexagonalParams.Gain = itGain->second;

        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }


	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourHexagonal::Reset() {
		m_bOperational = false;

		ResumeStep();
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourHexagonal::ResumeStep() {
		m_bOperational = true;
	}

    /****************************************/
    /****************************************/

    CVector2 AutoMoDeBehaviourHexagonal::Hexagonal(CCI_EPuckVirtualCamrabSensor::TReadings sReadings) {


        CVector2 sCamRabVectorSum(0,CRadians::ZERO);

        UInt32 unNumMsg = 0;
        Real fLJ;

        if (!sReadings.empty()) {
            for(size_t i = 0; i < sReadings.size(); ++i) {

                if ((sReadings[i]->Data[1] != (UInt8) m_pcRobotDAO->GetRobotIdentifier())) {

                    /* Lennard-Jones interaction force */
                    fLJ = LJMagnitude(sReadings[i]->Distance, m_sHexagonalParams.TargetDistance, m_sHexagonalParams.Gain);

                    CRadians AngleRad;
                    AngleRad.FromValueInDegrees(sReadings[i]->Angle);

                    /* Sum the contributions of the neighbors */
                    sCamRabVectorSum += CVector2(fLJ,AngleRad.SignedNormalize());

                    ++unNumMsg;

                }
            }

            if(unNumMsg > 0) {
               /* Divide the accumulator by the number of messeges */
               sCamRabVectorSum /= unNumMsg;
               /* Limit the interaction force */
               if(sCamRabVectorSum.Length() > 1.0f) {
                  sCamRabVectorSum.Normalize();
               }
            }
            /* All done */
            return sCamRabVectorSum;
        }
        else {
            /* Alone */
            return CVector2();
        }

    }

    /****************************************/
    /****************************************/

    Real AutoMoDeBehaviourHexagonal::LJMagnitude(Real fDistance, Real fTargetDistance, Real fGain) {

        Real fExp = m_sHexagonalParams.Exponent;
        Real fDa = fTargetDistance / fDistance;
        //Real fExp = m_sSquareParams.Exponent;
        Real fDaN = ::pow(fDa,fExp);

        Real fMagnitude = -(fGain/fDistance)*(fDaN*fDaN - fDaN);

        return fMagnitude;

    }

    /****************************************/
    /****************************************/

    void AutoMoDeBehaviourHexagonal::PrintSensor(CCI_EPuckVirtualCamrabSensor::TReadings sReadings) {

        if (m_pcRobotDAO->GetRobotIdentifier() == 1) {

            if(! sReadings.empty()) {

                  LOG << "----------------------------CamRab-----------------------------" << std::endl;
                  for(size_t i = 0; i < sReadings.size(); ++i) {

                      CRadians anguloR;
                      anguloR.FromValueInDegrees(sReadings[i]->Angle);

                      LOG << "R" << m_pcRobotDAO->GetRobotIdentifier() << "R" << sReadings[i]->Data[1] << " Msg: " << i << " Dist: " << sReadings[i]->Distance << " Ang: " << anguloR.SignedNormalize().GetValue() << " Fus: "<< sReadings[i]->Fusion << std::endl;

                  }

                  LOG << "-------------------------------------------------------------" << std::endl;

               }
        }

    }



}




