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

        else{
            // print out each regular character
            putchar(*format);
            //increment the character counter
            count++;
        }
        format++;

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
        *length = *p++;
    }

    // if *p is a d, x, c, or s
    if (*p == 'd' || *p == 'x' || *p == 'c' || *p == 's') {
        *specifier = *p++;
    }

    // update the format pointer
    *format = p;

    }

void forInt(int num, int width, int precision, char flags[5], char length, int *count){
    // if the inputted int is negative
    if (num < 0){
        // print out a negative sign
        putchar('-');
        // increment the char count
        *count += 1;
        // make the number positive so you get rid of the '-' that you already printed
        num = -num;
//
  }
    // if num = 0
    else if (num == 0){
        // print out a 0
        putchar('0');
        // increment the char count
        *count += 1;
        // there's nothing more to print so return
        return;
    }
    // place to store the digits
    char numString[50];
    int i = 0;


    // while num > 0
    while (num > 0) {
        // fill in numString index i with the last digit of num and change it to a char
        numString[i] = (num % 10) + '0';
        // increment i to the next index
        i++;
        // take the last digit off of num
        num /= 10;
    }
    // subract 1 from i because it gets incremented one extra time
    i--;
    // while there are still numbers in numString to be printed
    while(i >= 0){
        // print the numbers from the end of numString until the beginning because the order's reversed
        putchar(numString[i]);
        *count += 1;
        i--;
    }

}



int main() {
    char s[] = "wow";
    printf("%-d\n", 322/10);
    printF("this %d %d", 5, 3);
    return 0;
}
