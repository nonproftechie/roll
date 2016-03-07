#include <pebble.h>
#include <time.h>
#include <stdlib.h>

Window *my_window;
TextLayer *text_layer;
static GRect bounds;
static GRect start;
static GRect finish;
static int randNum;
static char buf[6];
uint32_t num_samples = 3;
static PropertyAnimation *s_box_animation;


int generateRandomNumber() {
  srand(time(NULL));
  int n = rand() % 7;
  while(n == 0 || n == randNum) {
    n = rand() % 7;
  }
  return n;
}

static void anim_stopped_handler(Animation *animation, bool finished, void *context){}

static void redraw() {
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  randNum = generateRandomNumber();
  snprintf(buf, sizeof(buf), "%i", randNum);
  text_layer_set_text(text_layer, buf);
  #if defined(PBL_COLOR)
    switch (randNum) {
      case 1:
        text_layer_set_text_color(text_layer, GColorRed);
        window_set_background_color(my_window, GColorBlack);
        break;
      case 2:
        text_layer_set_text_color(text_layer, GColorBlack);
        window_set_background_color(my_window, GColorRed);
        break;
      case 3:
        text_layer_set_text_color(text_layer, GColorBrightGreen);
        window_set_background_color(my_window, GColorBlack);
        break;
      case 4:
        text_layer_set_text_color(text_layer, GColorBlack);
        window_set_background_color(my_window, GColorBrightGreen);
        break;
      case 5:
        text_layer_set_text_color(text_layer, GColorVividCerulean);
        window_set_background_color(my_window, GColorBlack);
        break;
      case 6:
        text_layer_set_text_color(text_layer, GColorBlack);
        window_set_background_color(my_window, GColorVividCerulean);
        break;
    }
    vibes_double_pulse();
  #endif
  
  start = GRect(0, (bounds.size.h / 2), bounds.size.w, bounds.size.h);
  finish = GRect(0, (bounds.size.h / 2) - 32, bounds.size.w, bounds.size.h);
  Layer *t_layer_as_layer = text_layer_get_layer(text_layer);
  s_box_animation = property_animation_create_layer_frame(t_layer_as_layer, &start, &finish);
  animation_set_duration((Animation*)s_box_animation, 300);
  animation_set_delay((Animation*)s_box_animation, 0);
  animation_set_curve((Animation*)s_box_animation, AnimationCurveEaseIn);
  animation_set_handlers((Animation*)s_box_animation, (AnimationHandlers) {
    .stopped = anim_stopped_handler
  }, NULL);
  animation_schedule((Animation*)s_box_animation);
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
  Layer *window_layer = window_get_root_layer(my_window);
  bounds = layer_get_bounds(window_layer);
  
  window_set_background_color(my_window, GColorBlack);
  
  text_layer = text_layer_create((GRect) { .origin = { 0, (bounds.size.h / 2) - 32 }, .size = { bounds.size.w, bounds.size.h } });
  
  #if defined(PBL_COLOR)
    text_layer_set_text_color(text_layer, GColorBrightGreen);
  #else
    text_layer_set_text_color(text_layer, GColorWhite);
  #endif
  
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  snprintf(buf, sizeof(buf), "ready");
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
