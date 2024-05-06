This Calendar Project implements a similar function to google calendars but looks more primitive. 

Functions below can be found in calendar.c:

## Calendar.c functions

int init_calendar(const char *name, int days, int (*comp_func)(const void *, const void *), void (*free_func)(void *), Calendar **calendar) {}

int print_calendar(Calendar *calendar, FILE *file, int val) {}

int add_event(Calendar *calendar, const char *name, int start, int duration, void *info, int day) {}

int find_event(Calendar *calendar, const char *event_name, Event **event) {}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {}

int remove_event(Calendar *calendar, const char *event_name) {}

void *get_event_info(Calendar *calendar, const char *event_name) {}

int clear_day(Calendar *calendar, int day) {}

int clear_calendar(Calendar *calendar) {}

int destroy_calendar(Calendar *calendar) {}
