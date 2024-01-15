#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_COMMAND_LENGTH 512

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void run_test(const char *test_filename) {
    if (!is_regular_file(test_filename)) {
        fprintf(stderr, "Skipping non-regular file: %s\n", test_filename);
        return;
    }

    struct stat output_stat;
    if (stat("C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\Output", &output_stat) != 0 || !S_ISDIR(output_stat.st_mode)) {
        printf("Creating Output directory...\n");
        mkdir("C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\Output");
    }

    char temp_buffer[MAX_FILENAME_LENGTH];
    strcpy(temp_buffer, test_filename);

    const char *output_folder;
    output_folder = strtok(strrchr(temp_buffer, '\\') + 1, ".");

    char output_filename[MAX_FILENAME_LENGTH];
    snprintf(output_filename, MAX_FILENAME_LENGTH, "C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\Output%s.out", output_folder);

    char temp_output_filename[MAX_FILENAME_LENGTH];
    snprintf(temp_output_filename, MAX_FILENAME_LENGTH, "C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\temp_output.txt");

    char redirect_command[MAX_COMMAND_LENGTH];
    snprintf(redirect_command, MAX_COMMAND_LENGTH, "gcc \"%s\" -o \"%s\" && \"%s\" > \"%s\"", test_filename, output_filename, output_filename, temp_output_filename);


    FILE *pipe = popen(redirect_command, "r");
    if (pipe == NULL) {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else {

        int status = pclose(pipe);

        if (status != 0) {
            fprintf(stderr, "Compilation or execution failed for %s\n", test_filename);
            exit(EXIT_FAILURE);
        }
    }

    char temp_buffer2[MAX_FILENAME_LENGTH];
    strcpy(temp_buffer2, test_filename);

    const char *tests_folder;
    tests_folder = strtok(strrchr(temp_buffer2, '\\') + 1, ".");

    char check_filename[MAX_FILENAME_LENGTH];
    snprintf(check_filename, MAX_FILENAME_LENGTH, "C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\Tests\\check_%s.txt", tests_folder);

    char compare_command[MAX_COMMAND_LENGTH];
    snprintf(compare_command, MAX_COMMAND_LENGTH, "fc \"%s\" \"%s\"", temp_output_filename, check_filename);

    FILE *output_file = fopen(temp_output_filename, "r");
    FILE *check_file = fopen(check_filename, "r");

    if (output_file == NULL || check_file == NULL) {
        perror("Error opening files for comparison");
        exit(EXIT_FAILURE);
    }

    char output_line[256];
    char check_line[256];

    int linesMatch = 1;

    while (fgets(output_line, sizeof(output_line), output_file) != NULL &&
           fgets(check_line, sizeof(check_line), check_file) != NULL) {


        if (strcmp(output_line, check_line) != 0) {
            fprintf(stderr, "fc output: %s\n", check_line);
            linesMatch = 0;
            break;
        } else {
            printf("fc output: %s\n", check_line);
        }
    }

    fclose(output_file);
    fclose(check_file);

    if (linesMatch) {
        printf("%s passed!\n", strrchr(test_filename, '\\') + 1);
    } else {
        fprintf(stderr, "%s failed!\n", strrchr(test_filename, '\\') + 1);
        exit(EXIT_FAILURE);
    }

    remove(temp_output_filename);
}

void run_all_tests(const char *input_folder) {
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(input_folder)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            printf("Processing file: %s\n", ent->d_name);

            char test_filename[MAX_FILENAME_LENGTH];
            snprintf(test_filename, MAX_FILENAME_LENGTH, "%s\\%s", input_folder, ent->d_name);

            run_test(test_filename);
        }
        closedir(dir);
    } else {
        perror("Error opening Input folder");
        exit(EXIT_FAILURE);
    }
}

int main() {
    const char *input_folder = "C:\\Users\\Acer\\Desktop\\testing_framework\\testing_framework\\Program\\Input";


    run_all_tests(input_folder);

    return EXIT_SUCCESS;
}