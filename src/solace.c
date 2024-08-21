#include <stdio.h>

int main(int argc, char* argv) {
    if (argc != 2) {
        perror("Error: invalide use of solace compiler\n");
        printf("Usage: ./anvl <sourcefilename.solc>\n\n");
    }

    return 0;
}