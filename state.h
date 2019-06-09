#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct State
{
    int nS1;
    int nS2;
    float v;
    int action_value;
    //<0 : S1 -> S2, >0 : S2 -> S1, =0 : not change
};
typedef struct State* pState;

void random_state(int n, pState S[21][21]);
void print(pState S[21][21], int n);
float Poisson(int n, int lambda);
float* prob_Slot(int start, int end, int rent_lam, int return_lam);
void Value_iteration(pState S[21][21], int n, float gamma);