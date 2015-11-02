#include <pebble.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Window *s_main_window;
static int items = 1;
static TextLayer *s_items;
char str[60];


char* itoa(int val, int base)
{
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

void to_char()
{
  memset(&str[0], 0, sizeof(str)); // clear the array's old elements

  if(items <= 0)
    strcat(str, "0");
  else
  {
    char *temp = itoa(items,10);
    strcat(str, temp);
  }
}

static void main_window_load(Window *window)
{
  s_items = text_layer_create(GRect(0, 55, 155, 50));
  text_layer_set_background_color(s_items, GColorClear);
  text_layer_set_text_color(s_items, GColorBlack);  
  
  to_char();
  text_layer_set_text(s_items, str);
  
  text_layer_set_font(s_items, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_items, GTextAlignmentCenter);
  
  //link to the parent
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_items));
} 

static void main_window_unload(Window *window)
{
  text_layer_destroy(s_items);
}

static void handleUp(ClickRecognizerRef recognizer, void *context)
{
  //Window *window = (Window *)context;
  items++;
  to_char();
  text_layer_set_text(s_items, str);
}

static void handleDown(ClickRecognizerRef recognizer, void *context)
{
  if(items > 0)
    items--;
  to_char();
  text_layer_set_text(s_items, str);
}

static void config_provider(Window *window)
{
  window_single_click_subscribe(BUTTON_ID_UP, handleUp);
  window_single_click_subscribe(BUTTON_ID_DOWN, handleDown);
}


  
static void init()
{
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers)
                            {
                              .load = main_window_load,
                              .unload = main_window_unload
                            });
  window_stack_push(s_main_window, true);
  
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
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
