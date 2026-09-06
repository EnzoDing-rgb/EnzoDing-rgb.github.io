#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define CHIP "/dev/gpiochip5"
#define LINE 4

int main(void)
{
	struct gpiod_chip *chip = gpiod_chip_open(CHIP);
	struct gpiod_line_settings *s;
	struct gpiod_line_config *lc;
	struct gpiod_request_config *rc;
	struct gpiod_line_request *r;
	unsigned off = LINE;

	if (!chip) {
		perror("open");
		return 1;
	}
	s = gpiod_line_settings_new();
	lc = gpiod_line_config_new();
	rc = gpiod_request_config_new();
	gpiod_line_settings_set_direction(s, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(s, GPIOD_LINE_VALUE_INACTIVE);
	gpiod_line_config_add_line_settings(lc, &off, 1, s);
	gpiod_request_config_set_consumer(rc, "led-hold");
	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);
	if (!r) {
		fprintf(stderr, "request fail\n");
		return 1;
	}

	printf("[TEST] IO1_4 HIGH 10s — watch LED\n");
	fflush(stdout);
	gpiod_line_request_set_value(r, LINE, GPIOD_LINE_VALUE_ACTIVE);
	sleep(10);

	printf("[TEST] IO1_4 LOW 3s\n");
	fflush(stdout);
	gpiod_line_request_set_value(r, LINE, GPIOD_LINE_VALUE_INACTIVE);
	sleep(3);

	printf("[TEST] blink 8 times\n");
	fflush(stdout);
	for (int i = 0; i < 8; i++) {
		gpiod_line_request_set_value(r, LINE, GPIOD_LINE_VALUE_ACTIVE);
		usleep(350000);
		gpiod_line_request_set_value(r, LINE, GPIOD_LINE_VALUE_INACTIVE);
		usleep(350000);
	}

	printf("[TEST] done\n");
	fflush(stdout);
	gpiod_line_request_release(r);
	gpiod_chip_close(chip);
	return 0;
}
