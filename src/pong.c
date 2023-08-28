#include <stdio.h>

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

int main() {
    int l_rocket = POS_ROCKET_DEFAULT, r_rocket = POS_ROCKET_DEFAULT;
    int x_ball = X_BALL_START_1, y_ball = Y_BALL_START;

    int score_left = 0, score_right = 0;
    int start_ball_position = 0;  // Переменная-флаг, означающая начальное положение шара.
    int x_vector = 1, y_vector = 1;

    PrintTable(l_rocket, r_rocket, x_ball, y_ball, score_left, score_right);
    while (score_left < SCORE_TO_WIN && score_right < SCORE_TO_WIN) {
        if ((x_ball == WIDTH - 2 && y_ball >= r_rocket && y_ball <= r_rocket + 2) ||
            (x_ball == 1 && y_ball >= l_rocket && y_ball <= l_rocket + 2)) {
            x_vector *= -1;
        } else if ((y_ball == 1) || (y_ball == HEIGHT - 2)) {
            y_vector *= -1;
        } else if ((x_ball == WIDTH - 2 && !(y_ball >= r_rocket && y_ball <= r_rocket + 2)) ||
                   (x_ball == 1 && !(y_ball >= l_rocket && y_ball <= l_rocket + 2))) {
            start_ball_position = start_ball_position ? 0 : 1;
            x_ball == 1 ? score_right++ : score_left++;
            x_vector = start_ball_position ? -1 : 1;
            l_rocket = POS_ROCKET_DEFAULT, r_rocket = POS_ROCKET_DEFAULT;
            x_ball = start_ball_position ? X_BALL_START_2 : X_BALL_START_1;
            y_ball = Y_BALL_START;
        }

        x_ball += x_vector;
        y_ball += y_vector;

        char button = GetCommand();
        if (button == 'a' || button == 'z') {
            l_rocket = MoveLeftRocket(l_rocket, button);
        } else if (button == 'k' || button == 'm') {
            r_rocket = MoveRightRocket(r_rocket, button);
        } else if (button == 'q') {
            break;
        } else if (button == 'e') {
            printf("Invalid input\n");
            continue;
        }

        PrintTable(l_rocket, r_rocket, x_ball, y_ball, score_left, score_right);
    }
    EndGame(score_left, score_right);
}

void PrintTable(int l_rocket, int r_rocket, int x_ball, int y_ball, int score_left, int score_right) {
    ClearTheScreen();
    // Вывод поля
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (y == 0 || y == HEIGHT - 1) {
                printf("-");
            } else if (x == x_ball && y == y_ball) {
                printf("*");
            } else if ((x == 0 && y >= l_rocket && y <= l_rocket + 2) ||
                       (x == WIDTH - 1 && y >= r_rocket && y <= r_rocket + 2) ||
                       (x == WIDTH / 2 - 1 || x == WIDTH / 2)) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    // Вывод интерфейса
    for (int y = HEIGHT; y < HEIGHT + 2; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT + 1) {
                printf("-");
            } else if (y == HEIGHT && x == WIDTH / 2 - 4) {
                printf("%2d", score_left);
            } else if (y == HEIGHT && x > WIDTH / 2 - 3 && x < WIDTH / 2 - 1) {
                printf("::");
            } else if (y == HEIGHT && x == WIDTH / 2) {
                printf("%d", score_right);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
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

void ClearTheScreen() {
    printf("\033[2J");
    printf("\033[0;0f");
}

char GetCommand() {
    char c1 = getchar();
    if (c1 == '\n') {
        return 's';
    }
    char c2 = getchar();
    while (c2 != '\n') {
        c2 = getchar();
    }
    if (c1 == 'a' || c1 == 'z' || c1 == 'k' || c1 == 'm' || c1 == 'q') {
        return c1;
    } else if (c1 == ' ') {
        return 's';
    }

    return 'e';
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
