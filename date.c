#include <stdlib.h>
#include <stdio.h>
#include "date.h"

struct date {
        int day;
        int month;
        int year;
};

Date *date_create(char *datestr){
        int day = 0;
        int month = 0;
        int year = 0;

        /* check datestr is in the expected dd/mm/yyyy format and assign the information to the ints day,month and year respectively*/
        sscanf(datestr,"%2d/%2d/%4d",&day,&month,&year);

        if (day >= 1 && day<= 31 && month >= 1 && month <= 12){
            Date *d = malloc(sizeof(Date));
            d -> day = day;
            d -> month = month;
            d -> year = year;
            return d;
        } else {
            return NULL;
        }
}

Date *date_duplicate(Date *d){
    Date *dupe = (Date *)malloc(sizeof(Date));
    if (d == NULL) {
        return NULL;
    } else {
        *dupe = *d;
    }
	return dupe;
}

int date_compare(Date *date1, Date *date2){


    if (date1->year<date2->year || (date1->year>=date2->year && date1->month<date2->month) || (date1->year>=date2->year && date1->month>=date2->month && date1->day<date2->day)){
        return -999;
    } else if (date1->year == date2->year && date1->month == date2->month && date1->day == date2->day){
        return 0;
    } else {
        return 999;
    }
}

void date_destroy(Date *d){
    free(d);
}
