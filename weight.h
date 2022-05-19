#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define PIN_WEIGHT_SCK 13 // Clock pin for HX711 module
#define PIN_WEIGHT_DT 12 // Data pin for HX711 module
#define WEIGHT_WINDOW_SIZE 10 // Size of sliding window

void weight_setup(void);
void weight_recalibrate(int measurements, uint32_t timeout);
int32_t weight_read(uint32_t timeout);

#ifdef __cplusplus
}
#endif
