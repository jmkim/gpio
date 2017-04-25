#include "gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPIO_MAKE_PATH(out, format, ...)    sprintf(out, format, #__VA_ARGS__)

#define GPIO_BASE_PATH(out, number)         GPIO_MAKE_PATH(out, "%s/%s%d", GPIO_PATH, GPIO_PREFIX, number)
#define GPIO_ACTIVE_PATH(out, gpio)         GPIO_MAKE_PATH(out, "%s/%s", gpio->path, "active_low")
#define GPIO_EDGE_PATH(out, gpio)           GPIO_MAKE_PATH(out, "%s/%s", gpio->path, "edge")
#define GPIO_DIRECTION_PATH(out, gpio)      GPIO_MAKE_PATH(out, "%s/%s", gpio->path, "direction")
#define GPIO_VALUE_PATH(out, gpio)          GPIO_MAKE_PATH(out, "%s/%s", gpio->path, "value")

GPIO *
gpio_create (int number)
{
  GPIO *out = (GPIO *) malloc (sizeof (GPIO));

  if (out != NULL)
    {
      out->number = number;
      GPIO_BASE_PATH (out->path, number);
      out->debounce_time = GPIO_DEBOUNCE_TIME;
    }

  return out;
}

void
gpio_free (GPIO *gpio)
{
  if (gpio != NULL)
    { free (gpio); }
}

int
gpio_read_from_file (const char *path, char *out, int max_size)
{
  FILE *fd;

  if ((fd = fopen (path, "r")) == NULL)
    { return 1; }

  if (fgets (out, max_size, fd) == NULL)
    { return 2; }

  return 0;
}

int
gpio_write_to_file (const char *path, const char *in)
{
  FILE *fd;

  if ((fd = fopen (path, "w")) == NULL)
    { return 1; }

  if (fputs (in, fd) == EOF)
    { return 2; }

  return 0;
}

int
gpio_export_to_file (GPIO *gpio, char *export_path)
{
  char number[5];
  sprintf (number, "%d", gpio->number);
  return gpio_write_to_file (export_path, number);
}

GPIO_DIRECTION
gpio_get_direction (GPIO *gpio)
{
  GPIO_DIRECTION direction;
  char path[GPIO_FILENAME_MAX], out[GPIO_DIRECTION_STRING_LENGTH_MAX];

  GPIO_DIRECTION_PATH (path, gpio);
  gpio_read_from_file (path, out, sizeof out);

  if (strcmp (out, gpio_direction_string[GPIO_DIRECTION_OUTPUT]) == 0)
    { return GPIO_DIRECTION_OUTPUT; }
  else if (strcmp (out, gpio_direction_string[GPIO_DIRECTION_INPUT]) == 0)
    { return GPIO_DIRECTION_INPUT; }
  else
    { return GPIO_DIRECTION_INPUT; }  /* TODO: Could not parse the data */
}

int
gpio_set_direction (GPIO *gpio, GPIO_DIRECTION direction)
{
  char path[GPIO_FILENAME_MAX], in[GPIO_DIRECTION_STRING_LENGTH_MAX];

  GPIO_DIRECTION_PATH (path, gpio);

  if (direction == GPIO_DIRECTION_OUTPUT)
    { sprintf (in, "%s", gpio_direction_string[GPIO_DIRECTION_OUTPUT]); }
  else
    { sprintf (in, "%s", gpio_direction_string[GPIO_DIRECTION_INPUT]); }

  return gpio_write_to_file (path, in);
}

GPIO_VALUE
gpio_get_value (GPIO *gpio)
{
  GPIO_VALUE value;
  char path[GPIO_FILENAME_MAX], out[GPIO_VALUE_STRING_LENGTH_MAX];

  GPIO_VALUE_PATH (path, gpio);
  gpio_read_from_file (path, out, sizeof out);

  if (strcmp (out, gpio_value_string[GPIO_VALUE_HIGH]) == 0)
    { return GPIO_VALUE_HIGH; }
  else if (strcmp (out, gpio_value_string[GPIO_VALUE_LOW]) == 0)
    { return GPIO_VALUE_LOW; }
  else
    { return GPIO_VALUE_LOW; }  /* TODO: Could not parse the data */
}

int
gpio_set_value (GPIO *gpio, GPIO_VALUE value)
{
  char path[GPIO_FILENAME_MAX], in[GPIO_VALUE_STRING_LENGTH_MAX];

  GPIO_VALUE_PATH (path, gpio);

  sprintf (in, "%d", value);
  return gpio_write_to_file (path, in);
}

int
gpio_toggle_output (GPIO *gpio)
{
  if (gpio_get_value (gpio) == GPIO_VALUE_HIGH)
    { return gpio_set_value (gpio, GPIO_VALUE_LOW); }
  else
    { return gpio_set_value (gpio, GPIO_VALUE_HIGH); }
}

GPIO_ACTIVE
gpio_get_active (GPIO *gpio)
{
  GPIO_ACTIVE active;
  char path[GPIO_FILENAME_MAX], out[GPIO_ACTIVE_STRING_LENGTH_MAX];

  GPIO_ACTIVE_PATH (path, gpio);
  gpio_read_from_file (path, out, sizeof out);

  if (strcmp (out, gpio_active_string[GPIO_ACTIVE_HIGH]) == 0)
    { return GPIO_ACTIVE_HIGH; }
  else if (strcmp (out, gpio_active_string[GPIO_ACTIVE_LOW]) == 0)
    { return GPIO_ACTIVE_LOW; }
  else
    { return GPIO_ACTIVE_LOW; }  /* TODO: Could not parse the data */
}

int
gpio_set_active (GPIO *gpio, GPIO_ACTIVE active)
{
  char path[GPIO_FILENAME_MAX], in[GPIO_ACTIVE_STRING_LENGTH_MAX];

  GPIO_ACTIVE_PATH (path, gpio);

  if (active == GPIO_ACTIVE_HIGH)
    { sprintf (in, "%s", gpio_active_string[GPIO_ACTIVE_HIGH]); }
  else
    { sprintf (in, "%s", gpio_active_string[GPIO_ACTIVE_LOW]); }

  return gpio_write_to_file (path, in);
}

GPIO_EDGE
gpio_get_edge_type (GPIO *gpio)
{
  GPIO_EDGE edge;
  char path[GPIO_FILENAME_MAX], out[GPIO_EDGE_STRING_LENGTH_MAX];

  GPIO_EDGE_PATH (path, gpio);
  gpio_read_from_file (path, out, sizeof out);

  if (strcmp (out, gpio_edge_string[GPIO_EDGE_NONE]) == 0)
    { return GPIO_EDGE_NONE; }
  else if (strcmp (out, gpio_edge_string[GPIO_EDGE_RISING]) == 0)
    { return GPIO_EDGE_RISING; }
  else if (strcmp (out, gpio_edge_string[GPIO_EDGE_FALLING]) == 0)
    { return GPIO_EDGE_FALLING; }
  else if (strcmp (out, gpio_edge_string[GPIO_EDGE_BOTH]) == 0)
    { return GPIO_EDGE_BOTH; }
  else
    { return GPIO_EDGE_BOTH; }  /* TODO: Could not parse the data */
}

void set_debounce_time (GPIO *gpio, int time)
{ gpio->debounce_time = time; }
