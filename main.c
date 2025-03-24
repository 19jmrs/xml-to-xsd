#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

typedef struct XmlNode {
    char *name;
    char *content;
    struct XmlNode *child;
    struct XmlNode *nextNode;
} XmlNode;

char *skipWhiteSpace(char *s){
    while(*s && isspace((unsigned char)*s)){
        s++;
    }
    return s;
}

char *parseName (char **p){
    char *start = *p;
    //while it's a valid character for xml name
    while((**p && (isalnum((unsigned char) **p))) || **p == '_' || **p == ":" || **p == "." || **p == "-"){
        (*p)++;
    }
    //get the size of the name
    size_t length = *p - start;
    char *name = (char *)malloc(sizeof(length + 1));
    if(!name){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
   
    strncpy(name,start,length);
    name[length] = '\0';
    return name;
}

XmlNode *parseElement(char **input){
    *input = skipWhiteSpace(*input);
    if(**input != '<'){
        fprintf(stderr, "Expected '<'\n");
        return NULL;
    }
    (*input)++;
    if(**input == '/'){
        return NULL;
    }
    XmlNode *node = (XmlNode *)malloc(sizeof(XmlNode));
    if(!node){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    //add value to XmlNode
    node->name = parseName(input);
    node->child = NULL;
    node->content = NULL;
    node->nextNode = NULL;

    *input = skipWhiteSpace(*input);
    int selfClosing = 0;
    if(**input == '/'){
        selfClosing = 1;
        (*input)++;
    }
    if(**input != '>'){
        fprintf(stderr, "Expected '>' at end of start tag for %s\n", node->name);
    }
    (*input)++;

    if(selfClosing)
        return node;

    XmlNode *childHead = NULL, *childTail = NULL;
    while(1){
        *input = skipWhiteSpace(*input);
        if(**input == '<'){
            if((*input)[1] == '/'){
                break;
            }
            else{
                XmlNode *child = parseElement(input);       
                if(child){
                    if(!childHead){
                        childHead = childTail = child;
                    } else{
                        childTail->nextNode = child;
                        childTail = child;
                    }
                }
            }
        } else{
            char *textStart = *input;
            while(**input && **input != '<'){
                (*input)++;
            }
            size_t len = *input - textStart;
            char *text = (char *)malloc(len + 1);
            if(!text){
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            strncpy(text, textStart, len);
            text[len] = '\0';

            int onlyWhiteSpace = 1;
            for(size_t i = 0; i < len; i++){
                if(!isspace((unsigned char)text[i])){
                    onlyWhiteSpace = 0;
                    break;
                }
            }
            if(!onlyWhiteSpace){
                if(node->content == NULL){
                    node->content = text;
                } else{
                    char *newContent = (char *)malloc(strlen(node->content) + strlen(text) + 1);
                    if(!newContent){
                        perror("malloc");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(newContent, node->content);
                    strcat(newContent, text);
                    free(node->content);
                    node->content = newContent;
                    free(text);
                }
            } else {
                free(text);
            }
        }
        *input = skipWhiteSpace(*input);
        if(**input == '<' && (*input)[1] == '/'){
            break;
        }
    }
    if(**input != '<' || (*input)[1] != '/'){
        fprintf(stderr, "Expected closing tag for %s\n", node->name);
    } else {
        (*input) += 2;
        char *closingName = parseName(input);
        if(strcmp(closingName, node->name) != 0){
            fprintf(stderr, "Mismatched closing tag: expected %s, got %s\n", node->name, closingName);
        }
        free(closingName);
        *input = skipWhiteSpace(*input);
        if(**input == '>'){
            (*input)++;
        } else{
            fprintf(stderr, "Expected '>' at end of closign tag for %s\n", node->name);
        }
    }
    node->child = childHead;
    return node;
    

}

int main() {
    int opt = 0;
    char *str = NULL;
    int size = 0;
    int length = 0;
    char ch;

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
                
                // appends to the end of the previous line
                strcpy(str + length, line);
                length += lineLen;
            }
            fprintf(stdout, "<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n");
            //add here rest of corp
            XmlNode *root = parseElement(&str);
            fprintf(stdout, "\n Element: %s", root->name);
            fprintf(stdout, "\n Child: %s", root->child->name);
            fprintf(stdout, "</xs:schema>\n");
            break;
        case -1:
            printf("Leaving...\n");
            free(str); // Free memory before exiting
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
    
    
    free(str); // Clean up at the end
    return 0;
}