#ifdef __cplusplus
extern "C" {
#endif
  
#include "avr/sleep.h"
#include <RFM69.h>

  #define PIN_RADIO_RESET 3
  // Values that wake up cpu when measured
  #define CAP_WAKEUP_VAL 100
  #define WEIGHT_WAKEUP_VAL 200000
  // Unused pins are defined in power.ani in a for loop :v)
  void pwr_init(void);
  void pwr_cpu_sleep(int);
  void pwr_RF_sleep(void);
  void pwr_RF_wake(void);
  void pwr_idle(int);
  bool pwr_should_sleep(int);

#ifdef __cplusplus
}
#endif
