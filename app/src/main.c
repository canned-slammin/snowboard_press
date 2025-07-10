#include "main.h"

/*BEGIN subsystem testing setup and globals*/
const struct device *const dev_gpioa = DEVICE_DT_GET(DT_NODELABEL(gpioa));
static struct gpio_callback alert_cb;

struct k_msgq eventq;
K_MSGQ_DEFINE(eventq, sizeof(uint32_t), 32, 1);

char alert_str[11] = {0};

uint32_t alert_count = 0;

lv_obj_t *alert_label;
lv_obj_t *relay1_label;
lv_obj_t *relay2_label;

lv_obj_t *relay1_button;
lv_obj_t *relay2_button;

/*END subsystem testing setup and globals*/

/*Subsystem testing functions*/

void alert_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	/*add ALERT to queue*/
	uint32_t msg = ALERT;
	k_msgq_put(&eventq, &msg, K_NO_WAIT);
}

void handle_alert(void) {

	printk("ALERT message received");
	sprintf(alert_str, "%d", ++alert_count);
	printk("Printing count...");
	lv_label_set_text(alert_label, alert_str);

}

void handle_toggle_relay1(void) {

	if (gpio_pin_toggle(dev_gpioa, RELAY1_PIN != 0)) {
		printk("Error toggling relay 1\r\n");
	}

}

void handle_toggle_relay2(void) {

	if (gpio_pin_toggle(dev_gpioa, RELAY2_PIN != 0)) {
		printk("Error toggling relay 2\r\n");
	}	
}

static void lv_relay1_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	/*add TOGGLE_RELAY1 to queue*/
	uint32_t msg = TOGGLE_RELAY1;
	k_msgq_put(&eventq, &msg, K_NO_WAIT);

}

static void lv_relay2_callback(lv_event_t *e)
{
	ARG_UNUSED(e);
	/*add TOGGLE_RELAY2 to queue*/
	uint32_t msg = TOGGLE_RELAY2;
	k_msgq_put(&eventq, &msg, K_NO_WAIT);
}

void setup_gpio(void) {

	int32_t ret = 0;

	/*set up gpio pins*/
	if (!device_is_ready(dev_gpioa)) {
		printk("GPIOA not ready");
	}

	/* configure relay1 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY1_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		printk("Relay 1 pin failed to configure");
	}

	/*configure relay2 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY2_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		printk("Relay 2 pin failed to configure");
	}

	/*configure alert as input, active high (NOTE: Needs hardware pull up)*/
	ret = gpio_pin_configure(dev_gpioa, ALERT_PIN, GPIO_INPUT);
	if (ret != 0) {
		printk("ERROR: failed to configure alert pin as input");
	}

	ret = gpio_pin_interrupt_configure(dev_gpioa, ALERT_PIN, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Alert pin interrupt failed to configure");
	}

	/*register alert callback*/
	gpio_init_callback(&alert_cb, alert_callback, BIT(ALERT_PIN));
	gpio_add_callback(dev_gpioa, &alert_cb);

}

void setup_gui(void) {
	
	char* relay1_str = {"Toggle Relay 1"};
	char* relay2_str = {"Toggle Relay 2"};
	relay1_button = lv_btn_create(lv_scr_act());
	lv_obj_align(relay1_button, LV_ALIGN_CENTER, 0, 45);
	lv_obj_add_event_cb(relay1_button, lv_relay1_callback, LV_EVENT_CLICKED, NULL);
	relay1_label = lv_label_create(relay1_button);

	relay2_button = lv_btn_create(lv_scr_act());
	lv_obj_align(relay2_button, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_event_cb(relay2_button, lv_relay2_callback, LV_EVENT_CLICKED, NULL);
	relay2_label = lv_label_create(relay2_button);

	alert_label = lv_label_create(lv_scr_act());
	lv_obj_align(alert_label, LV_ALIGN_CENTER, 0, -45);

	lv_label_set_text(relay1_label, relay1_str);
	lv_obj_align(relay1_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(relay2_label, relay2_str);
	lv_obj_align(relay2_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(alert_label, alert_str);
}

int main(void) {

	/*BEGIN subsystem testing variable init*/
	const struct device *display_dev;

	int32_t ret = 0;

	/*END subsystem testing variable init*/

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

	setup_gui();
	//setup_gpio();

	/*set up gpio pins*/
	if (!device_is_ready(dev_gpioa)) {
		printk("GPIOA not ready");
	}

	/* configure relay1 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY1_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		printk("Relay 1 pin failed to configure");
	}

	/*configure relay2 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY2_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		printk("Relay 2 pin failed to configure");
	}

	/*configure alert as input, active high (NOTE: Needs hardware pull up)*/
	ret = gpio_pin_configure(dev_gpioa, ALERT_PIN, GPIO_INPUT);
	if (ret != 0) {
		printk("ERROR: failed to configure alert pin as input");
	}

	ret = gpio_pin_interrupt_configure(dev_gpioa, ALERT_PIN, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Alert pin interrupt failed to configure");
	}

	/*register alert callback*/
	gpio_init_callback(&alert_cb, alert_callback, BIT(ALERT_PIN));
	gpio_add_callback(dev_gpioa, &alert_cb);


	/*TODO not sure if required - initial ready to clear alert flag*/

    /*start lvgl task handler and turn off display blanking*/
	lv_task_handler();
	display_blanking_off(display_dev);


    while (1) {
		
		uint32_t msg = 0;

		/*get message from queue*/
		ret = k_msgq_get(&eventq, &msg, K_NO_WAIT);
		if (ret == 0) {
			printk("msg: %x\r\n", msg);
		}

		if (msg & ALERT) {
			handle_alert();
		}

		if (msg & TOGGLE_RELAY1) {
			handle_toggle_relay1();
		} 
		
		if (msg & TOGGLE_RELAY2) {
			handle_toggle_relay2();
		} 

		lv_task_handler();
		
		k_sleep(K_MSEC(10));
	}

    return 0;
}