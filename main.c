#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int menu(){
    int opt = 0;
    printf("\n");
    printf("================");
    printf("\n1- XML to XSD \n");
    printf("-1- Leave \n");
    printf("================\n");
    scanf("%d", &opt);

    getchar(); //consume the newline

    return opt;
}

char* convertXML(const char* input){
    const char* head = "<xs:schema attributeFormDefault=\"unqualified\" elementFormDefault=\"qualified\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">";
    //size_t headLen = strlen(head);
    size_t inputLen = strlen(input);
    int sizeOutput = strlen(input) * 2;
    char* output = malloc(sizeOutput);

    if(output == NULL){
        return NULL;
    }
    strcpy(output,head);

    for(size_t i = 0; i < inputLen; i++){
        if(strcmp(input[i], "<")){

        }
    }  
    return output;
}

int main() {
    int opt = 0;
    char *str = NULL;
    int size = 0;
    int length = 0;
    char ch;

    while (opt != -1) {
        opt = menu();
        switch (opt) {
        case 1:
            // Reset variables for each new XML input
            free(str); // Free any previous allocation
            str = NULL;
            length = 0;
            size = 1024; // Start with a larger buffer
            
            str = malloc(size * sizeof(char)); // Initial allocation
            if (str == NULL) {
                printf("Memory allocation failed! \n");
                exit(1);
            }
            
            printf("Enter the XML (press Enter twice to finish):\n");
            int emptyLine = 0;
            while (!emptyLine) {
                char line[1024];
                if (fgets(line, sizeof(line), stdin) == NULL) {
                    break;
                }
                
                // Check for empty line (just a newline)
                if (strcmp(line, "\n") == 0) {
                    emptyLine = 1;
                    continue;
                }
                
                int lineLen = strlen(line);
                
                // Ensure the buffer is large enough
                if (length + lineLen >= size) {
                    size *= 2;
                    char *temp = realloc(str, size * sizeof(char));
                    if (temp == NULL) {
                        printf("Memory allocation failed! \n");
                        free(str);
                        exit(1);
                    }
                    str = temp;
                }
                
                // Copy the line to the buffer
                strcpy(str + length, line);
                length += lineLen;
            }
            
            // Add null terminator
            if (length > 0 && str != NULL) {
                str[length] = '\0';
                
                char* output = convertXML(str);
                if (output != NULL) {
                    printf("Output XSD:\n%s\n", output);
                    free(output);
                } else {
                    printf("Error converting XML to XSD\n");
                }
            } else {
                printf("No XML input provided\n");
            }
            break;
        case -1:
            printf("Leaving...\n");
            free(str); // Free memory before exiting
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    }
    
    free(str); // Clean up at the end
    return 0;
}