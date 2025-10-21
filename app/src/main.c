

#include "main.h"

// echo bot
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <stdio.h>
#include <string.h>

#define MSG_SIZE 32

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(DT_NODELABEL(usart3));

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
			/* terminate string */
			rx_buf[rx_buf_pos] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}

/*
 * Print a null-terminated string character by character to the UART interface
 */
void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

/*BEGIN subsystem testing setup and globals*/

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *const dev_gpioa = DEVICE_DT_GET(DT_NODELABEL(gpioa));
const struct device *const dev_uart3 = DEVICE_DT_GET(DT_NODELABEL(usart3));
static struct gpio_callback alert_cb;

struct k_msgq eventq;
K_MSGQ_DEFINE(eventq, sizeof(uint32_t), 32, 1);


/*END subsystem testing setup and globals*/

/*BEGIN Subsystem testing functions*/
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

void alert_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	/*add ALERT to queue*/
	uint32_t msg = ALERT;
	k_msgq_put(&eventq, &msg, K_NO_WAIT);
}


/*END subsystem testing callbacks*/

int main(void) {

	/*BEGIN subsystem testing variable init*/
	
	const struct device *display_dev;

	char alert_str[11] = {0};
	char* relay1_str = {"Toggle Relay 1"};
	char* relay2_str = {"Toggle Relay 2"};

	lv_obj_t *alert_label;
	lv_obj_t *relay1_label;
	lv_obj_t *relay2_label;

	int32_t ret = 0;

	uint32_t alert_count = 0;
	
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
		LOG_ERR("GPIOA not ready");
	}

	/* configure relay1 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY1_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		LOG_ERR("Relay 1 pin failed to configure");
	}

	/*configure relay2 pin as output and set low*/
	ret = gpio_pin_configure(dev_gpioa, RELAY2_PIN, GPIO_OUTPUT_LOW);
	if (ret != 0) {
		LOG_ERR("Relay 2 pin failed to configure");
	}

	/*configure alert as input, active high (NOTE: Needs hardware pull up)*/
	ret = gpio_pin_configure(dev_gpioa, ALERT_PIN, GPIO_INPUT);
	if (ret != 0) {
		LOG_ERR("ERROR: failed to configure alert pin as input");
	}

	ret = gpio_pin_interrupt_configure(dev_gpioa, ALERT_PIN, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		LOG_ERR("Alert pin interrupt failed to configure");
	}

	/*register alert callback*/
	gpio_init_callback(&alert_cb, alert_callback, BIT(ALERT_PIN));
	gpio_add_callback(dev_gpioa, &alert_cb);

	/*TODO not sure if required - initial ready to clear alert flag*/

	lv_label_set_text(relay1_label, relay1_str);
	lv_obj_align(relay1_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(relay2_label, relay2_str);
	lv_obj_align(relay2_label, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(alert_label, alert_str);

    /*start lvgl task handler and turn off display blanking*/
	lv_task_handler();
	display_blanking_off(display_dev);



	/*START echo bot*/ 

	char tx_buf[MSG_SIZE];

	if (!device_is_ready(uart_dev)) {
		printk("UART device not found!");
		return 0;
	}

	/* configure interrupt and callback to receive data */
	ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}
	uart_irq_rx_enable(uart_dev);

	print_uart("Hello! I'm your echo bot.\r\n");
	print_uart("Tell me something and press enter:\r\n");

	/*END echo bot*/


    while (1) {
		
		uint32_t msg = 0;

		/*START echo bot*/
		/*TODO this is just going to hog the whole event loop*/
		/* indefinitely wait for input from the user */
		while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0) {
			print_uart("Echo: ");
			print_uart(tx_buf);
			print_uart("\r\n");
		}
		/*END echo bot*/

		/*get message from queue*/
		ret = k_msgq_get(&eventq, &msg, K_NO_WAIT);
		if (ret == 0) {
			LOG_INF("msg: %x\r\n", msg);
		}

		/*handle ALERT msg*/
		if (msg & ALERT) {
			LOG_INF("ALERT message received");
			sprintf(alert_str, "%d", ++alert_count);
			LOG_INF("Printing count...");
			lv_label_set_text(alert_label, alert_str);
		}
		/*handle TOGGLE RELAY1 msg*/
		if (msg & TOGGLE_RELAY1) {
			ret = gpio_pin_toggle(dev_gpioa, RELAY1_PIN);
			if (ret != 0) {
				LOG_ERR("Error toggling relay 1\r\n");
			}
		} 
		
		/*handle TOGGLE RELAY2 msg*/
		if (msg & TOGGLE_RELAY2) {
			ret = gpio_pin_toggle(dev_gpioa, RELAY2_PIN);
			if (ret != 0) {
				LOG_ERR("Error toggling relay 2\r\n");
			}
		}

		lv_task_handler();
		
		k_sleep(K_MSEC(10));
	}

    return 0;
}