#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define PIN_CAP_CHARGE  9
#define PIN_CAP_OPAMP  	10

#define CAP_VCC         3.3
#define CAP_VREF        1.65
#define CAP_V0          0
#define CAP_R           826000
#define CAP_WINDOW_SIZE 8

void cap_setup(void);
void cap_recalibrate(int measurements, uint32_t timeout);
int32_t cap_read(uint32_t timeout);

#ifdef __cplusplus
}
#endif