#pragma once

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/modbus/modbus.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>
#include <lvgl.h>
#include <lvgl_input_device.h>
#include <stdio.h>
#include <string.h>

/*bacnet defines*/
/*#define BACNET_MSG_SIZE (16)*/


/*TODO debug*/
/*Modbus client interface*/
extern int client_iface;

#define ALERT_PIN  ((gpio_pin_t) 8u)
#define RELAY1_PIN ((gpio_pin_t) 9u)
#define RELAY2_PIN ((gpio_pin_t) 10u)

#define ALERT         ((1) << (0))
#define TOGGLE_RELAY1 ((1) << (1))
#define TOGGLE_RELAY2 ((1) << (2))
#define BACNET_RX     ((1) << (3))