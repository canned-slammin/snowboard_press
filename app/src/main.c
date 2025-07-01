#include "main.h"
/*BEGIN lvgl example includes*/
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>
/*END lvgl example includes*/

/* BEGIN lvgl example setup and globals*/

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

/*
static uint32_t count;
*/

/* END lvgl example setup and globals*/

/*BEGIN subsystem testing setup and globals*/
const struct device *const dev_gpioa = DEVICE_DT_GET(DT_NODELABEL(gpioa));
static struct gpio_callback relay1_cb;
static struct gpio_callback relay2_cb;
static struct gpio_callback alert_cb;

static uint8_t relay1_on = 0;
static uint8_t relay2_on = 0;
static uint8_t alert = 0;
/*END subsystem testing setup and globals*/

/*BEGIN lvgl example callbacks*/
/*
static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	++count;
}
*/
/*END lvgl example callbacks*/

/*BEGIN subsystem testing callbacks*/

static void lv_relay1_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	relay1_on ^= 1;
}

static void lv_relay2_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	relay2_on ^= 1;
}

void alert_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	alert ^= 1;
}
/*END subsystem testing callbacks*/

int main(void) {

    /*BEGIN lvgl example variable init*/

	/*
    char count_str[11] = {0};
	char* beer_str = {"beers I'm going to drink tonight:"};
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;
	lv_obj_t *beers;
	*/

    /*END lvgl example variable init*/

	/*BEGIN subsystem testing variable init*/
	const struct device *display_dev;

	char* alert_str = {"OFF"};
	char* relay1_str = {"Toggle Relay 1"};
	char* relay2_str = {"Toggle Relay 2"};

	lv_obj_t *alert_label;
	lv_obj_t *relay1_label;
	lv_obj_t *relay2_label;

	/*END subsystem testing variable init*/

    /*BEGIN lvgl example get display*/

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

    /*END lvgl example get display*/

    /*create graphics objects*/
	/*
	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN) || IS_ENABLED(CONFIG_LV_Z_POINTER_INPUT)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, -15);
		lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED,
				    NULL);
		hello_world_label = lv_label_create(hello_world_button);
	} else {
		hello_world_label = lv_label_create(lv_scr_act());
	}
	*/

	lv_obj_t *relay1_button;
	relay1_button = lv_btn_create(lv_scr_act());
	lv_obj_align(relay1_button, LV_ALIGN_CENTER, 0, 45);
	lv_obj_add_event_cb(relay1_button, lv_relay1_callback, LV_EVENT_CLICKED, NULL);
	relay1_label = lv_label_create(relay1_button);

	lv_obj_t *relay2_button;
	relay2_button = lv_btn_create(lv_scr_act());
	lv_obj_align(relay2_button, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_event_cb(relay2_button, lv_relay2_callback, LV_EVENT_CLICKED, NULL);
	relay2_label = lv_label_create(relay2_button);

	alert_label = lv_label_create(lv_scr_act());
	lv_obj_align(alert_label, LV_ALIGN_CENTER, 0, -45);

    /*initialize graphics objects*/
	/*
	lv_label_set_text(hello_world_label, "NICE");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	beers = lv_label_create(lv_scr_act());
	lv_obj_align(beers, LV_ALIGN_BOTTOM_MID, 0, -20);
	lv_label_set_text(beers, beer_str);
	*/

	lv_label_set_text(relay1_label, relay1_str);
	lv_obj_align(relay1_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(relay2_label, relay2_str);
	lv_obj_align(relay2_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(alert_label, alert_str);

    /*start lvgl task handler and turn off display blanking*/
	lv_task_handler();
	display_blanking_off(display_dev);


    /*BEGIN lvgl example main loop*/

    while (1) {
		
		/*
		sprintf(count_str, "%d", count);
		lv_label_set_text(count_label, count_str);
		*/

		lv_task_handler();
		
		k_sleep(K_MSEC(10));
	}

    /*END lvgl example main loop*/

    return 0;
}