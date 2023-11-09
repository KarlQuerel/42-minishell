/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>

void enable_input_processing() {
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag |= ISIG; // Enable special input processing (including arrow keys)
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void disable_input_processing() {
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~ISIG; // Disable special input processing
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() {
	enable_input_processing(); // Enable special input processing

	printf("Press the up arrow key (or press 'q' to quit):\n");

	while (1) {
		char* input = readline("> ");
		if (input == NULL) {
			// Handle EOF or error
			printf("Error reading input. Exiting.\n");
			break;
		}

		if (strcmp(input, "q") == 0) {
			// Exit when 'q' is typed
			free(input);
			break;
		}

		if (strstr(input, "\033[A") != NULL) {
			// Check for the up arrow key escape sequence ("\033[A")
			printf("Up arrow key pressed!\n");
		}

		add_history(input);
		free(input);
	}

	disable_input_processing(); // Disable special input processing
	return 0;
}
 */
/* 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int main() {
    struct termios t;
    if (tcgetattr(STDIN_FILENO, &t) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    t.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing
    if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            printf("Key pressed: %c (%d)\n", c, c);
            if (c == 'q') {
                break;
            }
        }
    }

    // Restore terminal settings
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    return 0;
}
 */

/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>

int main() {
    struct termios term_settings;
    if (tcgetattr(STDIN_FILENO, &term_settings) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    struct termios modified_term_settings = term_settings;
    modified_term_settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing

    if (tcsetattr(STDIN_FILENO, TCSANOW, &modified_term_settings) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    printf("Press the up arrow key (or press 'q' to quit):\n");

    char escape_sequence[3] = {0}; // Buffer to store escape sequences
    int escape_sequence_length = 0;

    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            // Append the character to the escape sequence buffer
            escape_sequence[escape_sequence_length++] = c;

            // Check if we have received a complete escape sequence for the up arrow key
            if (escape_sequence_length == 3 &&
                escape_sequence[0] == 27 && // ASCII code for escape character
                escape_sequence[1] == '[' && escape_sequence[2] == 'A') {
                printf("HEY\n");
                escape_sequence_length = 0; // Reset the buffer
            } else if (c == 'q') {
                // Exit when 'q' is pressed
                break;
            } else if (escape_sequence_length >= 3) {
                // Reset the buffer if the escape sequence is not recognized
                escape_sequence_length = 0;
            }
        }
    }

    // Restore terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term_settings) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    return 0;
}
 */


/* #include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// Custom function to print "HEY"
void print_hey(int count, int key) {
    if (key == 0x101) {
        // Check for the up arrow key (0x101 represents the up arrow key)
        printf("HEY\n");
    }
}

int main() {
    rl_bind_key(0x101, print_hey); // Bind the up arrow key to the custom function

    printf("Press the up arrow key (or press 'q' to quit):\n");

    while (1) {
        char* input = readline("> ");
        if (input == NULL) {
            // Handle EOF or error
            printf("Error reading input. Exiting.\n");
            break;
        }

        if (strcmp(input, "q") == 0) {
            // Exit when 'q' is typed
            free(input);
            break;
        }

        add_history(input);
        free(input);
    }

    rl_unbind_key(0x101); // Unbind the custom function for the up arrow key
    return 0;
}
 */