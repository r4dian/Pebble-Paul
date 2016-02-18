#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[32];
  switch(tick_time->tm_mday) // would be (tick_time->tm_mday % 100) if any month had 111 days or more
  {
    case 11:
    case 12:
    case 13:
      strftime(s_buffer, sizeof(s_buffer), "%B the %dth be with you.", tick_time);
      break;
  }

  switch(tick_time->tm_mday % 10)
  {
    case 1:
      strftime(s_buffer, sizeof(s_buffer), "%B the %dst be with you.", tick_time);
      break;
    case 2:
      strftime(s_buffer, sizeof(s_buffer), "%B the %dnd be with you.", tick_time);
      break;
    case 3:
      strftime(s_buffer, sizeof(s_buffer), "%B the %drd be with you.", tick_time);
      break;
    default:
      strftime(s_buffer, sizeof(s_buffer), "%B the %dth be with you.", tick_time);
  }
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  update_date();
}

static void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "September the 04th be with you.");
  // system font list: https://github.com/pebble-examples/app-font-browser/blob/master/src/app_font_browser.c
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21)); 
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) 
{
  text_layer_destroy(s_time_layer);
}

static void init() 
{
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_background_color(s_main_window, GColorBlack);
  window_stack_push(s_main_window, true);
  update_date();
}

static void deinit() 
{
  window_destroy(s_main_window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}
