#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>






//*****************************************************

char* getBracket_name(char ch){
    if(ch == '('){
        return "LeftPar";
    }else if(ch == ')'){
        return "RightPar";
    }else if(ch == '['){
        return "LeftSquareBracket";
    }else if(ch == ']'){
        return "RightSquareBracket";
    }else if(ch == '{'){
        return "LeftCurlyBracket";
    }else if(ch == '}'){
        return "RightCurlyBracket";
    }else{
        return NULL;
    }
}
//4- Brackets: Returns 'true' if the character is a BRACKET.
bool isBrackets(char ch)
{
    /*
    •	LeftPar: (       			RightPar: )
    •	LeftSquareBracket: [		RightSquareBracket: ]
    •	LeftCurlyBracket:  {		RightCurlyBracket:  }
    •	Example Token: LeftCurlyBracket
     */
    if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}'){
        return (true);
    }
    return (false);
}
//*****************************************************

//3- Operators: Returns 'true' if the character is an OPERATOR.
/*
    •	Valid operators of the language are +, -, *, /, ++, --, :=
    •	Example Token: Operator(++)
*/
bool isOperator(char* character) //CHAR* olarak kullanman gerek.
{

    if ((strcmp(character,"+")==0) || (strcmp(character,"-")==0) || (strcmp(character,"*")==0) || (strcmp(character,"/")==0) || (strcmp(character,"++")==0)|| (strcmp(character,"--")==0) || (strcmp(character,":=")==0)) {
        return (true);
    }
    return (false);
}

bool is_1_Operator(char ch[1])
{
    if ((strcmp(ch,"+")==0) || (strcmp(ch,"-")==0) || (strcmp(ch,"*")==0) || (strcmp(ch,"/")==0) ||
        (strcmp(ch,"=")==0) || (strcmp(ch,":")==0))
    {
        return (true);
    }
    else{
        return (false);

    }
}

//*******************************************************************************

//6- Keywords: Returns 'true' if the string is a KEYWORD.
/*
    break, case, char, const, continue, do, else, enum, float, for, goto, if, int, long, record, return, static, while
    Psi++ language is not case sensitive, and all the keywords are standardized as lower case. You can write the same word as “while” OR “While” OR “WHILE” and they all generate the
    Example Token: Keyword(while)
 * */


bool isKeyword(char* str)
{
    str = strlwr(str);
    if (!strcmp(str, "if") || !strcmp(str, "else") ||
        !strcmp(str, "while") || !strcmp(str, "do") ||
        !strcmp(str, "break") || !strcmp(str, "continue") ||
        !strcmp(str, "int") || !strcmp(str, "double") ||
        !strcmp(str, "float") || !strcmp(str, "return") ||
        !strcmp(str, "char") || !strcmp(str, "case") ||
        !strcmp(str, "for") || !strcmp(str, "long") ||
        !strcmp(str, "short") || !strcmp(str, "const") ||
        !strcmp(str, "void") || !strcmp(str, "static") ||
        !strcmp(str, "enum") || !strcmp(str, "goto") || !strcmp(str, "record")) {
        return (true);
    }
    return (false);
}

//*******************************************************************************


char fpeek(FILE *stream){
    char c;

    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}
char* two_char_to_str(char chr1, char chr2){
    char* temp = (char*) malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}
char* char_to_str(char chr){
    char* temp = (char*) malloc(2);
    temp[0] = chr;
    temp[1] = '\0';
    return temp;
}

// 1- Identifiers: Returns 'true' if the string is a VALID IDENTIFIER.
/*
•	The maximum identifier size is 30 characters. If you use an identifier larger than that, the lexical analyzer issues an error message.
•	Psi++ language is not case sensitive, and all the identifier names are standardized as lower case.
•	Identifiers start with an alphabetic character (a letter) and are composed of one or more letters/digits/_ (underscore)
•	Example Token: Identifier(my_var_1)

 * */

char identifier_or_keyword(char ch, FILE* f_read_Ptr, FILE *f_write_Ptr, char str_id_temp[],int k){
    // There are 2 situation. These are keywords and Valid identifier.
    int IDENTIFIER_LIMIT = 30; //The maximum identifier size is 30 characters.
    strlwr(char_to_str(ch));
    str_id_temp[k] = ch;

    //this loop checks EOF, Did k reach the top limit ,or it includes any operator except "_"
    while ((ch = fgetc(f_read_Ptr)) != EOF && (k <= IDENTIFIER_LIMIT) && (strcmp(char_to_str(ch), "\n")!=0) &&(isBrackets(ch) == false) && (is_1_Operator(char_to_str(ch)) == false) && (strcmp(char_to_str(ch)," ")!=0)&& (strcmp(char_to_str(ch),";")!=0)) {
        str_id_temp[++k] = ch;
    }
    if (k >= IDENTIFIER_LIMIT) {
        printf("ERROR : The maximum identifier size is 30 characters. You have exceeded the usage limit");

    }
        // str_id_temp is a keyword or not ?
    else if (isKeyword(str_id_temp)) {
        fprintf(f_write_Ptr, "'%s' is a KEYWORD\n", str_id_temp);


    }
    else if (isdigit(str_id_temp[0])){
        fprintf(f_write_Ptr, "It is not a valid identifier\n" );

    }
    else {
        fprintf(f_write_Ptr, "'%s' is an identifier\n", str_id_temp);

    }
    return ch;


}

//*******************************************************************************


int main() {
    //OPEN THE FILES TO READ AND WRITE;
    FILE* f_read_Ptr,*f_write_Ptr;
    f_read_Ptr = fopen("./code.psi","r");
    f_write_Ptr = fopen("./code.lex","w");
    //these are temp chars which are keeping the value for a temporary moment.
    char ch_op ;
    char ch;
    char* str_temp_2 = NULL;
    char* str_op_for_2_char;
    int i;

    //If the file could not be opened;
    if (f_read_Ptr == NULL && f_write_Ptr == NULL){
        printf("ERROR : Files could not be opened !");
        return 0;
    }

    //MAIN LOOP
    while((ch = fgetc(f_read_Ptr)) != EOF){

        //1) Ignore whitespace and end of lines
        if (ch == ' ' || ch== '\t' || strcmp(char_to_str(ch), "\n")==0) {
            continue;
        }
        //Check the ch, is it digit?
        if (isdigit(ch)){
            //numeric------------------------------------------------------------
            // 2- Integer constants : Returns 'true' if the string is an INTEGER.
            /*
            •	Maximum integer size is 10 digits. If you use an integer value longer than that, the lexical analyzer issues an error message.
            •	Negative values are not supported.
            •	Example Token: IntConst(1290)
            */
            int LIMIT = 11;
            i = 0; // it keeps the lenght of the integer
            char str_num_temp[10] = {0}; //create space up to max size;
            str_num_temp[i] = ch;

            //It checks whether the value is a digit or not. Also, it checks the Pointer about the EOF situation.
            while((ch = fgetc(f_read_Ptr)) != EOF && (!isspace(ch)) && (isdigit(ch))){
                str_num_temp[++i] = ch;

                //if the next value is not a digit;
                if(!isdigit(fpeek(f_read_Ptr))){
                    str_num_temp[++i] = '\0';
                    break;
                }
            }
            //after number there can be an alpha character or "_" // it will give a notification on code.lex
            if(isalpha(fpeek(f_read_Ptr)) || strcmp(char_to_str(fpeek(f_read_Ptr)), "_")==0){
                ch = identifier_or_keyword( ch, f_read_Ptr,f_write_Ptr,str_num_temp,i);
            }

            //Maximum integer size is 10 digits.
            // If you use an integer value longer than that, the lexical analyzer issues an error message.
            else if(i >= LIMIT){
                printf("ERROR: INTEGER CONSTANT is longer than 10 characters.\n");

            }else if(ch == EOF){
                printf("ERROR: END OF FILE.\n");
                return 0;
            }
            else{
                //Example Token: IntConst(1290) , it will write on the file (code.lex)
                fprintf(f_write_Ptr, "IntConst(%s)\n", str_num_temp);}



        }
        //Check the ch, is it alphabetic or not?
        if (isalpha(ch)) {
            char str_id_temp[30] = {0}; //create space up to max size;
            int k = 0;
            //checks the type of str_id_temp : keyword or identifier ;
            ch = identifier_or_keyword(ch,f_read_Ptr,f_write_Ptr, str_id_temp,k);
        }

        //5- String constants
        /*
        •	String constants of Psi++ are delimited by double quotes (ASCII code 34) as in “this is a string”
        •	String constants have UNLIMITED SIZE
        •	String constants cannot contain the double quote character. When you reach one, the string terminates.
        •	If a string constant cannot terminate before the file ends, there should be a lexical error issued.
        */
        if(ch=='"'){
            str_temp_2 = (char*) malloc(200);
            i = 0;
            while(((ch = fgetc(f_read_Ptr)) != EOF) ){
                str_temp_2[i++] = ch;
                if(fpeek(f_read_Ptr) == '"') {
                    fgetc(f_read_Ptr);
                    str_temp_2[i] = '\0';
                    break;
                }
            }

            if(ch == EOF){
                printf( "ERROR: String not closed\n");
                return 0;
            }else{
            fprintf(f_write_Ptr, "String  : '%s'\n", str_temp_2);}
            free(str_temp_2);

        }

        // COMMENT
        if(ch == '/' && fpeek(f_read_Ptr) == '*'){
            printf("COMMENT OPENED\n");
            fgetc(f_read_Ptr); // *
            while((ch = fgetc(f_read_Ptr)) != EOF){
                if(ch == '*' && fpeek(f_read_Ptr) == '/'){
                    ch = fgetc(f_read_Ptr); // waste the slash (/)
                    break;
                }
            }
            if(ch == EOF){
                //RAISE AN ERROR comment not closed
                printf( "ERROR: Comment not closed\n");
                return 0;

            }
            printf("COMMENT CLOSED\n");

        }

        // is operator -> Example Token: Operator(++)------------------------------------

        if(is_1_Operator(str_op_for_2_char = char_to_str(ch))){
            ch_op = fpeek(f_read_Ptr);
            if (ch_op == '+' || ch_op == '-' || ch_op == '='){
                ch_op = fgetc(f_read_Ptr);
                str_op_for_2_char = two_char_to_str(ch,ch_op);
            }

            if (isOperator(str_op_for_2_char)){
                fprintf(f_write_Ptr, "Operator(%s)\n", str_op_for_2_char);
            }

        }

        //brackets
        if (isBrackets(ch) == true) {
            fprintf(f_write_Ptr, "It is '%s' \n", getBracket_name(ch));
        }
        // END OF LINE
        if(ch == ';'){
            fprintf(f_write_Ptr, "EndOfLine\n");

        }


    }
    fclose(f_read_Ptr);
    fclose(f_write_Ptr);
    return 0;



}

