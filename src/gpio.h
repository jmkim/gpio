#ifndef GPIO_LIBRARY_H
#define GPIO_LIBRARY_H

#define GPIO_FILENAME_MAX FILENAME_MAX

#define GPIO_PATH "/sys/class/gpio"
#define GPIO_PREFIX "gpio"
#define GPIO_DEBOUNCE_TIME 0

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"

typedef enum GPIO_DIRECTION_ { GPIO_DIRECTION_INPUT = 0, GPIO_DIRECTION_OUTPUT = 1 } GPIO_DIRECTION;
typedef enum GPIO_VALUE_ { GPIO_VALUE_LOW = 0, GPIO_VALUE_HIGH = 1 } GPIO_VALUE;
typedef enum GPIO_ACTIVE_ { GPIO_ACTIVE_HIGH = 0, GPIO_ACTIVE_LOW = 1 } GPIO_ACTIVE;
typedef enum GPIO_EDGE_ {
  GPIO_EDGE_NONE = 0,
  GPIO_EDGE_RISING = 1,
  GPIO_EDGE_FALLING = 2,
  GPIO_EDGE_BOTH = 3
} GPIO_EDGE;

#define GPIO_DIRECTION_STRING_LENGTH_MAX    4
#define GPIO_VALUE_STRING_LENGTH_MAX        2
#define GPIO_ACTIVE_STRING_LENGTH_MAX       2
#define GPIO_EDGE_STRING_LENGTH_MAX         8

char gpio_direction_string[2][GPIO_DIRECTION_STRING_LENGTH_MAX] = {"in", "out"};
char gpio_value_string[2][GPIO_VALUE_STRING_LENGTH_MAX] = {"0", "1"};
char gpio_active_string[2][GPIO_ACTIVE_STRING_LENGTH_MAX] = {"0", "1"};
char gpio_edge_string[4][GPIO_EDGE_STRING_LENGTH_MAX] = {"none", "rising", "falling", "both"};

typedef struct GPIO_ {
  int number;
  char *path; /* Default: GPIO_PATH/GPIO_PREFIX$number */
  int debounce_time; /* Default: GPIO_DEBOUNCE_TIME */
} GPIO;

GPIO *gpio_create (int);                    /* Create and initialise GPIO struct */
void gpio_free (GPIO *);                    /* Free the allocated GPIO struct */
int gpio_export_to_file (GPIO *, char *);   /* Export GPIO; 0: OK, 1: Cannot open file, 2: Cannot write to file */
#define gpio_export(gpio)                   gpio_export_to_file(gpio, GPIO_EXPORT_PATH)
#define gpio_unexport(gpio)                 gpio_export_to_file(gpio, GPIO_UNEXPORT_PATH)
GPIO_DIRECTION gpio_get_direction (GPIO *);
int gpio_set_direction (GPIO *, GPIO_DIRECTION);
#define gpio_set_direction_in(gpio)         gpio_set_direction(gpio, GPIO_DIRECTION_IN)
#define gpio_set_direction_out(gpio)        gpio_set_direction(gpio, GPIO_DIRECTION_OUT)
GPIO_VALUE gpio_get_value (GPIO *);
int gpio_set_value (GPIO *, GPIO_VALUE);    /* GPIO_VALUE - 0: Low, 1: High */
#define gpio_set_value_low(gpio)            gpio_set_value(gpio, GPIO_VALUE_LOW)
#define gpio_set_value_high(gpio)           gpio_set_value(gpio, GPIO_VALUE_HIGH)
int gpio_toggle_output (GPIO *);
GPIO_ACTIVE gpio_get_active (GPIO *);
int gpio_set_active (GPIO *, GPIO_ACTIVE);  /* GPIO_ACTIVE - 0: Low, 1: High (default is 1) */
#define gpio_set_active_low(gpio)           gpio_set_active(gpio, GPIO_ACTIVE_LOW)
#define gpio_set_active_high(gpio)          gpio_set_active(gpio, GPIO_ACTIVE_HIGH)
GPIO_EDGE gpio_get_edge_type (GPIO *);
void set_debounce_time (GPIO *, int);       /* GPIO_DEBOUNCE_TIME default is 0 */
#endif