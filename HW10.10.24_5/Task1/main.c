#include <stdio.h>
#include <limits.h>
#include <stddef.h>
struct fp_s{
    int sign;
    int exponent;
    long int trailing;
};

struct fp32_s{
    unsigned char T_16_23 : 8;
    unsigned char T_8_15 : 8;
    unsigned char T_1_7 : 7;
    unsigned char E_8_8 : 1;
    unsigned char E_1_7 : 7;
    unsigned char s:1;
};

union fp32_u{
    float f;
    struct fp32_s s;
};

void fp32_to_struct(const float f, struct fp_s *ps){
    union fp32_u u;
    unsigned int bias = 127;

    u.f = f;
    //биты сохраняются в том же порядке, а байты уже шиворот на выворот
    ps -> sign = u.s.s;
    ps -> exponent = ((u.s.E_8_8) | (u.s.E_1_7 << 1)) - bias;
    ps -> trailing = ((u.s.T_1_7) | (u.s.T_8_15 << 8) | (u.s.T_16_23 << 16)); 
}
void struct_to_fp32(struct fp_s s, float *ps){
    union fp32_u u;
    unsigned int bias = 127;

    s.exponent = s.exponent + bias;

    u.s.s = s.sign;
    u.s.E_1_7 = ((s.exponent & 0xFF) >> 1);
    u.s.E_8_8 = (s.exponent & 1);
    u.s.T_16_23 = (s.trailing >> 16) & 0xFF;
    u.s.T_8_15 = (s.trailing >> 8) & 0xFF;
    u.s.T_1_7 = (s.trailing) & 0x7F;

    *ps = u.f;
}


int main() {
    float f;
    struct fp_s ps;

    scanf("%f", &f);
    printf("Число изначальное: %f\n", f);

    fp32_to_struct(f, &ps);
    printf("Число изначальное в виде структуры: S: %d, E: %d, T: %ld\n", ps.sign, ps.exponent, ps.trailing);

    struct_to_fp32(ps, &f);
    printf("После проебразования: %f\n", f);
    fp32_to_struct(f, &ps);
    printf("После преобразования: S: %d, E: %d, T: %ld\n", ps.sign, ps.exponent, ps.trailing);
    return 0;
}