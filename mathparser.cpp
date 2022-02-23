/******************************************************************

Introduction:
mathparser is a simple c++ program to parse math expressions.

The program is a modified version of math expression parser 
presented in the book : "C++ The Complete Reference" by H.Schildt.

It supports operators: + - * / ^ ( )

It supports math functions : SIN, COS, TAN, ASIN, ACOS, ATAN, SINH, 
COSH, TANH, ASINH, ACOSH, ATANH, LN, LOG, EXP, SQRT, SQR, ROUND, INT.

It supports variables A to Z.

Sample:
25 * 3 + 1.5*(-2 ^ 4 * log(30) / 3)
x = 3
y = 4
r = sqrt(x ^ 2 + y ^ 2)
t = atan(y / x)

mathparser version 1.0 by Hamid Soltani. (gmail: hsoltanim)
Last modified: Aug. 2016.

*******************************************************************/

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <map>
#include <math.h>

#define PI 3.14159265358979323846

using namespace std;

enum types { DELIMITER = 1, VARIABLE, NUMBER, FUNCTION };
class parser {
    char* exp_ptr;     // points to the expression
    char  token[256];  // holds current token
    char  tok_type;    // holds token's type
    void  eval_exp1(double& result);
    void  eval_exp2(double& result);
    void  eval_exp3(double& result);
    void  eval_exp4(double& result);
    void  eval_exp5(double& result);
    void  eval_exp6(double& result);
    void  get_token();

    //    std::map<std::string, float> vars;

    void print_vars();

public:
    parser();
    double eval_exp(char* exp);
    char   errormsg[64];
};
// Parser constructor.
parser::parser() {
    int i;
    exp_ptr = NULL;
    //    vars.clear();
    errormsg[0] = '\0';
}
#if 0
void parser::print_vars() {
    for (const auto& n : vars) {
        cout << n.first << " = " << n.second << endl;
    }
}
#endif
// Parser entry point.
double parser::eval_exp(char* exp) {
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token) {
        strcpy(errormsg, "End");  // no expression present
        return (double)0;
    }
    eval_exp1(result);
    if (*token)  // last token must be null
        strcpy(errormsg, "Syntax Error");
    return result;
}
// Process an assignment.
void parser::eval_exp1(double& result) {
    int  slot;
    char temp_token[80];
    if (tok_type == VARIABLE) {
        // save old token
        char* t_ptr = exp_ptr;
        strcpy(temp_token, token);
        // compute the index of the variable

        get_token();
        if (*token != '=') {
            exp_ptr = t_ptr;            // return current token
            strcpy(token, temp_token);  // restore old token
            tok_type = VARIABLE;
        } else {
            get_token();  // get next part of exp
            eval_exp2(result);
            //            vars[std::string(temp_token)] = result;
            // print_vars();
            return;
        }
    }
    eval_exp2(result);
}
// Add or subtract two terms.
void parser::eval_exp2(double& result) {
    char   op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-') {
        get_token();
        eval_exp3(temp);
        switch (op) {
            case '-':
                result = result - temp;
                break;
            case '+':
                result = result + temp;
                break;
        }
    }
}
// Multiply or divide two factors.
void parser::eval_exp3(double& result) {
    char   op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/') {
        get_token();
        eval_exp4(temp);
        switch (op) {
            case '*':
                result = result * temp;
                break;
            case '/':
                result = result / temp;
                break;
        }
    }
}
// Process an exponent.
void parser::eval_exp4(double& result) {
    double temp;
    eval_exp5(result);
    while (*token == '^') {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}
// Evaluate a unary + or -.
void parser::eval_exp5(double& result) {
    char op;
    op = 0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-') {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-')
        result = -result;
}
// Process a function, a parenthesized expression, a value or a variable
void parser::eval_exp6(double& result) {
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc) {
        strcpy(temp_token, token);
        get_token();
    }
    if ((*token == '[')) {
        get_token();
        eval_exp2(result);
        if (*token != ']')
            strcpy(errormsg, "Unbalanced Parentheses");
        if (isfunc) {
            if (!strcmp(temp_token, "SIN"))
                result = sin(PI / 180 * result);
            else if (!strcmp(temp_token, "COS"))
                result = cos(PI / 180 * result);
            else if (!strcmp(temp_token, "TAN"))
                result = tan(PI / 180 * result);
            else if (!strcmp(temp_token, "ASIN"))
                result = 180 / PI * asin(result);
            else if (!strcmp(temp_token, "ACOS"))
                result = 180 / PI * acos(result);
            else if (!strcmp(temp_token, "ATAN"))  // LinuxCNC has ATAN[arg]/[arg]
                result = 180 / PI * atan(result);
#if 0
            else if (!strcmp(temp_token, "SINH")) // Not in LinuxCNC
                result = sinh(result);
            else if (!strcmp(temp_token, "COSH"))
                result = cosh(result);
            else if (!strcmp(temp_token, "TANH"))
                result = tanh(result);
            else if (!strcmp(temp_token, "ASINH"))
                result = asinh(result);
            else if (!strcmp(temp_token, "ACOSH"))
                result = acosh(result);
            else if (!strcmp(temp_token, "ATANH"))
                result = atanh(result);
            else if (!strcmp(temp_token, "LOG"))
                result = log10(result);
            else if (!strcmp(temp_token, "SQR"))
                result = result * result;
#endif
            else if (!strcmp(temp_token, "LN"))
                result = log(result);
            else if (!strcmp(temp_token, "EXP"))
                result = exp(result);
            else if (!strcmp(temp_token, "SQRT"))
                result = sqrt(result);
            else if (!strcmp(temp_token, "ROUND"))
                result = round(result);
            else if (!strcmp(temp_token, "FIX"))
                result = floor(result);
            else if (!strcmp(temp_token, "FUP"))
                result = ceil(result);
            else
                strcpy(errormsg, "Unknown Function");
        }
        get_token();
    } else {
        switch (tok_type) {
            case VARIABLE:
                result = vars[std::string(token)];
                get_token();
                return;
            case NUMBER:
                result = atof(token);
                get_token();
                return;
            default:
                strcpy(errormsg, "Syntax Error");
        }
    }
}
// Obtain the next token.
void parser::get_token() {
    char* temp;
    tok_type = 0;
    temp     = token;
    *temp    = '\0';
    if (!*exp_ptr)  // at end of expression
        return;
    while (isspace(*exp_ptr))  // skip over white space
        ++exp_ptr;
    if (strchr("+-*/%=[]", *exp_ptr)) {
        tok_type = DELIMITER;
        // Translate "**" to "^"
        if (*exp_ptr == '*' && exp_ptr[1] == '*') {
            *temp++ = '^';
            exp_ptr += 2;
        } else {
            *temp++ = *exp_ptr++;  // advance to next char
        }
    } else if (*exp_ptr == '#') {  // GCode variable
        tok_type = VARIABLE;
        ++exp_ptr;
        if (*exp_ptr == '<') {  // named variable
            ++exp_ptr;
            while (*exp_ptr != '>') {
                if (!*exp_ptr) {
                    // tok_type = SYNTAX_ERROR
                    return;
                }
                if (isspace(*exp_ptr)) {
                    ++exp_ptr;
                    continue;
                }
                if (isalpha(*exp_ptr)) {
                    *temp++ = tolower(*exp_ptr++);
                    continue;
                }
                if (isdigit(*exp_ptr) || *exp_ptr == '_') {
                    *temp++ = *exp_ptr++;
                    continue;
                }
                // tok_type = SYNTAX_ERROR
                return;
            }
            ++exp_ptr;
        } else {  // numbered variable
            while (isdigit(*exp_ptr)) {
                *temp++ = *exp_ptr++;
            }
        }
    } else if (isalpha(*exp_ptr)) {
        while (!strchr(" +-/*%=[]\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        while (isspace(*exp_ptr))  // skip over white space
            ++exp_ptr;
        tok_type = (*exp_ptr == '[') ? FUNCTION : VARIABLE;
    } else if (isdigit(*exp_ptr) || *exp_ptr == '.') {
        while (!strchr(" +-/*%=[]\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    //    if ((tok_type == VARIABLE) && (token[1]))
    //        strcpy(errormsg, "Only first letter of variables is considered");
}

int main() {
    char   expstr[256];
    parser ob;
    cout << "Math expression parser. Enter a blank line to stop.\n\n";
    do {
        cout << "Enter expression: ";
        cin.getline(expstr, 255);
        double ans = ob.eval_exp(expstr);
        if (*ob.errormsg)
            cout << "Error: " << ob.errormsg << "\n\n";
        else
            cout << "Answer: " << ans << "\n\n";
    } while (*expstr);
    return 0;
}
