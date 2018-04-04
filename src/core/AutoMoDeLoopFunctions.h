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
     * The number of robots to be placed for the experiment.
     */
    UInt32 m_unNumberRobots;

    /*
    * The Battery Life
    */
    UInt32 m_unBatteryLife;

    /*
    * The Max experiment time (in ticks (seconds * ticks_per_second))
    */
    UInt32 m_unMaxExpTime;

    /*
     * The radius of the circle in which the robots will be distributed.
     */
    Real m_fDistributionRadius;

    CRandom::CRNG* m_pcRng;

  public:
    /*
     * Method used to create and distribute the robots.
     * The position is given by the method GetRandomPosition().
     */
    void PositionRobots();

    /*
     * This method should return the fitness of the controller.
     */
    virtual Real GetObjectiveFunction() = 0;

    /*
     * Return a random position.
     */
    virtual CVector3 GetRandomPosition() = 0;


    void SetNumberRobots(UInt32 un_number_robots);

    /*
     * Set Battery Life (in Seconds)
     */
    void SetBatteryLife(UInt32 un_battery_life);

    /*
    * Set Max experiment time (in ticks (seconds * ticks_per_second)).
    */
    void SetMaxExpTime(UInt32 un_max_exp_time);

};

#endif
