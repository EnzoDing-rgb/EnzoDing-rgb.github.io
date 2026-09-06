#include <gpiod.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile int run = 1;

static void bye(int s)
{
	(void)s;
	run = 0;
}

int main(void)
{
	struct gpiod_chip *chip;
	struct gpiod_line_settings *s;
	struct gpiod_line_config *lc;
	struct gpiod_request_config *rc;
	struct gpiod_line_request *r;
	unsigned off = 4;

	signal(SIGINT, bye);
	signal(SIGTERM, bye);

	chip = gpiod_chip_open("/dev/gpiochip5");
	if (!chip) {
		perror("open");
		return 1;
	}
	s = gpiod_line_settings_new();
	lc = gpiod_line_config_new();
	rc = gpiod_request_config_new();
	gpiod_line_settings_set_direction(s, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(s, GPIOD_LINE_VALUE_ACTIVE);
	gpiod_line_config_add_line_settings(lc, &off, 1, s);
	gpiod_request_config_set_consumer(rc, "led-stay-on");
	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);
	if (!r) {
		fprintf(stderr, "request fail\n");
		return 1;
	}
	gpiod_line_request_set_value(r, 4, GPIOD_LINE_VALUE_ACTIVE);
	printf("[ON] IO1_4 held HIGH\n");
	fflush(stdout);
	while (run)
		sleep(1);
	gpiod_line_request_set_value(r, 4, GPIOD_LINE_VALUE_INACTIVE);
	gpiod_line_request_release(r);
	gpiod_chip_close(chip);
	return 0;
}
