#include "state.h"


int main()
{
    int n = 21;
    pState S[21][21];
    random_state(21, S);
    Value_iteration(S, n, 0.9);
    print(S, n);
    return 0;
}