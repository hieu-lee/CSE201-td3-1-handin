#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double* arr = new double[new_size];
  for (int i = 0; i < length; i++)  {
      *(arr + i) = *(array + i);
  }
  delete[] array;
  for (int i = length; i < new_size; i++) {
      *(arr + i) = 0;
  }

  return arr; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
  double* arr = new double[new_size];
  for (int i = 0; i < new_size; i++) {
      *(arr + i) = *(array + i);
  }
  delete[] array;
  return arr; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  max_size = (current_size == max_size)?max_size + 5:max_size;
  current_size++;
  double* arr = new double[current_size];
  for (int i = 0; i < current_size - 1; i++) {
      *(arr + i) = *(array + i);
  }
  *(arr + current_size - 1) = element;
  delete[] array;
  return arr;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  current_size--;
  double* arr = new double[current_size];
  max_size = (max_size - current_size >= 5)?max_size - 5:max_size;
  for (int i = 0; i < current_size; i++) {
      *(arr + i) = *(array + i);
  }
  delete[] array;
  return arr; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    telemetry = append_to_array(t,telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x,telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y,telemetry, telemetry_current_size, telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}


void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  for (int i = 0; i < tot_telemetries; i++) {
      int tmp1 = global_telemetry_current_size;
      int tmp2 = *(telemetries_sizes + i);
      global_telemetry_current_size += tmp2;
      global_telemetry_max_size = (global_telemetry_current_size >= global_telemetry_max_size) ? global_telemetry_current_size + 5 : global_telemetry_max_size;
      double* new_global_tele = new double[global_telemetry_current_size];
      int index1 = 0, index2 = 0, index = 0;
      while (index1 < tmp1 && index2 < tmp2) {
          if (global_telemetry[index1] > telemetries[i][index2]) {
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
          }
          else {
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
          }
      }
      if (index1 == tmp1) {
          while (index2 < tmp2) {
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
              new_global_tele[index] = telemetries[i][index2];
              index++;
              index2++;
          }
      }
      else {
          while (index1 < tmp1) {
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
              new_global_tele[index] = global_telemetry[index1];
              index++;
              index1++;
          }
      }
      global_telemetry = new_global_tele;
  }
}
