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

int main (){
    int opt = 0;
    char *str = NULL;
    int size = 0;
    int length = 0;
    char ch;

    while(opt != -1){
        opt = menu();
        switch (opt)
        {
        case 1:
            // Reset variables for each new XML input
            free(str); // Free any previous allocation
            str = NULL;
            length = 0;
            size = 10;
            
            str = malloc(size * sizeof(char)); // Initial allocation
            if(str == NULL){
                printf("Memory allocation failed! \n");
                exit(1);
            }
            
            printf("Enter the XML: ");
            while((ch = getchar()) != '\n' && ch != EOF){
                if(length + 1 >= size){
                    size *= 2;
                    char *temp = realloc(str, size * sizeof(char));
                    if(temp == NULL){
                        printf("Memory allocation failed! \n");
                        free(str);
                        exit(1);
                    }
                    str = temp;
                }
                str[length++] = ch; //store the character
            }
            str[length] = '\0';

            printf("You entered: %s \n", str);
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