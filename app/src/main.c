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


#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);


/*BEGIN subsystem testing setup and globals*/
const struct device *const dev_gpioa = DEVICE_DT_GET(DT_NODELABEL(gpioa));
static struct gpio_callback relay1_cb;
static struct gpio_callback relay2_cb;
static struct gpio_callback alert_cb;

static uint8_t relay1_on = 0;
static uint8_t relay2_on = 0;
static uint8_t alert = 0;

/*TODO add message queue*/

/*END subsystem testing setup and globals*/


static void error_handler()
{

	/*lol*/
	while(1) {};

}

static void lv_relay1_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	/*TODO add TOGGLE_RELAY1 to queu*/
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



	/*BEGIN subsystem testing variable init*/
	const struct device *display_dev;

	char* alert_str = {"OFF"};
	char* relay1_str = {"Toggle Relay 1"};
	char* relay2_str = {"Toggle Relay 2"};

	lv_obj_t *alert_label;
	lv_obj_t *relay1_label;
	lv_obj_t *relay2_label;

	/*END subsystem testing variable init*/

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}


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

	/*set up gpio pins*/
	if (!device_is_ready(dev_gpioa)) {
		error_handler();
	}

	/*TODO configure relay1 pin as output and set low*/
	/*TODO configure relay2 pin as output and set low*/
	/*TODO configure alert as input, active high, pull down (i think, might wanna check datasheet of ADS)*/


	lv_label_set_text(relay1_label, relay1_str);
	lv_obj_align(relay1_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(relay2_label, relay2_str);
	lv_obj_align(relay2_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(alert_label, alert_str);

    /*start lvgl task handler and turn off display blanking*/
	lv_task_handler();
	display_blanking_off(display_dev);


    while (1) {
		
		/*TODO get message from queue*/

		/*TODO handle TOGGLE_RELAY1 msg*/
		/*TODO handle TOGGLE RELAY2 msg*/
		/*TODO handle TOGGLE_ALERT msg*/

		/*
		sprintf(count_str, "%d", count);
		lv_label_set_text(count_label, count_str);
		*/

		lv_task_handler();
		
		k_sleep(K_MSEC(10));
	}

    return 0;
}