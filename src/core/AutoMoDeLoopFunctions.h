/*
 * @file <src/core/AutoMoDeLoopFunctions.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#ifndef AUTOMODE_LOOP_FUNCTIONS
#define AUTOMODE_LOOP_FUNCTIONS

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>
#include <argos3/plugins/robots/arena/simulator/arena_entity.h>

using namespace argos;

class AutoMoDeLoopFunctions: public CLoopFunctions {
  protected:

    ~AutoMoDeLoopFunctions();

    /*
     * Initialization method where the parameters of the loop function
     * are fetched from the xml declaration.
     */
    virtual void Init(argos::TConfigurationNode& t_tree);

    /*
     * Reset function. First remove the robots, then re-position them.
     */
    virtual void Reset();

    /*
     * Method used to remove the robots from the arena.
     */
    void RemoveRobots();

    /*
     * Method used to reallocate the robots.
     * The position is given by the method GetRandomPosition().
     */
    void MoveRobots();

    /*
     * Method used to create and distribute the robots.
     * The position is given by the method GetRandomPosition().
     */
    void PositionRobots();

    /*
     * Method used to remove the walls from the arena.
     */
    void RemoveBoxes();

    /*
     * Method used to reallocate the walls.
     * The position is given by the method GetBoxesPositions().
     */
    void MoveBoxes();

    /*
     * Method used to create and distribute the walls.
     * The position is given by the method GetBoxesPositions().
     */
    void PositionBoxes();

    /*
     * Method used to create and distribute the Arena.
     */
    void PositionArena();

    /*
     * Method used to deternmine wheter a number is even.
     */
    bool IsEven(UInt32 unNumber);

    /*
     * The number of robots to be placed for the experiment.
     */
    UInt32 m_unNumberRobots;

    /*
     * The radius of the circle in which the robots will be distributed.
     */
    Real m_fDistributionRadius;

    /*
     * Build the arena with the arena_entity plugin.
     */
    bool m_bBuildArena;

    /*
     * The number of edges in the arena used in the experiment.
     */
    UInt32 m_unNumberEdges;

    /*
     * The number of boxes in each edge of the arena used in the experiment.
     */
    UInt32 m_unNumberBoxes;

    /*
     * The lenght of the boxes used in the experiment.
     */
    Real m_fLenghtBoxes;

    /*
     * The arena used in the experiment.
     */    
    CArenaEntity* m_pcArena;

    /*
     * The position of the boxes in the arena used in the experiment.
     */
    CRandom::CRNG* m_pcRng;

    /*
     * Selector of the piecewise function configuration
     */
    UInt32 m_unPwConfig;



  public:

    /*
     * This method should return the fitness of the controller.
     */
    virtual Real GetObjectiveFunction() = 0;

    /*
     * Return a random position.
     */
    virtual CVector3 GetRandomPosition() = 0;

    /*
     * Return the radious of the arena.
     */
    Real GetArenaRadious();

};

#endif
