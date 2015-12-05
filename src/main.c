#include <pebble.h>
#include <time.h>
#include <stdlib.h>

Window *my_window;
TextLayer *text_layer;
static int randNum;
static char buf[128];
uint32_t num_samples = 3;


int generateRandomNumber() {
  int temp = randNum;
  srand(time(NULL));
  int n = rand() % 7;
  while(n == 0 && n != temp) {
    n = rand() % 7;
  }
  return n;
}

static void redraw() {
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  randNum = generateRandomNumber();
  snprintf(buf, sizeof(buf), "%i", randNum);
  text_layer_set_text(text_layer, buf);
  vibes_short_pulse();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
    if (direction != 0) {
      redraw();
    }
}

static void click_handler(ClickRecognizerRef recognizer, void *context) {
  redraw();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, click_handler);
}

void handle_init(void) {
  my_window = window_create();
  window_set_background_color(my_window, GColorBlueMoon);
  text_layer = text_layer_create(GRect(0, 48, 144, 48));
  text_layer_set_text_color(text_layer, GColorChromeYellow);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  snprintf(buf, sizeof(buf), "shake");
  text_layer_set_text(text_layer, buf);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_layer));
  window_stack_push(my_window, true);
  accel_tap_service_subscribe(tap_handler);
  window_set_click_config_provider(my_window, click_config_provider);
}

void handle_deinit(void) {
  accel_tap_service_unsubscribe();
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
