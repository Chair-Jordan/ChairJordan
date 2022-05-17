#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define PIN_WEIGHT_SCK	20
#define PIN_WEIGHT_DT		21
#define WEIGHT_WINDOW_SIZE 10

void weight_setup(void);
void weight_recalibrate(int measurements, uint32_t timeout);
int32_t weight_read(uint32_t timeout);

#ifdef __cplusplus
}
#endif