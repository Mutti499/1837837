#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 10000000 // maximum number of key-value pairs that can be stored hash table

typedef enum
{
    TOK_INVALID,
    TOK_VARIABLE,
    TOK_INTEGER,
    TOK_PLUS,  
    TOK_MINUS, 
    TOK_TIMES, 
    TOK_AND,
    TOK_OR,
    TOK_RPARENTHESES,
    TOK_LPARENTHESES,
    TOK_FUNCTIONSEPERATOR,
    TOK_XOR,
    TOK_LS,
    TOK_RS,
    TOK_LR,
    TOK_RR,
    TOK_NOT,
    TOK_NULL,
    TOK_EQUAL
} TokenType; //Token type options to use when creating Tokens

typedef struct
{
    TokenType type; 
    long long int value;   

} Token;// Token structure for nodes

struct Node;

typedef struct Node{
    TokenType operator;
    Token* token;
    struct Node* leftchild;
    struct Node* rightchild;
} Node; //Node structure for trees


typedef struct {
    char *key;
    long long int value;    
} Variable; // Variable structure for has table assignments

Variable hashTable[MAX_SIZE]; // Initializing hash table

static long long int evaluator(Node* root);// Evaluator function for calculating the tree output
static Node* nodeCreator(TokenType operator1, Node* term1, Node* next_term1,Token* token); //Node creater function for creating nodes for the parse tree
static void nextToken(); // Iterator function to switch to the next token in the token array
static Node* parse_Exp(); // First layer of parse function to create "OR TOKEN NODE" for tree structure
static Node* parse_and(); // Second layer of parse function to create "AND TOKEN NODE" for tree structure
static Node* parse_plus(); // Third layer of parse function to create "PLUS TOKEN NODE" or "MINUS TOKEN NODE" for tree structure
static Node* parse_term(); // Fourth layer of parse function to create "TIMES TOKEN NODE" for tree structure
static Node* parse_separator(); // Fifth layer of parse function to create "FUNCTION TOKEN NODE" for tree structure
static Node* parse_functions();// Sixth layer of parse function to create "FUNCTION TOKEN NODE" for tree structure
static Node* parse_not();// Seventh layer of parse function to create "NOT TOKEN NODE" for tree structure
static Node* parse_factor();// Last layer of parse function to create leaf nodes which will be tokens for tree structure
static unsigned long long int hasher(char *key); // Hash calculator which will find unique keys for strings 
static void insertTable(char *key, long long int value);  // Functions for putting key - value pairs to hashMap
static long long int searchTable(char *key); // Functions for searching value in hashMap for unique keys
static long long int editTable(char *key, long long int value); // Functions for editing values in hashMap for given key value 

static bool errorHandler(); // Error handler function for finding errors before creating tree structure
static bool lastTokenChecker(); // First function in errorhandler() to find there whether the last Token is valid or not 
static bool sequentTokenChecker(); // Second function in errorhandler() to find is there invalid token sequence 


Token tokens[256]  = {0}; // Token array which will be store tokens according to given input
char input[258]  = {0}; // Char array to store given input line. It was 256 but changed it 258 to prevent 256char input with last letter x ( x o r checking part )  
int myIndex = 0; // Index finder for searching in token array
Token* current_token ;
bool invalidTokenCheck = false; // If invalid token is created code will give an error
bool paranthesesCheck = false; // If there are any mismatching parantheses code will give an error
bool unnecessarySeparator = false; // If there exist any unnecessary separator, code will give an error
bool separatorNeeded = false;

int main()
{

    while (1) // LOOP which will repeat after taking input and calculating the result 
    {
        invalidTokenCheck = false; 
        paranthesesCheck = false;
        unnecessarySeparator = false;
        bool unexpected = false;
        separatorNeeded = false;
        
        char* mallc;
        char assignVariable[256] = {0}; 

        printf("> ");
        char* check = fgets(input, sizeof(input), stdin);
        if (check == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        if(input[0] == '\0'){
            continue;
        }

        int is_only_tabs_spaces = 1;

        char *ptr = strchr(input, '%'); // find the first occurrence of %
        if (ptr != NULL) { // if % is found
            *ptr = '\0'; // set % to null to ignore everything after it
        }

        for (int i = 0; i < strlen(input); i++) {
            if (input[i] == '\t') {
                input[i] = ' ';
            }
            if (input[i] != '\t' && input[i] != ' ') {
                is_only_tabs_spaces = 0;
            }
        }

        if(is_only_tabs_spaces){
            memset(input, 0, sizeof(input));
            continue;
        }

        int lenghtVar = 0;
        int i = 0;
        bool check_assign = strchr(input, '=') != NULL;
        bool existenceOfAssign = false; 

        if(input[0] == '='){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            continue;
        }
        else if (check_assign) {
            existenceOfAssign = true; 
            int word_count = 0;
            bool wordContinium = false;
            lenghtVar = 0;
            while((input[i] != '=')){
                if ((input[i] == 'x' && input[i+1] == 'o' && input[i+2] == 'r') || (input[i] == 'l' && input[i+1] == 's') || (input[i] == 'r' && input[i+1] == 's') || (input[i] == 'l' && input[i+1] == 'r' ) || (input[i] == 'r' && input[i+1] == 'r')  || (input[i] == 'n' && input[i+1] == 'o' && input[i+2] == 't')){
                    unexpected = true;
                    break;
                }
                if( !(input[i] >= 'a' && input[i] <= 'z' || input[i] >= 'A' && input[i] <= 'Z') &&  !(input[i] == ' ')){
                    unexpected = true;
                    break;
                }
                if( (input[i] >= 'a' && input[i] <= 'z' || input[i] >= 'A' && input[i] <= 'Z') && !wordContinium ){
                    wordContinium=true;
                    word_count++;
                }
                if((input[i] >= 'a' && input[i] <= 'z' || input[i] >= 'A' && input[i] <= 'Z') && wordContinium){
                    assignVariable[lenghtVar] = input[i]; 
                    lenghtVar++;
                }
                if( (input[i] == ' ') && wordContinium ){
                    wordContinium=false;
                }
                i++;
                
            }
            if(word_count > 1){
                printf("Error!\n");
                memset(input, 0, sizeof(input));
                continue;
            }
            mallc = (char*)malloc((lenghtVar+1)*sizeof(char));
            strcpy(mallc,assignVariable);
            i++;
        } 


        while (input[i] != '\0')
        {
            char ch = input[i];
            if(ch == '%'){
                break;
            }
            else if (ch >= '0' && ch <= '9')
            {
                // parse a number and store it as a token
                long long int num = (int)input[i] - (int)('0'); // initialize num to 0       
                while (input[i+1] >= '0' && input[i+1] <= '9')
                {
                    num = num * 10 + input[i+1] - '0';
                    i++;
                }
                Token token = {TOK_INTEGER, num};
                tokens[myIndex++] = token; // store the type in tokens
            }
            else if (ch == '+')
            {
                Token token = {TOK_PLUS, 31};
                tokens[myIndex++] = token;
            }
            else if (ch == '-')
            {
                Token token = {TOK_MINUS, 32};
                tokens[myIndex++] = token;
            }
            else if (ch == '*')
            {
                Token token = {TOK_TIMES, 33};
                tokens[myIndex++] = token;
            }
            else if (ch == '(')
            {
                Token token = {TOK_LPARENTHESES, 35};
                tokens[myIndex++] = token;
            }
            else if (ch == ')')
            {
                Token token = {TOK_RPARENTHESES, 36};
                tokens[myIndex++] = token;
            }
            else if (ch == '&')
            {
                Token token = {TOK_AND, 37};
                tokens[myIndex++] = token;
            }
            else if (ch == '|')
            {
                Token token = {TOK_OR, 38};
                tokens[myIndex++] = token;
            }
            else if(ch == 'x' && input[i+1] == 'o' && input[i+2] == 'r'){
                i += 2;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_XOR, 39};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }

            }
            else if(ch == 'l' && input[i+1] == 's'){
                i += 1;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_LS, 40};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }
            }
            else if(ch == 'r' && input[i+1] == 's'){
                i += 1;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_RS, 41};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }  
            }
            else if(ch == 'l' && input[i+1] == 'r'){
                i += 1;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_LR, 42};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }
            }
            else if(ch == 'r' && input[i+1] == 'r'){
                i += 1;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_RR, 43};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }     
            }
            else if(ch == 'n' && input[i+1] == 'o' && input[i+2] == 't'){
                i += 2;
                bool spaced = false;
                while(input[i+1] == ' ' || input[i+1] == '\t'){ //  input[i+1] == '\0' gibi bişiler lazım olursa eklenir 
                    i++;
                    spaced = true;
                }
                if(input[i+1] == '('){
                    Token token = {TOK_NOT, 44};
                    tokens[myIndex++] = token;
                }
                else if(!isalpha(input[i+1])){
                    paranthesesCheck = true;
                    break;  
                }
                else if(isalpha(input[i+1]) && !spaced){
                    int size = 0;
                    char search[256] = {0};
                    search[0] = input[i];
                    size++;
                    while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                        i++;
                        search[size] = input[i];
                        size++;
                    }             
                    char assignCopy[size+1]; // !! Not sure it is correct
                    strcpy(assignCopy, search);
                    long long int value = searchTable(assignCopy);     
                    Token token = {TOK_VARIABLE, value};
                    tokens[myIndex++] = token;

                }
                else if(isalpha(input[i+1]) && spaced){
                    paranthesesCheck = true;
                    break;
                }
            }

            else if ((ch == 'l' && input[i+1] == 's' && !isalpha(input[i+2])) || (ch == 'r' && input[i+1] == 's' && !isalpha(input[i+2])) || (ch == 'l' && input[i+1] == 'r' && !isalpha(input[i+2])) || (ch == 'r' && input[i+1] == 'r' && !isalpha(input[i+2])) || (ch == 'n' && input[i+1] == 'o' && input[i+2] == 't' && !isalpha(input[i+3])) ){
                paranthesesCheck = true;
                break;
            }

            else if (ch == ',')
            {
                Token token = {TOK_FUNCTIONSEPERATOR, 45};
                tokens[myIndex++] = token;
            }
            else if( (input[i] >= 'a' && input[i] <= 'z' || input[i] >= 'A' && input[i] <= 'Z')){

                int size = 0;
                char search[256] = {0};
                search[0] = input[i];
                size++;
                while( (input[i+1] >= 'a' && input[i+1] <= 'z' || input[i+1] >= 'A' && input[i+1] <= 'Z')){
                    i++;
                    search[size] = input[i];
                    size++;
                }             
                char assignCopy[size+1]; // !! Not sure it is correct
                strcpy(assignCopy, search);
                long long int value = searchTable(assignCopy);     
                Token token = {TOK_VARIABLE, value};
                tokens[myIndex++] = token;
            }
            else if( !(input[i] >= 'a' && input[i] <= 'z' || input[i] >= 'A' && input[i] <= 'Z') &&  !(input[i] == ' ')){ 
                unexpected = true;
                break;
            }           

            i++;
        }
        
        if(unexpected || errorHandler()){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            memset(tokens,0, sizeof(tokens));
            myIndex = 0;
            continue;
        }

        if(myIndex == 0){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            memset(tokens, 0, sizeof(tokens));
            myIndex = 0;
            continue;
        }

        myIndex = 0;
        current_token = &tokens[0];
        Node* term = parse_Exp();        

        if(paranthesesCheck){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            memset(tokens, 0, sizeof(tokens));
            myIndex = 0;
            continue;
        }

        if(unnecessarySeparator){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            memset(tokens, 0, sizeof(tokens));
            myIndex = 0;
            continue;
        }

        long long int answer = evaluator(term); 

        if(invalidTokenCheck){
            printf("Error!\n");
            memset(input, 0, sizeof(input));
            memset(tokens, 0, sizeof(tokens));
            myIndex = 0;
            continue;
        }
        if (existenceOfAssign) {              // assign varsa variable'a esitleyip hash'e at yoksa printle 
            insertTable(mallc,answer);
        }
        else{
            printf("%lli\n", answer);
        }


        memset(input, 0, sizeof(input));
        memset(tokens, 0, sizeof(tokens));
        myIndex = 0;


    } // outside of while loop

    return 0;
}

void nextToken() {

    
    myIndex++;
    current_token = &tokens[myIndex];

};


Node* nodeCreator(TokenType operator1,Node* term1, Node* next_term1, Token* token){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->token = token;
    newNode->operator = operator1;
    newNode->leftchild = term1;
    newNode->rightchild = next_term1;


    return newNode;  
};


Node* parse_Exp(){
    Node* and = parse_and();
    while ( current_token->type == TOK_OR){
        TokenType nodeOperator = current_token->type;
        nextToken();
        Node* next_and = parse_and();
        and = nodeCreator(nodeOperator,and,next_and,NULL);
    };
    return and;
};

Node* parse_and(){  
    Node* plus = parse_plus();
    while ( current_token->type == TOK_AND){
        TokenType nodeOperator = current_token->type;
        nextToken();
        Node* next_plus = parse_plus();
        plus = nodeCreator(nodeOperator,plus,next_plus,NULL);
    };
    return plus;
};

Node* parse_plus(){ 
    Node* term = parse_term();
    while ( current_token->type == TOK_PLUS ||current_token->type == TOK_MINUS){
        TokenType nodeOperator = current_token->type; 
        nextToken();
        Node* next_term = parse_term();
        term = nodeCreator(nodeOperator,term,next_term,NULL);
    };
    return term;
};

Node* parse_term(){  
    Node* separator = parse_separator();
    while ( current_token->type == TOK_TIMES){
        TokenType nodeOperator = current_token->type;
        nextToken();
        Node* next_separator = parse_separator();
        separator = nodeCreator(nodeOperator,separator,next_separator,NULL);
    };
    return separator;
};

Node* parse_separator(){  
    Node* functions = parse_functions();
    if ( current_token->type == TOK_FUNCTIONSEPERATOR && separatorNeeded == false){
        unnecessarySeparator = true;
    };
    return functions;
};


Node* parse_functions(){ 
    Node* not;
    if(current_token->type == TOK_XOR || current_token->type == TOK_LS || current_token->type == TOK_RS || current_token->type == TOK_LR || current_token->type == TOK_RR) {
        TokenType nodeOperator = current_token->type; 
        nextToken(); 
        if (current_token->type != TOK_LPARENTHESES){
            paranthesesCheck = true;
            return NULL; 
        }
        nextToken(); 
        separatorNeeded = true;
        not = parse_Exp(); 
        if (current_token->type != TOK_FUNCTIONSEPERATOR ){
            paranthesesCheck = true;
            return NULL; 
        };
        nextToken();
        Node* next_not = parse_Exp(); 
           
        if (current_token->type != TOK_RPARENTHESES ){ 
            paranthesesCheck = true;
        };
        nextToken();
        not = nodeCreator(nodeOperator,not,next_not,NULL); 
        
    } else {
        not = parse_not();
    };
    return not;

};

Node* parse_not() {
    Node* factor; 
    if(current_token->type != TOK_NOT){
       factor = parse_factor();
    }

    else{
        while ( current_token->type == TOK_NOT){
            TokenType nodeOperator = current_token->type;
            nextToken();
            if (current_token->type != TOK_LPARENTHESES){
                paranthesesCheck = true;
                return NULL;
            }
            nextToken();
            Node* only_factor = parse_Exp();
             
            if (current_token->type != TOK_RPARENTHESES ){ 
                paranthesesCheck = true;
            };
            nextToken();

            factor = nodeCreator(nodeOperator,only_factor,NULL,NULL);
        };
    };
    return factor;
};

Node* parse_factor(){
    if ( current_token->type == TOK_VARIABLE){
        Node* node = nodeCreator(current_token->type,NULL,NULL,current_token);
        nextToken();
        return node;
    }

    else if ( current_token->type == TOK_INTEGER){
        Node* node = nodeCreator(current_token->type,NULL,NULL,current_token);
        nextToken();
        return node;
    }

    else if ( current_token->type == TOK_LPARENTHESES){
        nextToken();
        Node* expr = parse_Exp();
        if (current_token->type != TOK_RPARENTHESES ){
            paranthesesCheck = true;
        };
        nextToken();
        return expr;
    }

    else{
        invalidTokenCheck = true;
        return nodeCreator(TOK_INVALID,NULL,NULL,NULL);;
    }
}


long long int evaluator(Node* root){           
    if (root->operator == TOK_MINUS){
        return evaluator(root->leftchild) - evaluator(root->rightchild);
    }
    else if (root->operator == TOK_PLUS){
        return evaluator(root->leftchild) + evaluator(root->rightchild);
    }
    else if (root->operator == TOK_TIMES){
        return evaluator(root->leftchild) * evaluator(root->rightchild);
    }
    else if (root->operator == TOK_AND){ 
        return evaluator(root->leftchild) & evaluator(root->rightchild);
    }
    else if (root->operator == TOK_OR){ 
        return evaluator(root->leftchild) | evaluator(root->rightchild);
    }    
    else if (root->operator == TOK_XOR){ 
        return evaluator(root->leftchild) ^ evaluator(root->rightchild);
    }
    else if (root->operator == TOK_LS){ 
        long long int right = evaluator(root->rightchild);
        if(right < 0){
            invalidTokenCheck = true;
            return -99999;
        }
        return evaluator(root->leftchild) << right;
    }
    else if (root->operator == TOK_RS){ 
        long long int right = evaluator(root->rightchild);
        if(right < 0){
            invalidTokenCheck = true;
            return -99999;
        }
        return evaluator(root->leftchild) >> evaluator(root->rightchild);
    }
    else if (root->operator == TOK_LR){ 
        long long int right = evaluator(root->rightchild);
        if(right < 0){
            invalidTokenCheck = true;
            return -99999;
        }
        return (evaluator(root->leftchild) << evaluator(root->rightchild)) | (evaluator(root->leftchild) >> (sizeof(long long int) * 64 - evaluator(root->rightchild)));  //changed int
    }
    else if (root->operator == TOK_RR){ 
        long long int right = evaluator(root->rightchild);
        if(right < 0){
            invalidTokenCheck = true;
            return -99999;
        }
        return (evaluator(root->leftchild)  >> evaluator(root->rightchild)) | (evaluator(root->leftchild) << (sizeof(long long int) * 64 - evaluator(root->rightchild)));      //changed int
    }
    else if (root->operator == TOK_NOT){ 
        return ~evaluator(root->leftchild);
    }    
    else if (root->operator == TOK_VARIABLE){
        return root->token->value;
    }
    else if (root->operator == TOK_INTEGER){
        return root->token->value;
    }
    else if (root->operator == TOK_INVALID){
        invalidTokenCheck = true;
        return -99999;
    }
    return -1;
}



// *****************************************************************************************************************************************************


unsigned long long int hasher(char *key) {          
    unsigned long long int hashval = 5381; // Starting point for hash value          
    long long int c;            
    
    while ((c = *key++)) {
        hashval = ((hashval << 5) + hashval) + c; // Hash function to find unique key
    }
    
    return hashval;
}

void insertTable(char *key, long long int value) {              
    unsigned long long int index = hasher(key);  // finding key value for given char array         
    index = index % MAX_SIZE; 
    if (hashTable[index].key == NULL) { // Assigning key value pair to hash table if nothing exist
        hashTable[index].key = key;
        hashTable[index].value = value;
    } else {// Editing key value pair to hash table if key value pair already exist
        editTable(key, value);
    }
}


long long int searchTable(char *key) {          
    unsigned long long int index = hasher(key);  // finding key value for given char array       
    index = index % MAX_SIZE; 
    if (hashTable[index].key != NULL && strcmp(hashTable[index].key, key) == 0) { //Chechking if there is a stored value for the key
        return hashTable[index].value;
    }
    else {
        return 0;
    }
}

long long int editTable(char *key, long long int value) {              
    unsigned long long int index = hasher(key); // finding key value for given char array            
        index = index % MAX_SIZE; 

    if (hashTable[index].key != NULL && strcmp(hashTable[index].key, key) == 0) { //Chechking if there is a stored value for the key
        hashTable[index].value = value; // If there is a stored value it will be changed to new value;
        return hashTable[index].value;
    }
    return value;
}


// *****************************************************************************************************************************************************

bool errorHandler(){
    bool lastCheck = lastTokenChecker();
    bool sequentCheck = sequentTokenChecker();
    if(lastCheck || sequentCheck){
        return true;
    }
    return false;
}


bool lastTokenChecker(){
    if (myIndex == 0)
    {
        return false;
    }
    Token lastToken = tokens[myIndex - 1];
    if(lastToken.type != TOK_INTEGER && lastToken.type != TOK_VARIABLE && lastToken.type != TOK_RPARENTHESES){ // If last token is not variable, integer or right parantheses it will give an error
        return true;
    }
    return false;
}


bool sequentTokenChecker(){
        Token current;
        Token next;
        int counterL = 0;
        int counterR = 0;

        if (myIndex == 0)
        {
            return false;
        }
        for (int i = 1; i < myIndex; i++)
        {   
            current = tokens[i-1];
            next = tokens[i];

            if(current.type == TOK_RPARENTHESES){
                counterR++;
            }
            if(current.type == TOK_LPARENTHESES){
                counterL++;
            }    



            if(current.type == next.type && (current.type == TOK_VARIABLE || current.type == TOK_INTEGER)){ // Traversing whole token array and if there are more than one subseqeunt integer or variable token it will give an error 
                return true;
            }
            
            if((current.type == TOK_VARIABLE && next.type == TOK_INTEGER) || (next.type == TOK_VARIABLE && current.type == TOK_INTEGER)){ // Traversing whole token array and if there are more than one subseqeunt integer and variable token it will give an error 
                return true;
            }
            if((current.type == TOK_VARIABLE && next.type == TOK_LPARENTHESES) || (current.type == TOK_INTEGER && next.type == TOK_LPARENTHESES)){ // Traversing whole token array and if there are more than one subseqeunt integer or variable token it will give an error 
                return true;
            }
            if((current.type == TOK_RPARENTHESES && next.type == TOK_VARIABLE) || (current.type == TOK_RPARENTHESES && next.type == TOK_INTEGER)){ // Traversing whole token array and if there are more than one subseqeunt integer or variable token it will give an error 
                return true;
            }
            if((current.type == TOK_INTEGER && next.type == TOK_XOR) || (current.type == TOK_INTEGER && next.type == TOK_LS)  || (current.type == TOK_INTEGER && next.type == TOK_RS)  || (current.type == TOK_INTEGER && next.type == TOK_LR) ||(current.type == TOK_INTEGER && next.type == TOK_RR) || (current.type == TOK_INTEGER && next.type == TOK_NOT) ){
                return true;
            }
            if((current.type == TOK_RPARENTHESES && next.type == TOK_XOR) || (current.type == TOK_RPARENTHESES && next.type == TOK_LS) ||(current.type == TOK_RPARENTHESES && next.type == TOK_RS)||(current.type == TOK_RPARENTHESES && next.type == TOK_LR)||(current.type == TOK_RPARENTHESES && next.type == TOK_RR)||(current.type == TOK_RPARENTHESES && next.type == TOK_NOT) ){
                return true;
            }
        }

        if(tokens[myIndex-1].type == TOK_RPARENTHESES){
            counterR++;
        }
        if(tokens[myIndex-1].type == TOK_LPARENTHESES){
            counterL++;
        }         

        if(counterL != counterR){
            return true;
        }
        return false;
}
