
#include <stdio.h>

#include "feedback.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>

#include <libp/gpio.h>
#include <libp/spi.h>


/* TODO:: get __attribute__((constructor)), __attribute__((destructor)) working and remove manual init/fini code */
extern void bss_init(void);
extern void spi_init(void);
extern void led_init(void);
extern void button_init(void);
extern void sensor_init(void);

static void
ev3ninja_init (void)
{
  bss_init();
  spi_init();
  led_init();
  button_init();
  sensor_init();
}

extern void spi_fini(void);

static void
ev3ninja_fini (void)
{
  spi_fini();
}

int 
ev3ninja_main (void)
{
  ev3ninja_init();

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  feedback_flash_green();

  // init adc power pins
  gpio_init_pin(GPIO_PIN(6, 14)); // 5VONIGEN
  gpio_init_pin(GPIO_PIN(0, 6));  // ADCBATEN

  // enable battery power on adc
  GPIO_SET(GPIO_PIN(0, 6))  =  GPIO_MASK(GPIO_PIN(0, 6));
  GPIO_DIR(GPIO_PIN(0, 6)) &= ~GPIO_MASK(GPIO_PIN(0, 6));

  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);

  //unsigned char Input[16] = { 6,8,10,12,5,7,9,11,1,0,13,14,2,15,3,4 };

  //unsigned char input2_adc1 = Input[1];
  //unsigned char input2_adc2 = Input[5];

  while (1)
    {
      if (sensor_touch_get_state(SENSOR_PORT_2) == SENSOR_TOUCH_DOWN)
        printf(" down\r");
      else
        printf(" up  \r");

      //gpio_set(GPIO_PIN(8, 12), 1);
      //unsigned short Data1 = (spi_update((0x1840 | ((input2_adc1 & 0x000F) << 7)))) & 0x0FFF;
      //unsigned short Data2 = (spi_update((0x1840 | ((input2_adc2 & 0x000F) << 7)))) & 0x0FFF;
      //printf("%i %i %i %i %i || %i %i                \r", gpio_get(GPIO_PIN(8, 12)), gpio_get(GPIO_PIN(8, 15)), gpio_get(GPIO_PIN(0, 14)), gpio_get(GPIO_PIN(0, 13)), gpio_get(GPIO_PIN(8, 14)), Data1, Data2);
    }

  printf("\n");

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  ev3ninja_fini();

  return 0;
}

/* bss init */
extern unsigned long __bss_start, _end;

void
__attribute__((constructor)) // <- does not work yet
bss_init (void)
{
  /* Zero out BSS */
  unsigned char * cp = (unsigned char *) &__bss_start;
  while (cp < (unsigned char *)&_end)
    *cp++ = 0;
}

