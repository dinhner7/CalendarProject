/*
Name: Dinh Huynh
UID: 117045871
DirectoryID: dhuynh13
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"

int init_calendar(const char *name, int days, int (*comp_func)(const void *, const void *),
                  void (*free_func)(void *), Calendar **calendar) {
    Calendar *cal;
    if(days < 1 || name == NULL || calendar == NULL) {
        return FAILURE;
    }
    /*create new calendar*/
    cal = malloc(sizeof(Calendar));
    if(cal == NULL) {
        return FAILURE;
    }
    /*allocate space for name*/
    cal->name = malloc(strlen(name) + 1);
    if(cal->name == NULL) {
        free(cal);
        return FAILURE;
    }
    strcpy(cal->name, name);

    /*allocate space for events arr*/
    cal->days = days;
    cal->events = calloc(days, sizeof(Event));
    if(cal->events == NULL) {
        free(cal->name);
        free(cal);
        return FAILURE;
    }
    /*update rest of cal data*/
    cal->total_events = 0;
    cal->comp_func = comp_func;
    cal->free_info_func = free_func;

    *calendar = cal;
    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *file, int val) {
    int i;
    Event *curr;
    if(calendar == NULL || file == NULL) {
        return FAILURE;
    }
    /*calendar info only print if val true*/
    if(val != 0) {
        fprintf(file, "Calendar's Name: \"%s\"\nDays: %d\n", calendar->name, calendar->days);
        fprintf(file, "Total Events: %d\n\n", calendar->total_events);
    }
    /*always print event info*/
    fprintf(file, "**** Events ****\n");
    if(calendar->total_events != 0) {
        for(i = 1; i <= calendar->days; i++) {
            fprintf(file, "Day %d\n", i);
            curr = calendar->events[i - 1];
            while(curr != NULL) {
                fprintf(file, "Event's Name: \"%s\", Start_time: %d,", curr->name, curr->start_time);
                fprintf(file, " Duration: %d\n", curr->duration_minutes);
                curr = curr->next;
            }
        }
    }
    

    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start, 
              int duration, void *info, int day) {
    Event *event, *curr, *prev;
    /*failure cases*/
    if(calendar == NULL || name == NULL || day < 1 || day > calendar->days || 
       start < 0 || start > 2400 || duration <= 0) {
        return FAILURE;
    }
    
    if(find_event(calendar, name, NULL) == SUCCESS) {
        return FAILURE;
    }

    /*allocate event space*/
    event = malloc(sizeof(Event));
    if(event == NULL) {
        return FAILURE;
    }
    /*allocate name space*/
    event->name = malloc(strlen(name) + 1);
    if(event->name == NULL) {
        free(event);
        return FAILURE;
    }
    strcpy(event->name, name);

    /*other initializations*/
    event->start_time = start;
    event->duration_minutes = duration;
    event->info = info;
    event->next = NULL;
    
    curr = calendar->events[day - 1];
    if(curr == NULL) {
        calendar->events[day - 1] = event;
        calendar->total_events++;
        return SUCCESS;
    }
    /*insert event where specified*/
    while(curr != NULL && calendar->comp_func(curr, event) < 0) {
        prev = curr;
        curr = curr->next;
    }
    if(curr == calendar->events[day - 1]) {
        event->next = calendar->events[day - 1];
        calendar->events[day - 1] = event;
    }else {
        prev->next = event;
        event->next = curr;
    }

    calendar->total_events++;
    return SUCCESS;
}

int find_event(Calendar *calendar, const char *event_name, Event **event) {
    int i;
    Event *curr;

    if(calendar == NULL || event_name == NULL) {
        return FAILURE;
    }
    /*go thru array*/
    for(i = 1; i <= calendar->days; i++) {
        curr = calendar->events[i - 1];
        if(curr == NULL) {
            continue;
        }
        /*cycle thru linked list*/
        while(curr != NULL) {
            if(strcmp(curr->name, event_name) == 0) {
                if(event != NULL) {
                    *event = curr;
                }
                return SUCCESS;
            }
            curr = curr->next;
        }
    }

    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {
    Event *curr;

    if(calendar == NULL || name == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    curr = calendar->events[day - 1];
    while(curr != NULL) {
        if(strcmp(curr->name, name) == 0) {
            if(event != NULL) {
                *event = curr;
            }
            return SUCCESS;
        }
        curr = curr->next;
    }

    return FAILURE;
}

int remove_event(Calendar *calendar, const char *event_name) {
    int i, found, day;
    Event *curr, *prev;

    if(calendar == NULL || event_name == NULL) {
        return FAILURE;
    }

    found = 0;
    for(i = 1; i <= calendar->days; i++) {
        prev = NULL;
        curr = calendar->events[i - 1];
        while(curr != NULL) {
            if(!strcmp(curr->name, event_name)) {
                day = i;
                found = 1;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        if(found) {
            break;
        }
    }

    if(found) {
        if(prev == NULL) {
            calendar->events[day - 1] = curr->next;
            curr->next = NULL;
        }else {
            prev->next = curr->next;
            curr->next = NULL;
        }

        free(curr->name);
        if(curr->info != NULL) {
            calendar->free_info_func(curr->info);
        }
        free(curr);
        curr = NULL;
        calendar->total_events--;
        return SUCCESS;
    }
    
    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *event_name) {
    int i;
    Event *curr;
    if(calendar == NULL || event_name == NULL) {
        return NULL;
    }
    /*go thru array*/
    for(i = 1; i <= calendar->days; i++) {
        curr = calendar->events[i - 1];
        /*cycle thru linked list*/
        while(curr != NULL) {
            if(strcmp(curr->name, event_name) == 0) {
                return curr->info;
            }
            curr = curr->next;
        }
    }

    return NULL;
}

int clear_day(Calendar *calendar, int day) {
    Event *curr = calendar->events[day - 1];
    
    if(calendar == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    while(curr != NULL) {
        calendar->events[day - 1] = curr->next;
        curr->next = NULL;
        free(curr->name);
        if(curr->info != NULL) {
            calendar->free_info_func(curr->info);
        }
        free(curr);
        calendar->total_events--;
        curr = calendar->events[day - 1];
    }

    return SUCCESS;
}

int clear_calendar(Calendar *calendar) {
    int i;
    if(calendar == NULL) {
        return FAILURE;
    }
    for(i = 1; i <= calendar->days; i++) {
        clear_day(calendar, i);
    }

    return SUCCESS;
}

int destroy_calendar(Calendar *calendar) {
    if(calendar == NULL) {
        return FAILURE;
    }
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar->name);
    free(calendar);

    return SUCCESS;
}
