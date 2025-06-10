#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#define M_PI 3.14159265358979323846

void find_roots(double *coeff, int degree, double complex *roots) {
    
    double a = coeff[0];
    double b = coeff[1];
    double c = coeff[2];
    
    if (degree == 2) {
        double discriminant = b*b - 4*a*c;
        if (discriminant >= 0) {
            roots[0] = (-b + sqrt(discriminant))/(2*a);
            roots[1] = (-b - sqrt(discriminant))/(2*a);
        } else {
            roots[0] = -b/(2*a) + I*sqrt(-discriminant)/(2*a);
            roots[1] = -b/(2*a) - I*sqrt(-discriminant)/(2*a);
        }
    }
}

int main() {
    int num_degree, den_degree, i;
    double *num_coeff, *den_coeff;
    double w, w_start = 0.1, w_end = 100, w_step = 0.1;

    printf("Numerator degree: ");
    scanf("%d", &num_degree);
    num_coeff = (double*)malloc((num_degree+1) * sizeof(double));
    printf("Numerator coefficients (higher to lower degree):\n");
    for (i = 0; i <= num_degree; i++) {
        printf("b%d: ", num_degree - i);
        scanf("%lf", &num_coeff[i]);
    }

    printf("Denumerator degree: ");
    scanf("%d", &den_degree);
    den_coeff = (double*)malloc((den_degree+1) * sizeof(double));
    printf("Denumerator coefficients (yhigher to lower degree):\n");
    for (i = 0; i <= den_degree; i++) {
        printf("a%d: ", den_degree - i);
        scanf("%lf", &den_coeff[i]);
    }

    FILE *fp_bode = fopen("bode_data.csv", "w");
    if (!fp_bode) {
        printf("Not available!\n");
        return 1;
    }
    fprintf(fp_bode, "w,Magnitude(dB),Phase(degree)\n");
    printf("\nw\tMagnitude(dB)\tPhase(degree)\n");
    
    for (w = w_start; w <= w_end; w += w_step) {
        double num_real = 0, num_imag = 0;
        double den_real = 0, den_imag = 0;
        for (i = 0; i <= num_degree; i++) {
            int power = num_degree - i;
            double coeff = num_coeff[i];
            if (power == 0) {
                num_real += coeff;
            } else if (power % 2 == 0) {
                num_real += coeff * pow(w, power);
            } else {
                num_imag += coeff * pow(w, power);
            }
        }
        for (i = 0; i <= den_degree; i++) {
            int power = den_degree - i;
            double coeff = den_coeff[i];
            if (power == 0) {
                den_real += coeff;
            } else if (power % 2 == 0) {
                den_real += coeff * pow(w, power);
            } else {
                den_imag += coeff * pow(w, power);
            }
        }
        double num_abs = sqrt(num_real*num_real + num_imag*num_imag);
        double den_abs = sqrt(den_real*den_real + den_imag*den_imag);
        double magnitude = num_abs / den_abs;
        double phase = (atan2(num_imag, num_real) - atan2(den_imag, den_real)) * 180.0 / M_PI;
        printf("%.2f\t%.2f\t\t%.2f\n", w, 20*log10(magnitude), phase);
        fprintf(fp_bode, "%.2f,%.2f,%.2f\n", w, 20*log10(magnitude), phase);
    }
    fclose(fp_bode);

    FILE *fp_poles = fopen("poles_data.csv", "w");
    if (!fp_poles) {
        printf("Not available!\n");
        return 1;
    }
    fprintf(fp_poles, "real,imaginary\n");

    double complex *roots = (double complex*)malloc(den_degree * sizeof(double complex));
    find_roots(den_coeff, den_degree, roots);

    for (i = 0; i < den_degree; i++) {
        fprintf(fp_poles, "%.6f,%.6f\n", creal(roots[i]), cimag(roots[i]));
    }
    fclose(fp_poles);

    free(num_coeff);
    free(den_coeff);
    free(roots);
    return 0;
}
