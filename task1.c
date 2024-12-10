#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const int LINE_LENGTH =  10;

void probel(char *line) {
    int len = strlen(line);
    if (len < LINE_LENGTH) {
        for (int i = len; i < LINE_LENGTH; i++) {
            line[i] = ' '; 
        }
        line[LINE_LENGTH] = '\0'; 
    }
}

void reverse_line(char *line) {
    for (int i = 0; i < LINE_LENGTH / 2; i++) {
        char temp = line[i];
        line[i] = line[LINE_LENGTH - i - 1];
        line[LINE_LENGTH - i - 1] = temp;
    }
}

int main() {
    char input_line[LINE_LENGTH + 2]; 
    char buffer[LINE_LENGTH + 1];   
    int file1, file2;
    ssize_t bytes_read;

    
    file1 = creat("file1.txt", 0644);
    if (file1 < 0) {
        perror("Error creating file1.txt");
        return 1;
    }

    printf("Enter lines to write to file1.txt (type 'prc' to stop):\n");
    while (1) {
        printf("> ");
        fgets(input_line, sizeof(input_line), stdin);
        if (strncmp(input_line, "prc", 3) == 0) break;

        input_line[strcspn(input_line, "\n")] = '\0'; 
        probel(input_line);                        
        write(file1, input_line, LINE_LENGTH);    
    }
    close(file1);

   
    file1 = open("file1.txt", O_RDONLY);
    if (file1 < 0) {
        perror("Error opening file1.txt");
        return 1;
    }

    file2 = creat("file2.txt", 0644);
    if (file2 < 0) {
        perror("Error creating file2.txt");
        close(file1);
        return 1;
    }

    while ((bytes_read = read(file1, buffer, LINE_LENGTH)) > 0) {
        if (bytes_read < LINE_LENGTH) {
            fprintf(stderr, "Unexpected short read. Padding line.\n");
            memset(buffer + bytes_read, ' ', LINE_LENGTH - bytes_read);
        }
        buffer[LINE_LENGTH] = '\0';

        reverse_line(buffer);         
        write(file2, buffer, LINE_LENGTH);
    }

    if (bytes_read < 0) {
        perror("Error reading file1.txt");
    }

   
    close(file1);
    close(file2);

    printf("Reversed lines written to file2.txt.\n");
    return 0;
}

