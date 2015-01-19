#include "My_source/MyHeader.h"

static volatile uint32_t g_ui32Counter = 0;
volatile uint32_t timeCounter;
uint8_t keyStatus;
uint32_t battaryVal = 0;
uint32_t x, y;
uint8_t sendFlag;
uint8_t iss;
uint8_t cameraData[lineMax][rowsMax];
uint32_t errorFlag = 0;

int main(void)
{
    SystemInit();
    for(;;);
}



