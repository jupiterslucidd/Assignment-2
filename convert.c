#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>


 void div_convert (uint32_t n, int base, char *out) {
    char temp[65];
    int pos = 0;

    if (n == 0) {
        strcpy(out, "0");
        return;

    }

    while (n > 0) {
        int remainder = n % base;
        n = n / base;

        if (remainder < 10 )
            temp[pos++] = '0' + remainder;
        else
            temp[pos++] = 'A' + (remainder - 10);
    }
    // Reverse the result
    temp[pos] = '\0';
    int len = pos;
    for (int i = 0; i < len; i++) {
        out[i] = temp[len - 1 - i];
    }
    out[len] = '\0';

 }

 void sub_convert(uint32_t n, int base, char *out) {
    char temp[65];
    int pos = 0;

    if (n==0) {
        strcpy(out, "0");
        return;
    }

    uint32_t power = 1;
    while (power <= n / base) {
        power *= base;
    }

    while (power > 0) {
        int digit = n / power;
        n -= digit * power;

        if (digit < 10)
            temp[pos++] = '0' + digit;
        else
            temp[pos++] = 'A' + (digit - 10);
        
        power /= base;
    }

    temp[pos] = '\0';
    strcpy(out, temp);
 }

 void print_tables(uint32_t n) {
    char bin[33], oct[12], hex[9];

    div_convert(n, 2, bin);
    div_convert(n, 8, oct);
    div_convert(n, 16, hex);
    printf("Original: Binary=%s Octal=%s Decimal=%u Hex=%s\n",
            bin, oct, n, hex);
    
    uint32_t shifted = n << 3;
    div_convert(shifted, 2, bin);
    div_convert(shifted, 8, oct);
    div_convert(shifted, 16, hex);
    printf("Left shift by 3: Binary=%s Octal=%s Decimal=%u Hex=%s\n",
            bin, oct, shifted, hex);

    uint32_t masked = n & 0xFF;
    div_convert(masked, 2, bin);
    div_convert(masked, 8, oct);
    div_convert(masked, 16, hex);
    printf("And with 0xFF: Binary=%s Octal=%s Decimal=%u Hex=%s\n",
            bin, oct, masked, hex);

 }

// new editions

void to_32bit_binary(uint32_t value, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[ 31 - i] = (( value >> i)& 1) ? '1' : '0';
    }
    out[32] = '\0';
}

void oct_to_bin(const char *oct, char *out) {
    out[0] = '\0';
    for (int i = 0; oct[i] != '\0'; i++) {
        int digit = oct[i] - '0';
        char buf[4];
        buf[0] = ((digit >> 2) & 1) ? '1' : '0';
        buf[1] = ((digit >> 1) & 1) ? '1' :'0';
        buf[2] = (digit & 1) ? '1' : '0';
        buf[3] = '\0';
        strcat(out, buf);
    }
}

void oct_to_hex(const char *oct, char *out) {
  char bin[128] = "";
    oct_to_bin(oct, bin);

    int len = strlen(bin);
    int pad = (4 - (len % 4)) % 4;
    char padded[128] = "";
    for (int i = 0; i < pad; i++) strcat(padded, "0");
    strcat(padded, bin);

    int plen = strlen(padded);
    out[0] = '\0';
    int started = 0; 
    for (int i = 0; i < plen; i += 4) {
        int val = 0;
        for (int j = 0; j < 4; j++) {
            val = (val << 1) | (padded[i + j] - '0');
        }
        char hexchar = (val < 10) ? ('0' + val) : ('A' + (val - 10));

        if (hexchar != '0' || started || i == plen - 4) {
            int lenout = strlen(out);
            out[lenout] = hexchar;
            out[lenout + 1] = '\0';
            started = 1;
        }
    }
}

void hex_to_bin(const char *hex, char *out) {
    out[0] = '\0';
    for (int i = 0; hex[i] != '\0'; i++){
        char c = toupper(hex[i]);
        int val;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = 10 + (c - 'A');
        else continue;

        char buf[5];
        buf[0] = ((val >> 3) & 1) ? '1' : '0';
        buf[1] = ((val >> 2) & 1) ? '1' : '0';
        buf[2] = ((val >> 1) & 1) ? '1' : '0';
        buf[3] = (val & 1) ? '1' : '0';
        buf[4] = '\0';
        strcat(out, buf);
    }
}

void to_sign_magnitude(int32_t n, char *out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    } else {
        int32_t absval = -n;
        to_32bit_binary((uint32_t)absval, out);
        out[0] = '1';  // set sign bit
    }
}

void to_ones_complement(int32_t n, char *out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    } else {
        uint32_t absval = (uint32_t)(-n);
        char temp[33];
        to_32bit_binary(absval, temp);
        for (int i = 0; i < 32; i++) {
            out[i] = (temp[i] == '0') ? '1' : '0';
        }
        out[32] ='\0';
    }
}

void to_twos_complement(int32_t n, char *out) {
    uint32_t val = (uint32_t)n;
    to_32bit_binary(val, out);

}
