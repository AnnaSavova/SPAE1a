include "stdio.h"
#include "Date.h"
#include "Date.c"

int main()
{
    Date* firstDate, *secondDate, *compareDate;
    char* dateStr = "01/02/1970";
    char* secondDateStr = "02/02/1970";

    firstDate = date_create(dateStr);
    
    printf("First Date - Day: %d Month: %d Year: %d\n", firstDate->day, firstDate->month, firstDate->year);

    secondDate = date_duplicate(firstDate);

    printf("Second Date - Day: %d Month: %d Year: %d\n", secondDate->day, secondDate->month, secondDate->year);

    compareDate = date_create(secondDateStr);

    int compare = date_compare(firstDate, compareDate);
    printf("Compare %s to %s - answer: %d\n", dateStr, secondDateStr, compare);

    compare = date_compare(compareDate, firstDate);
    printf("Compare %s to %s - answer: %d\n", secondDateStr, dateStr, compare);

    compare = date_compare(firstDate, firstDate);
    printf("Compare %s to %s - answer: %d\n",dateStr, dateStr, compare);

    date_destroy(firstDate);
    date_destroy(secondDate);
    date_destroy(compareDate);

    printf("Dates destroyed");

}