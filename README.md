This Calendar Project implements a similar function to google calendars but looks more primitive. 

Functions below can be found in calendar.c:

## Calendar.c functions

- int init_calendar(const char *name, int days, int (*comp_func)(const void *, const void *), void (*free_func)(void *), Calendar **calendar) {}
  - Creates a calendar object with a string *\*name*, the number of days *days*, a comparator function *\*comp_func*, a memory deallocation function *\*free_func*, and a pointer to the calendar created *\*\*calendar*

- int print_calendar(Calendar *calendar, FILE *file, int val) {}
  - Takes in a calendar, txt file and number of lines *val* and prints out the calendar

- int add_event(Calendar *calendar, const char *name, int start, int duration, void *info, int day) {}
  - Adds an event into the calendar 

- int find_event(Calendar *calendar, const char *event_name, Event **event) {}

- int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {}

- int remove_event(Calendar *calendar, const char *event_name) {}

- void *get_event_info(Calendar *calendar, const char *event_name) {}

- int clear_day(Calendar *calendar, int day) {}

- int clear_calendar(Calendar *calendar) {}

- int destroy_calendar(Calendar *calendar) {}
