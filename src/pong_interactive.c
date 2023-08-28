#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 27
#define WIDTH 82
#define SCORE_TO_WIN 21
#define POS_ROCKET_DEFAULT 13
#define Y_BALL_START 5
#define X_BALL_START_1 30
#define X_BALL_START_2 51

void PrintTable(int l_rocket, int r_rocket, int x_ball, int y_ball, int score_left, int score_right);
void ClearTheScreen();
char GetCommand();
int MoveLeftRocket(int y, char button);
int MoveRightRocket(int y, char button);
void EndGame(int score_left, int score_right);
void MoveBall(int *l_rocket, int *r_rocket, int *x_ball, int *y_ball, int *start_ball_position, int *x_vector,
              int *y_vector, int *score_right, int *score_left);
int KeyProcessing(int *l_rocket, int *r_rocket, int *speed, char button);

int main() {
    int l_rocket = POS_ROCKET_DEFAULT, r_rocket = POS_ROCKET_DEFAULT;
    int x_ball = X_BALL_START_1, y_ball = Y_BALL_START;

    int score_left = 0, score_right = 0;
    int start_ball_position = 0;  // Переменная-флаг, означающая начальное положение шара.
    int x_vector = 1, y_vector = 1;

    int speed = 100000;

    initscr();
    noecho();
    curs_set(0);

    nodelay(stdscr, TRUE);

    while (score_left < SCORE_TO_WIN && score_right < SCORE_TO_WIN) {
        PrintTable(l_rocket, r_rocket, x_ball, y_ball, score_left, score_right);
        refresh();
        char button = getch();

        MoveBall(&l_rocket, &r_rocket, &x_ball, &y_ball, &start_ball_position, &x_vector, &y_vector,
                 &score_right, &score_left);

        if (KeyProcessing(&l_rocket, &r_rocket, &speed, button)) {
            break;
        }

        flushinp();
        usleep(speed);
    }
    echo();
    endwin();
    ClearTheScreen();
    EndGame(score_left, score_right);
}

void PrintTable(int l_rocket, int r_rocket, int x_ball, int y_ball, int score_left, int score_right) {
    clear();
    // Drawing of the table
    for (int i = 0; i < WIDTH; i++) {
        mvprintw(0, i, "-");
        mvprintw(HEIGHT - 1, i, "-");
    }
    for (int i = 1; i < HEIGHT - 1; i++) {
        mvprintw(i, WIDTH / 2 - 1, "|");
        mvprintw(i, WIDTH / 2, "|");
    }
    mvprintw(y_ball, x_ball, "*");
    for (int i = 0; i < 3; i++) {
        mvprintw(l_rocket + i, 0, "|");
        mvprintw(r_rocket + i, WIDTH - 1, "|");
    }
    // Drawing of the interface
    for (int i = 0; i < WIDTH; i++) {
        mvprintw(HEIGHT + 1, i, "-");
    }
    mvprintw(HEIGHT, WIDTH / 2 - 1, "::");
    mvprintw(HEIGHT, WIDTH / 2 - 3, "%2d", score_left);
    mvprintw(HEIGHT, WIDTH / 2 + 1, "%d", score_right);
}

void EndGame(int score_left, int score_right) {
    if (score_left > score_right) {
        printf(
            "\t  ______ _                         __    _    _ _____ _   _ \n"
            "\t  | ___ \\ |                       /  |  | |  | |_   _| \\ | |\n"
            "\t  | |_/ / | __ _ _   _  ___ _ __  `| |  | |  | | | | |  \\| |\n"
            "\t  |  __/| |/ _` | | | |/ _ \\ '__|  | |  | |/\\| | | | | . ` |\n"
            "\t  | |   | | (_| | |_| |  __/ |    _| |_ \\  /\\  /_| |_| |\\  |\n"
            "\t  \\_|   |_|\\__,_|\\__, |\\___|_|    \\___/  \\/  \\/ \\___/\\_| \\_/\n"
            "\t                  __/ |                                     \n"
            "\t                 |___/   \n");
    } else if (score_right > score_left) {
        printf(
            "\t  ______ _                         _____   _    _ _____ _   _ \n"
            "\t  | ___ \\ |                       / __  \\ | |  | |_   _| \\ | |\n"
            "\t  | |_/ / | __ _ _   _  ___ _ __  `' / /' | |  | | | | |  \\| |\n"
            "\t  |  __/| |/ _` | | | |/ _ \\ '__|   / /   | |/\\| | | | | . ` |\n"
            "\t  | |   | | (_| | |_| |  __/ |    ./ /___ \\  /\\  /_| |_| |\\  |\n"
            "\t  \\_|   |_|\\__,_|\\__, |\\___|_|    \\_____/  \\/  \\/ \\___/\\_| \\_/\n"
            "\t                  __/ |                                       \n"
            "\t                 |___/    \n");
    } else {
        printf(
            "\t\t\t\t   _____ _    \n"
            "\t\t\t\t  |_   _(_)     \n"
            "\t\t\t\t    | |  _  ___ \n"
            "\t\t\t\t    | | | |/ _ \\\n"
            "\t\t\t\t    | | | |  __/\n"
            "\t\t\t\t    \\_/ |_|\\___|\n");
    }
}

int MoveLeftRocket(int y, char button) {
    if (button == 'a' && y > 1) {
        return y - 1;
    } else if (button == 'z' && y < HEIGHT - 4) {
        return y + 1;
    } else {
        return y;
    }
}

int MoveRightRocket(int y, char button) {
    if (button == 'k' && y > 1) {
        return y - 1;
    } else if (button == 'm' && y < HEIGHT - 4) {
        return y + 1;
    } else {
        return y;
    }
}

void MoveBall(int *l_rocket, int *r_rocket, int *x_ball, int *y_ball, int *start_ball_position, int *x_vector,
              int *y_vector, int *score_right, int *score_left) {
    if ((*x_ball == WIDTH - 2 && *y_ball >= *r_rocket && *y_ball <= *r_rocket + 2) ||
        (*x_ball == 1 && *y_ball >= *l_rocket && *y_ball <= *l_rocket + 2)) {
        *x_vector *= -1;
    } else if ((*y_ball == 1) || (*y_ball == HEIGHT - 2)) {
        *y_vector *= -1;
    } else if ((*x_ball == WIDTH - 2 && !(*y_ball >= *r_rocket && *y_ball <= *r_rocket + 2)) ||
               (*x_ball == 1 && !(*y_ball >= *l_rocket && *y_ball <= *l_rocket + 2))) {
        *start_ball_position = *start_ball_position ? 0 : 1;
        *x_ball == 1 ? (*score_right)++ : (*score_left)++;
        *x_vector = *start_ball_position ? -1 : 1;
        *l_rocket = POS_ROCKET_DEFAULT, *r_rocket = POS_ROCKET_DEFAULT;
        *x_ball = *start_ball_position ? X_BALL_START_2 : X_BALL_START_1;
        *y_ball = Y_BALL_START;
    }

    *x_ball += *x_vector;
    *y_ball += *y_vector;
}

void ClearTheScreen() {
    printf("\033[2J");
    printf("\033[0;0f");
}

int KeyProcessing(int *l_rocket, int *r_rocket, int *speed, char button) {
    if (button == 'a' || button == 'z') {
        *l_rocket = MoveLeftRocket(*l_rocket, button);
    } else if (button == 'k' || button == 'm') {
        *r_rocket = MoveRightRocket(*r_rocket, button);
    } else if (button == 'q') {
        return 1;
    } else if (button == '=') {
        if (*speed - 25000 >= 25000) {
            *speed -= 25000;
        }
    } else if (button == '-') {
        if (*speed < 150000) {
            *speed += 25000;
        }
    }
    return 0;
}
