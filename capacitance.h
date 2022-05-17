#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define PIN_CAP_CHARGE  9 // Pin which charges the capacitor
#define PIN_CAP_OPAMP  	10 // Pin which reads data from opamp

#define CAP_VCC         3.3
#define CAP_VREF        1.65
#define CAP_V0          0
#define CAP_R           826000 // Resistance of resistor in circuit
#define CAP_WINDOW_SIZE 8 // Size of sliding window

void cap_setup(void);
void cap_recalibrate(int measurements, uint32_t timeout);
int32_t cap_read(uint32_t timeout);

#ifdef __cplusplus
}
#endif
