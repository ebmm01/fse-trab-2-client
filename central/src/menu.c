#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "menu.h"
#include "socket.h"
#include "sensors.h"
#include "events.h"
#include "csv.h"

int parent_x, parent_y, new_x, new_y;

volatile int manual_temp = 35, highlight = 0, sensors[8] = { 0, 0, 0, 0, 0, 0 };
char terminal_messages[400];

char sensor_state_value[2][20] = { "desativacao", "ativacao" };

void notify_client_to_update_sensor(int sensor_index, int sensor_value) {
    char message[100];

    sprintf(message, "%s;%d;%d", CHANGE_SENSOR_VALUE, sensor_index, sensor_value);

    sprintf(terminal_messages, "Solicitando %s do sensor %s...", 
        sensor_state_value[sensor_value], output_sensor_names[sensor_index]);

    send_message_to_server(message, strlen(message) + 1);

    write_csv_on_file(output_sensor_names[sensor_index], sensor_value);
}

void* execute_menu() {
    
    int score_size = 3;
    int key = ERR;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    // set up initial windows
    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *field = newwin((parent_y - score_size) -4, (parent_x/2)-1, 0, 0);
    WINDOW *score = newwin((parent_y - score_size) -4, (parent_x/2)-1, 0, (parent_x/2));
    WINDOW *terminalData = newwin(3, parent_x - 2, (parent_y - score_size) -4, 0);
    
    char choices[6][30] = { 
        "Lampada Cozinha", 
        "Lampada Sala", 
        "Lampada quarto 01", 
        "Lampada quarto 02",
        "Ar condicionado 01",
        "Ar condicionado 02"
    };

    char menu_state [2][13] = { "Disconectado", "Conectado" };
    char sensor_state [3][32] = { "Ocioso", "Detectando", "Aguardando dados do client..." };

    box(field, 0, 0);
    box(score, 0, 0);
    box(terminalData, 0, 0);

    while(1) {
        // refresh each window
        wrefresh(stdscr);
        wrefresh(field);
        wrefresh(score);
        wrefresh(terminalData);

        for(int i = 0; i < 6; i++) {
            if (sensors[i])
                wattron(score, A_REVERSE);
            mvwprintw(score, i + 1, 2, "[%d] - %s", i + 1, choices[i]);
            wattroff(score, A_REVERSE);
        }

        if (key == KEY_RESIZE) {
            getmaxyx(stdscr, new_y, new_x);

            if (new_y != parent_y || new_x != parent_x) {
                parent_x = new_x;
                parent_y = new_y;

                wresize(field, new_y - score_size, new_x);
                wresize(score, score_size, new_x);
                mvwin(score, new_y - score_size, 0);
                
                wclear(stdscr);
                wclear(field);
                wclear(score);

                box(field, 0, 0);
                box(score, 0, 0);
                box(terminalData, 0, 0);
            }
        }

        

        key = getch();

        switch (key) {
            case '1':
                notify_client_to_update_sensor(0, 1 - sensors[0]);
                break;
            case '2':
                notify_client_to_update_sensor(1, 1 - sensors[1]);
                break;
            case '3':
                notify_client_to_update_sensor(2, 1 - sensors[2]);
                break;
            case '4':
                notify_client_to_update_sensor(3, 1 - sensors[3]);
                break;
            case '5':
                notify_client_to_update_sensor(4, 1 - sensors[4]);
                break;
            case '6':
                notify_client_to_update_sensor(5, 1 - sensors[5]);
                break;
            default:
                break;
        }

        mvwprintw(field, 0, 2, "Status dos sensores");
        mvwprintw(score, 0, 2, "Menu");
        mvwprintw(field, 1, 50, "                 ");
        mvwprintw(field, 1, 2, "Status do servidor distribuido: -------------------- %s", menu_state[connected]);
        mvwprintw(field, 2, 50, "                 ");
        mvwprintw(field, 2, 2, "Temperatura: --------------------------------------- %.2f ºC", temperature);
        mvwprintw(field, 3, 50, "                 ");
        mvwprintw(field, 3, 2, "Umidade: ------------------------------------------- %.2f \%", humidity);
        mvwprintw(field, 4, 50, "                                        ");
        mvwprintw(field, 4, 2, "Sensor de presenca Sala ---------------------------- %s", sensor_state[sensors_values[0]]);
        mvwprintw(field, 5, 50, "                                        ");
        mvwprintw(field, 5, 2, "Sensor de presenca Cozinha ------------------------- %s", sensor_state[sensors_values[1]]);
        mvwprintw(field, 6, 50, "                                        ");
        mvwprintw(field, 6, 2, "Sensor de abertura porta Cozinha ------------------- %s", sensor_state[sensors_values[2]]);
        mvwprintw(field, 7, 50, "                                        ");
        mvwprintw(field, 7, 2, "Sensor de abertura janela Cozinha ------------------ %s", sensor_state[sensors_values[3]]);
        mvwprintw(field, 8, 50, "                                        ");
        mvwprintw(field, 8, 2, "Sensor de abertura porta Sala ---------------------- %s", sensor_state[sensors_values[4]]);
        mvwprintw(field, 9, 50, "                                        ");
        mvwprintw(field, 9, 2, "Sensor de abertura janela Sala --------------------- %s", sensor_state[sensors_values[5]]);
        mvwprintw(field, 10, 50, "                                        ");
        mvwprintw(field, 10, 2, "Sensor de abertura janela quarto 01 ---------------- %s", sensor_state[sensors_values[6]]);
        mvwprintw(field, 11, 50, "                                        ");
        mvwprintw(field, 11, 2, "Sensor de abertura janela quarto 02 ---------------- %s", sensor_state[sensors_values[7]]);
        
        mvwprintw(terminalData, 1, 1, "                                                                              ");
        mvwprintw(terminalData, 0, 2, "%s", "Eventos recentes");
        mvwprintw(terminalData, 1, 2, "%s", terminal_messages);
        
        usleep(200000);
    }
}