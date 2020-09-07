/*
 * @file <src/modules/AutoMoDeBehaviour.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDeBehaviour.h"


namespace argos {

	AutoMoDeBehaviour::~AutoMoDeBehaviour() {}

	/****************************************/
	/****************************************/

	const bool AutoMoDeBehaviour::IsLocked() const {
		return m_bLocked;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviour::SetIndex(const UInt32& un_index) {
		m_unIndex = un_index;
	}

	/****************************************/
	/****************************************/

	const UInt32& AutoMoDeBehaviour::GetIndex() const {
		return m_unIndex;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviour::SetIdentifier(const UInt32& un_id) {
		m_unIdentifier = un_id;
	}

	/****************************************/
	/****************************************/

	const UInt32& AutoMoDeBehaviour::GetIdentifier() const {
		return m_unIdentifier;
	}

	/****************************************/
	/****************************************/

	const std::string AutoMoDeBehaviour::GetDOTDescription() {
		std::stringstream ss;
		ss << m_strLabel;
		if (!m_mapParameters.empty()) {
			std::map<std::string, Real>::iterator it;
			for (it = m_mapParameters.begin(); it != m_mapParameters.end(); it++) {
				ss << "\\n" << it->first << "=" << it->second ;
			}
		}
		return ss.str();
	}

	/****************************************/
	/****************************************/

    CVector2 AutoMoDeBehaviour::MILowLevelController(CVector2 c_vector_to_follow) {

        Real gain_v = 1.0;
        Real gain_w = 0.7;
        Real EpuckVmax = m_pcRobotDAO->GetMaxVelocity();

        /* Get the heading angle */
        CRadians fVectorAngle = c_vector_to_follow.Angle().SignedNormalize();

        /* vector matching the orientation of the focal robot */
        CVector2 vFocalRobot;
        vFocalRobot.Set(1.0f,0.0f);
        /* Get the heading angle */
        CRadians AngleFocalRobot = vFocalRobot.Angle().SignedNormalize();

        Real fVelLinear, fOmega;

        /* Avoid that robots go backwards*/
        if (vFocalRobot.DotProduct(c_vector_to_follow) >= 0) {
            fVelLinear = gain_v*(vFocalRobot.DotProduct(c_vector_to_follow))*EpuckVmax;
        }
        else fVelLinear = 0;

        /* Proportional controller */
        fOmega = gain_w*(fVectorAngle.GetValue() - AngleFocalRobot.GetValue());


        /* Limit wheels velocities if are higher (lower) than the max
         * (min) velocity, trying to ensure angular velocity against
         * the linear velocity.
         */
        CVector2 vLimitWheelsVelocity;
        vLimitWheelsVelocity = LimitMotorsUniToDiff(fVelLinear,fOmega);

        return vLimitWheelsVelocity;
    }


    /****************************************/
    /****************************************/

    CVector2 AutoMoDeBehaviour::LimitMotorsUniToDiff(Real fVel, Real fOmega) {

        Real EpuckVmax = m_pcRobotDAO->GetMaxVelocity();
        Real EpuckWmax = m_pcRobotDAO->GetMaxOmega();

        /* Limit the linear velocity (fVel) and the angular
         * velocity (fOmega) to their max values appying a saturation
         */
        fOmega = Max<Real>(Min(fOmega,EpuckWmax), -EpuckWmax);
        fVel = Max<Real>(Min(fVel,EpuckVmax), -EpuckVmax);

        /* Compute desired wheels velocities (vl_d, vr_d) needed
         * to ensure fOmega
         */
        CVector2 vVelWheelsDesired = UniToDiff(fVel,fOmega);

        /* Find the max and min wheels velocities (vel_r/vel_l)
         */
        Real fMaxVelocity_rl = Max<Real>(vVelWheelsDesired.GetX(),vVelWheelsDesired.GetY());
        Real fMinVelocity_rl = Min<Real>(vVelWheelsDesired.GetX(),vVelWheelsDesired.GetY());

        /* If the wheels velocities are higher (lower) than the max (min)
         * linear velocity, then decrease (increase) the wheels velocities
         */
        Real fDiffSup = fMaxVelocity_rl - EpuckVmax;
        Real fDiffInf = fMinVelocity_rl + EpuckVmax;

        CVector2 fVelWheelsLim;

        if (fMaxVelocity_rl > EpuckVmax) {
            fVelWheelsLim.Set(vVelWheelsDesired.GetX()-fDiffSup, vVelWheelsDesired.GetY()-fDiffSup);
        }
        else if (fMinVelocity_rl < -EpuckVmax) {
            fVelWheelsLim.Set(vVelWheelsDesired.GetX()-fDiffInf,vVelWheelsDesired.GetY()-fDiffInf);
        }
        else {
            fVelWheelsLim.Set(vVelWheelsDesired.GetX(),vVelWheelsDesired.GetY());
        }

        return fVelWheelsLim;
    }

    /****************************************/
    /****************************************/

    CVector2 AutoMoDeBehaviour::UniToDiff(Real fVel, Real fOmega) {

        /* This function transform from the linear and angular velocity
         * (fVel,fOmega) to wheels velocities using the unicycle model
         */

        Real fDistanceWheels = m_pcRobotDAO->GetLengthEpuckAxis();
        CVector2 fVelWheels;

        fVelWheels.Set((2.0f*fVel-fOmega*fDistanceWheels)*0.5f,(2.0f*fVel+fOmega*fDistanceWheels)*0.5f);

        return fVelWheels;
    }

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviour::AddParameter(const std::string& str_identifier, const Real& f_value) {
		m_mapParameters.insert(std::pair<std::string, Real>(str_identifier, f_value));
	}

	/****************************************/
	/****************************************/

	const bool AutoMoDeBehaviour::IsOperational() const {
		return m_bOperational;
	}

	/****************************************/
	/****************************************/

	const std::string& AutoMoDeBehaviour::GetLabel() const {
		return m_strLabel;
	}

	/****************************************/
	/****************************************/

	const std::map<std::string, Real> AutoMoDeBehaviour::GetParameters() {
		return m_mapParameters;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviour::SetRobotDAO(EpuckDAO* pc_robot_dao) {
		m_pcRobotDAO = pc_robot_dao;
	}

}
