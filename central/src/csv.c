#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

char * handle_filename() {
    char *file_name;
    file_name = malloc(sizeof(char) * 35);

    time_t t = time(NULL);
    struct tm dtm = *localtime(&t);
    sprintf(file_name, "csv/%d-%02d-%02d_logs.csv", dtm.tm_year + 1900, dtm.tm_mon + 1, dtm.tm_mday);
    return file_name;
}

void write_csv_on_file(char* event,  int action_value ) {
    char file_location[35];

    sprintf(file_location, "%s", handle_filename());

    FILE *fp = fopen(file_location, "a");
    char date_time[35];

    time_t t = time(NULL);
    struct tm dtm = *localtime(&t);
    sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", dtm.tm_year + 1900, dtm.tm_mon + 1, dtm.tm_mday, dtm.tm_hour, dtm.tm_min, dtm.tm_sec);

    fprintf(fp, "\"%s\",\"%s\",\"%d\"\n", date_time, event, action_value);
    fclose(fp);
}

void handle_file_creation() {
    FILE *fp;
    
    char file_location[35];
    memcpy(file_location, handle_filename(), sizeof(char) * 35);

    fp = fopen(file_location, "r");

    if(fp == NULL)
    {
        fp = fopen(file_location, "w");
        fprintf(fp, "\"date_time\",\"sensor\",\"value\"\n");
        fclose(fp);
    }
    else {
        fclose(fp);
    }
}