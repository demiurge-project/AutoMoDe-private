  sensor_prices = c(0, 500, 600, 700, 800, 900, 1000)
  actuator_prices = c(0, 400, 400, 400, 600, 600)
  
  robot_price = 0
  max_robots = 30
  min_robots = 15
  
  file_name = "hardware_grammar.txt"
  
  sensor_values = c(0,1,2,3,4,5,6)
  actuator_values = c(0.0,0.6,0.7,0.8,0.9,1.0)
  
  total_budget <- 15000
  
  ###############################################
  ###############################################
  
  formatRobotConstraint <- function(robots_vec) {
    if (length(robots_vec) == 1) {
      return(paste("(as.numerics(NumRobots) == ", robots_vec[1], ")", sep =""))
    } else {
      max_nrobots <- robots_vec[1]
      min_nrobots <- robots_vec[length(robots_vec)]
      return(paste("((as.numerics(NumRobots) >= ", max_nrobots, ") && (as.numerics(NumRobots) <= ", min_nrobots, "))", sep=""))
    }
  }
  
  ###############################################
  ###############################################
  
  formatSensorIndexConstraint <- function(possible_sensors, robots_vec) {
    final_str <- "RabSI        \"--rabsi \"    o ("
    for (i in 1:length(possible_sensors)) {
      final_str <- paste(final_str, sensor_values[possible_sensors[i]], sep="")
      if (i != length(possible_sensors)) {
        final_str <- paste(final_str, ",", sep="")
      }
    }
    final_str <- paste(final_str, ") | ", formatRobotConstraint(robots_vec), sep="")
    return(final_str)
  }
  
  ###############################################
  ###############################################
  
  formatActuatorRangeConstraint <- function(sensor_index, possible_actuators, robots_vec) {
    final_str <- "RabAR        \"--rabar \"    o ("
    for (i in 1:length(possible_actuators)) {
      final_str <- paste(final_str, actuator_values[possible_actuators[i]], sep="")
      if (i != length(possible_actuators)) {
        final_str <- paste(final_str, ",", sep="")
      }
    }
    sensor_constraint <- paste("(as.numerics(RabSI) == ", sensor_values[sensor_index], ")", sep="")
    final_str <- paste(final_str, ") | ", formatRobotConstraint(robots_vec), " && ", sensor_constraint, sep="")
    return(final_str)
  }
  
  ###############################################
  ###############################################
  
  matrixToSensorConstraints <- function(conf_matrix, robots_vec) {
    #print(robots_vec)
    #print(conf_matrix)
    pos_sensors <- c()
    for (s in 1:7) {
      for (a in 1:6) {
        if (conf_matrix[s,a] == 1) {
          pos_sensors <- c(pos_sensors, s)
        }      
      }
    }
    if (length(pos_sensors) > 0) {
      write(formatSensorIndexConstraint(pos_sensors[!duplicated(pos_sensors)], robots_vec), file_name, append=TRUE, sep="\n")
      #print(formatSensorIndexConstraint(pos_sensors[!duplicated(pos_sensors)], robots_vec))
    }
  }
  
  ###############################################
  ###############################################
  
  matrixToActuatorConstraints <- function(conf_matrix, robots_vec) {
    for (s in 1:7) {
      pos_actuators <- c()
      for (a in 1:6) {
        if (conf_matrix[s,a] == 1) {
          pos_actuators <- c(pos_actuators, a)
        }      
      }
      if (length(pos_actuators) > 0) {
        write(formatActuatorRangeConstraint(s, pos_actuators[!duplicated(pos_actuators)], robots_vec), file_name, append=TRUE, sep="\n")
        #print(formatActuatorRangeConstraint(s, pos_actuators[!duplicated(pos_actuators)], robots_vec))
      }
    }
  }
  
  ###############################################
  ###############################################
  
  equalMatrix <- function(a, b) {
    for (i in 1:7) {
      for (j in 1:6) {
        if (a[i,j] != b[i,j]) {
          return(FALSE)
        }
      }
    }
    return(TRUE)
  }
  
  ###############################################
  ###############################################
  
  write("", file_name, append=FALSE)
  

  for (i in 1:2) {
    previous_cost <- 0
    previous_robots <- 0
    num_robots <- c()
    for (nrobots in min_robots:max_robots) {
      current_robots <- nrobots
      current_cost <- matrix(0, nrow=7, ncol=6)
      for (a in 1:7) {
        for (b in 1:6) {
          current_cost[a,b] <- nrobots * (sensor_prices[a] + actuator_prices[b])
        }
      }
      current_cost[ current_cost <= total_budget] <- 1
      current_cost[ current_cost > total_budget] <- 0
      if (previous_cost != 0) {
        if (equalMatrix(current_cost, previous_cost)) {
          #print(paste(previous_robots, ' ', current_robots))
          num_robots <- c(num_robots, nrobots)
        } else if (length(num_robots) != 0) {
          if (i == 1) {
            matrixToSensorConstraints(previous_cost, num_robots)  
          } else {
            matrixToActuatorConstraints(previous_cost, num_robots)
          }
          num_robots <- c(nrobots)
        }
      } else {
        num_robots <- c(num_robots, nrobots)
      }
      
      if (nrobots == max_robots) {
        if (i == 1) {
          matrixToSensorConstraints(current_cost, num_robots)  
        } else {
          matrixToActuatorConstraints(current_cost, num_robots)
        }
      }
      
      previous_cost = current_cost
      previous_robots = current_robots
      #heatmap(cost, scale = "none", Rowv=NA, Colv=NA, col = cm.colors(2), main = nrobots)
    }
  }
  print("Done ...")
