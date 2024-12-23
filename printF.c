#include <stdio.h>
#include <stdarg.h>

// declare functions
void formatting(const char **format, char flags[5], int *width, int *precision, char *length, char *specifier);
void forInt(int num, int width, int precision, char flags[5], char length, int *count);

int printF(const char *format, ...){
    // list of variable arguments
    va_list args;
    // initialize the arg list with the last non-variable argument of the function
    va_start(args, format);

    // list to hold all possible flags
    char flags[5]= {'\0', '\0', '\0', '\0', '\0'};
    // var for the width
    int width = 0 ;
    // var for the precision
    int precision = 0;
    //var for the length modifier
    char length = '\0';
    //var for the specifier
    char specifier = '\0';

    // counter for return to be the total characters printed
    int count = 0;

    // loops through each character of the format string
    while(*format){
        // if it's the %, it's the start of the format specifier
        if (*format == '%'){
            // move on to the next char
            format++;


            // go through the format string to break it up
            formatting(&format, flags, &width, &precision, &length, &specifier);

            // check the specifier
            if (specifier == 'd') {
                forInt(va_arg(args, int), width, precision, flags, length, &count);
            }
        }

        // account for escape sequences ('\' situations)
        else if (*format == '\\'){
            // move on to the next char
            format++;

            // check different cases for that char
            switch(*format){
                // newline char
                case 'n':
                    putchar('\n');
                    count++;
                    break;
                // tab
                case 't':
                    putchar('\t');
                    count++;
                    break;
                // backslash
                case '\\':
                    putchar('\\');
                    count++;
                    break;
                // double quote
                case '"':
                    putchar('"');
                    count++;
                    break;
                // carriage return
                case 'r':
                    putchar('\r');
                    count++;
                    break;
                // backspace
                case 'b':
                    putchar('\b');
                    count++;
                    break;
                // form feed
                case 'f':
                    putchar('\f');
                    count++;
                    break;
                default:
                    // print nothing for unknown escape sequence
                    break;


            }

        }

        else{
            // print out each regular character
            putchar(*format);
            //increment the character counter
            count++;
            // move to the next char
            format++;
        }



    }

    va_end(args);

    return count;
}


void formatting(const char **format, char flags[5],  int *width, int *precision, char *length, char *specifier) {
    // p is a pointer to the char that the format string is up to
    const char *p = *format;

    // initialize the flag, width precision length and specifier
    for (int i = 0; i < 5; i++) {
        flags[i] = '\0';
    }
    *width = 0;
    *precision = 0;
    *length = '\0';
    *specifier = '\0';

    // while *p is one of the flags
    while (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0' || *p == '\'') {
        // set the flags in the list to whichever flags the string contains
        if (*p == '-') {
            // left-align
            flags[0] = '-';
        } else if (*p == '+') {
            // + in front of pos numbers
            flags[1] = '+';
        } else if (*p == ' ') {
            // space in front of pos numbers
            flags[2] = ' ';
        } else if (*p == '0') {
            // when width is specified fill in 0s for numbers
            flags[3] = '0';
        } else if (*p == '#') {
            // for %x start hex number output with 0x
            flags[4] = '#';
        }

        // move the pointer to the next char
        p++;
    }

    // after the flags if *p is a digit, it's the width
    while (*p >= '0' && *p <= '9') {
        // build up the width one digit at a time
        // convert it to an int and shift it by one decimal place each time
        *width = (*width * 10) + (*p - '0');
        //move the pointer to the next char
        p++;
    }

    // if *p is a '.' then it has a precision specification
    if (*p == '.') {
        // skip the . and get the number after it
        p++;
        // while p is a number
        while (*p >= '0' && *p <= '9') {
            // same logic as the width number
            // build up the precision one digit at a time
            // convert it to an int and shift it by one decimal place each time
            *precision = (*precision * 10) + (*p - '0');
            p++;
        }
    }

    // while *p is hh, h, l , or ll
    while (*p == 'h' || *p == 'l') {
        // set length to the length modifier
        *length = *p;
        //increment p to the next char
        p++;
    }

    // if *p is a d, x, c, or s
    if (*p == 'd' || *p == 'x' || *p == 'c' || *p == 's') {
        // set the specifier to be the specifier char
        *specifier = *p;
        // increment p to the next char
        p++;
    }

    // update the format pointer
    *format = p;

    }

void forInt(int num, int width, int precision, char flags[5], char length, int *count) {
    // determine if the num is negative
    int isNeg = 0;
    // keep track of how much padding needed
    int padding = 0;
    // if precision > length of the num, 0s need to be filled in
    int precisionZeros = 0;

    // if num is negative
    if (num < 0) {
        // set isNeg to 1
        isNeg = 1;
        // make num pos
        num = -num;
    }
    // place to store the numbers before they can be printed
    char numString[50];
    // index value
    int i = 0;
    // store the length of num so if there's a width that's greater than the len
    // the padding could know how much space it needs
    int lenNum = 0;


    // if num is 0
    if (num == 0) {
        // put a 0 in numString
        numString[i++] = '0';
    }
    else {
        // while num still has values to be printed
        while (num > 0) {
            // fill in numString index i with the last digit of num and change it to a char
            // increment i to the next index
            numString[i++] = (num % 10) + '0';
            // take the last digit off of num
            num /= 10;
        }

        // the len of num is the index + 1 because the index starts at 0
        lenNum = i;
    }
    // figure out how much padding you need

    // if the precision is greater than the len of num, then it needs to be filled in with 0s
    if (precision > lenNum) {
        // set the number of leading 0s to be the difference of precision and lenNum
        precisionZeros = precision - lenNum;
    }
    int totWidth = lenNum + precisionZeros;


    // if the num needs a +/- sign
    if (isNeg || flags[1] == '+'){
        // add to the total width to account for signs (+/-)
        totWidth++;
    }
    // if the width is greater than the total width
    if (width > totWidth) {
        // account for that padding
        padding = width - totWidth;
    }
    // if padding is necessary and it's not left-aligned
    if (padding > 0 && flags[0] != '-') {
        // the zero gets overwritten by the ' ' and "+" flags
        // so make sure those flags aren't present
        if (flags[2] == ' '){
            // print spaces
            putchar(' ');
            // increment the char counter
            *count += 1;
            // decrement padding
            padding--;
        }
        if (flags[3] == '0' && flags[1] != '+') {
            // while there's padding
            while (padding--) {
                // print spaces
                putchar('0');
                // increment the char counter
                *count += 1;
            }

        }

            // if there's space padding, and it's not left-aligned, handle it before the signs
        else {
            // while there's space padding
            while (padding--) {
                // print spaces
                putchar(' ');
                // increment the char counter
                *count += 1;
            }
        }
    }

    // if there's a + flag and it's positive
    if (flags[1] == '+' && !isNeg) {
        // print a +
        putchar('+');
        // increment the char counter
        *count += 1;
    }

    // if num is negative
    else if (isNeg) {
        // print a - before the 0s
        putchar('-');
        // increment the char counter
        *count += 1;

    }


    // while there are more leading 0s
    while (precisionZeros--){
        // print a 0
        putchar('0');
        // increment the char counter
        *count += 1;
    }



    // while there are numbers to print
    while (i--){
        // print the value at the end of the string because it was appended in reverse order
        putchar(numString[i]);
        // increment the counter
        *count += 1;
    }

    // if output is left aligned
    if (flags[0] == '-'){
        while (padding--){
            // print spaces
            putchar(' ');
            // increment counter
            *count += 1;
        }
    }





}

int main() {
    printf("%+09.6d\n", 123); // with width 9 and precision 6
    printF("%+09.6d\n", 123);  // custom printF implementation

    printf("% 09d\n", 123);     // with width 9
    printF("% 09d\n", 123);     // custom printF implementation

    printf("%-9d\n", 123);     // left-aligned with width 9
    printF("%-9d\n", 123);     // custom printF implementation

    printf("%09d\n", 123);     // with zero-padding width 9
    printF("%09d\n", 123);     // custom printF implementation

    printf("%9.4d\n", 123);    // with width 9 and precision 4
    printF("%9.4d\n", 123);    // custom printF implementation

    return 0;
}
