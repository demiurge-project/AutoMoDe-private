/*
 * @file <src/AutoMoDeMain.cpp>
 *
 * @author Antoine Ligot and Muhamed Salman - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/core/simulator/argos_command_line_arg_parser.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include "./core/AutoMoDeFiniteStateMachine.h"
#include "./core/AutoMoDeFsmBuilder.h"
#include "./core/AutoMoDeLoopFunctions.h"
#include "./core/AutoMoDeController.h"
#include "./core/AutoMoDeSwarmConfiguration.h"

using namespace argos;

const std::string ExplainParameters() {
	std::string strExplanation = "Missing finite state machine configuration. The possible parameters are: \n\n"
		" -r | --readable-fsm \t Prints an URL containing a DOT representation of the finite state machine [OPTIONAL]. \n"
		" -s | --seed \t The seed for the ARGoS simulator [OPTIONAL] \n"
		" --fsm-config CONF \t The finite state machine description [MANDATORY]\n"
		"\n The description of the finite state machine should be placed at the end of the command line, after the other parameters.";
	return strExplanation;
}

/**
 * @brief
 *
 */
int main(int n_argc, char** ppch_argv) {

	bool bHistory = true;
	bool bReadableFSM = false;
	std::vector<std::string> vecConfigFsm;
	bool bFsmControllerFound = false;
	UInt32 unSeed = 0;

	std::vector<AutoMoDeFiniteStateMachine*> vecFsm;

	try {
		// Cutting off the FSM configuration from the command line

		int nCurrentArgument = 1;
		while(!bFsmControllerFound && nCurrentArgument < n_argc) {
			if(strcmp(ppch_argv[nCurrentArgument], "--fsm-config") == 0) {
				bFsmControllerFound = true;
				nCurrentArgument++;
				while (nCurrentArgument < n_argc) {
					vecConfigFsm.push_back(std::string(ppch_argv[nCurrentArgument]));
					nCurrentArgument++;
				}
				// Do not take the FSM configuration into account in the standard command line parsing.
				n_argc = n_argc - vecConfigFsm.size() - 1;
			}
			nCurrentArgument++;
		}
		if (!bFsmControllerFound) {
			THROW_ARGOSEXCEPTION(ExplainParameters());
		}

		AutoMoDeSwarmConfiguration* pcSwarmConfiguration = new AutoMoDeSwarmConfiguration();
		pcSwarmConfiguration->ParseSwarmConfiguration(vecConfigFsm);

		// Configure the command line options
		CARGoSCommandLineArgParser cACLAP;
		cACLAP.AddFlag('r', "readable-fsm", "", bReadableFSM);

		cACLAP.AddFlag('t', "history", "", bHistory);

		cACLAP.AddArgument<UInt32>('s', "seed", "", unSeed);

		// Parse command line without taking the configuration of the FSM into account
		cACLAP.Parse(n_argc, ppch_argv);

		CSimulator& cSimulator = CSimulator::GetInstance();

		switch(cACLAP.GetAction()) {
    	case CARGoSCommandLineArgParser::ACTION_RUN_EXPERIMENT: {
				CDynamicLoading::LoadAllLibraries();
				cSimulator.SetExperimentFileName(cACLAP.GetExperimentConfigFile());

				// Creation of the finite state machine.
				AutoMoDeFsmBuilder cBuilder = AutoMoDeFsmBuilder();
				AutoMoDeFiniteStateMachine* pcFiniteStateMachine = cBuilder.BuildFiniteStateMachine(vecConfigFsm);

				// If the URL of the finite state machine is requested, display it.
				if (bReadableFSM) {
					std::cout << "Finite State Machine description: " << std::endl;
					std::cout << pcFiniteStateMachine->GetReadableFormat() << std::endl;
				}

				// Setting random seed. Only works with modified version of ARGoS3.
				cSimulator.SetRandomSeed(unSeed);
				cSimulator.LoadExperiment();

				AutoMoDeLoopFunctions& cLoopFunctions = dynamic_cast<AutoMoDeLoopFunctions&> (cSimulator.GetLoopFunctions());
				cLoopFunctions.SetNumberRobots(pcSwarmConfiguration->GetNumberOfRobots());
				cLoopFunctions.PositionRobots();

				// Duplicate the finite state machine and pass it to all robots.
				CSpace::TMapPerType cEntities = cSimulator.GetSpace().GetEntitiesByType("controller");
				for (CSpace::TMapPerType::iterator it = cEntities.begin(); it != cEntities.end(); ++it) {
					CControllableEntity* pcEntity = any_cast<CControllableEntity*>(it->second);
					AutoMoDeFiniteStateMachine* pcPersonalFsm = new AutoMoDeFiniteStateMachine(pcFiniteStateMachine);
					vecFsm.push_back(pcPersonalFsm);
					try {
						AutoMoDeController& cController = dynamic_cast<AutoMoDeController&> (pcEntity->GetController());
						cController.SetFiniteStateMachine(pcPersonalFsm);
						cController.SetHistoryFlag(bHistory);
						cController.SetSwarmConfiguration(pcSwarmConfiguration);
						cController.InitializeHardwareModules();
					} catch (std::exception& ex) {
						LOGERR << "Error while casting: " << ex.what() << std::endl;
					}
				}

				// Setting modules parameters.
				CSpace::TMapPerType cEpuckEntities = cSimulator.GetSpace().GetEntitiesByType("epuck");
				for (CSpace::TMapPerType::iterator it = cEpuckEntities.begin(); it != cEpuckEntities.end(); ++it) {
					CEPuckEntity* pcEpuckEntity = any_cast<CEPuckEntity*>(it->second);
					(pcEpuckEntity->GetRABEquippedEntity()).SetRange(pcSwarmConfiguration->GetRabActuatorRange());
				}

				cSimulator.Execute();

				// Retrieval of the score of the swarm driven by the Finite State Machine
				Real fObjectiveFunction = cLoopFunctions.GetObjectiveFunction();
				std::cout << "Score " << fObjectiveFunction << std::endl;

				break;
			}

    	case CARGoSCommandLineArgParser::ACTION_QUERY:
        CDynamicLoading::LoadAllLibraries();
        //QueryPlugins(cACLAP.GetQuery());
        break;
    	case CARGoSCommandLineArgParser::ACTION_SHOW_HELP:
        cACLAP.PrintUsage(LOG);
        break;
		 	case CARGoSCommandLineArgParser::ACTION_SHOW_VERSION:
        cACLAP.PrintVersion();
        break;
      case CARGoSCommandLineArgParser::ACTION_UNKNOWN:
        // Should never get here
        break;
		}

		cSimulator.Destroy();

		for (unsigned int i = 0; i < vecFsm.size(); ++i) {
			delete vecFsm.at(i);
		}
		delete pcSwarmConfiguration;

	} catch(std::exception& ex) {
    // A fatal error occurred: dispose of data, print error and exit
    LOGERR << ex.what() << std::endl;
#ifdef ARGOS_THREADSAFE_LOG
    LOG.Flush();
    LOGERR.Flush();
#endif
    return 1;
  }

	/* Everything's ok, exit */
  return 0;
}
