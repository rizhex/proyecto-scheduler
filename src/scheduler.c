#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "simulation.h"

// La función que define un scheduler está compuesta por los siguientes
// parámetros:
//
//  - procs_info: Array que contiene la información de cada proceso activo
//  - procs_count: Cantidad de procesos activos
//  - curr_time: Tiempo actual de la simulación
//  - curr_pid: PID del proceso que se está ejecutando en el CPU
//
// Esta función se ejecuta en cada timer-interrupt donde existan procesos
// activos (se asegura que `procs_count > 0`) y determina el PID del proceso a
// ejecutar. El valor de retorno es un entero que indica el PID de dicho
// proceso. Pueden ocurrir tres casos:
//
//  - La función devuelve -1: No se ejecuta ningún proceso.
//  - La función devuelve un PID igual al curr_pid: Se mantiene en ejecución el
//  proceso actual.
//  - La función devuelve un PID diferente al curr_pid: Simula un cambio de
//  contexto y se ejecuta el proceso indicado.
//
int fifo_scheduler(proc_info_t *procs_info, int procs_count, int curr_time,
                   int curr_pid) {
  // Se devuelve el PID del primer proceso de todos los disponibles (los
  // procesos están ordenados por orden de llegada).
  return procs_info[0].pid;
}


int sjf_scheduler(proc_info_t *procs_info, int procs_count, int curr_time, int curr_pid) {
  int i;
  int t0 = INT_MAX; // Inicializar con un valor máximo
  int pid = -1; // Inicializar con un valor inválido
  int t1;
  
  for (i = 0; i < procs_count; i++) {
    t1 = process_total_time(procs_info[i].pid);
    if (t1 < t0) {
      t0 = t1;
      pid = procs_info[i].pid;
    }
  }
  
  return pid;
}

int stcf_scheduler(proc_info_t *procs_info, int procs_count, int curr_time, int curr_pid) {

  int i;
  int t0 = INT_MAX;
  int pid = -1;
  int t1;
  for (i = 0; i < procs_count; i++) {
      t1 = process_total_time(procs_info[i].pid) - procs_info[i].executed_time;
      if(t1 < t0){
        t0 = t1;
        pid = procs_info[i].pid;
      }
  }
    
    return pid;
}


int slice = 0;
int procs_index = 0;
int rr_scheduler(proc_info_t *procs_info, int procs_count, int curr_time, int curr_pid) {

  if(curr_pid != -1){
    if(slice < 3){
      slice++;
      return curr_pid;
    }else{
      slice = 0;
      if(procs_index < procs_count - 1){
        procs_index++;
        return procs_info[procs_index].pid;
      }else{
        procs_index = 0;
        return procs_info[procs_index].pid;
      }
    }
  }else{
    slice = 0;
    if(procs_index < procs_count - 1){
      return procs_info[procs_index].pid;
    }else{
      procs_index = 0;
      return procs_info[procs_index].pid;
    }
  }
}


int my_own_scheduler(proc_info_t *procs_info, int procs_count, int curr_time,
                     int curr_pid) {
  // Implementa tu scheduler aqui ... (el nombre de la función lo puedes
  // cambiar)

  // Información que puedes obtener de un proceso
  int pid = procs_info[0].pid;      // PID del proceso
  int on_io = procs_info[0].on_io;  // Indica si el proceso se encuentra
                                    // realizando una opreación IO
  int exec_time = procs_info[0].executed_time;  // Tiempo que el proceso se ha
                                                // ejecutado (en CPU o en I/O)

  // También puedes usar funciones definidas en `simulation.h` para extraer
  // información extra:
  int duration = process_total_time(pid);

  return -1;
}

// Esta función devuelve la función que se ejecutará en cada timer-interrupt
// según el nombre del scheduler.
schedule_action_t get_scheduler(const char *name) {
  // Si necesitas inicializar alguna estructura antes de comenzar la simulación
  // puedes hacerlo aquí.

  if (strcmp(name, "fifo") == 0) return *fifo_scheduler;
  if (strcmp(name, "sjf") == 0) return *sjf_scheduler;
  if (strcmp(name, "stcf") == 0) return *stcf_scheduler;
  if (strcmp(name, "rr") == 0) return *rr_scheduler;

  // Añade aquí los schedulers que implementes. Por ejemplo:
  //
  // if (strcmp(name, "sjf") == 0) return *sjf_scheduler;
  //

  fprintf(stderr, "Invalid scheduler name: '%s'\n", name);
  exit(1);
}
