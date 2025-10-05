#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "main.h"   // hogy legyen GPIO és I2C definiálva

#define ABLAKOMOSO_SZINT 2
#define AKSI             3
#define ELSO_KOD         4
#define HATSO_KOD        5
#define LEGZSAK_HIBA     6
#define MENETFENY        7
#define LD_BG_1          8
#define HATSO_ABLAK_FUTES 9
#define DOBFEK          10
#define UZEMANYAGSZINT  11
#define INDEX_BALLRA    12
#define LD_BG_2         13
#define LD_BG_3         14
#define LD_BG_4         15
#define INDEX_JOBBRA    24
#define HUTOVIZ         25
#define POTTY           26
#define LEGZSAK_KI      27
#define LD_BG_5         28
#define BIZTIOV         29
#define KEZIFEK         30
#define ABS             31
#define NYITOTT_AJTO    32
#define IZZITO          33
#define OLAJNYOMAS      34
#define CHECK_ENGINE    35

// LED struktúra
typedef struct {
	const char *name;
	uint8_t channel;
	uint8_t duty;
	uint8_t reg_addr;
	uint8_t duty_offset;
} Led_t;

// publikus API
int led_driver_init(void);
int set_led(uint8_t channel, uint8_t duty_percent);
int set_led_by_name(const char *name, uint8_t duty);
int set_background_brightness(uint8_t duty_percent);
int set_vaku_led(uint8_t percent);
void update_leds_from_faults(uint32_t hibatarolo);

// a LED lista elérhető, ha kell külsőleg is
extern Led_t leds[];
extern const int led_count;
extern uint8_t global_offset;

#endif // LED_DRIVER_H
