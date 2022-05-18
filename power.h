#ifdef __cplusplus
extern "C" {
#endif
  
#include "avr/sleep.h"
  // Unused pins are defined in power.ani in a for loop :v)
  void RTC_init(void);
  void ISR(RTC_PIT_vect);
  void power_init(void);
  void sleep(void);
  void init_pins(void);

#ifdef __cplusplus
}
#endif
