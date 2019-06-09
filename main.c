#include "state.h"


int main()
{
    pState S[21][21];
    random_state(21, S);
    Value_iteration(S, 21, 0.9);
    print(S, 21);
    return 0;
}