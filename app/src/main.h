#pragma once

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#define ALERT_PIN  (8)
#define RELAY1_PIN (9)
#define RELAY2_PIN (10)

#define TOGGLE_ALERT  ((1) << (0))
#define TOGGLE_RELAY1 ((1) << (1))
#define TOGGLE_RELAY2 ((1) << (2))