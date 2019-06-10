#include "state.h"


void random_state(int n, pState S[21][21])
{
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            S[i][j] = (pState)malloc(sizeof(struct State));
            S[i][j]->v = 0;
            S[i][j]->nS1 = i;
            S[i][j]->nS2 = j;
            S[i][j]->action_value = 0;
        }
    }
}

void print(pState S[21][21], int n)
{
    FILE *fp = fopen("result10.txt", "w+");
    fprintf(fp, "\t");
    printf("\t");
    for(int i = 0; i < n; i++)
    {
        printf("%d\t", i);
        fprintf(fp, "%d\t", i);
    }
    printf("\n");
    fprintf(fp, "\n");
    for(int i = 0; i < n; i++)
    {
        printf("%d\t", i);
        fprintf(fp, "%d\t", i);
        for(int j = 0; j < n; j++)
        {
            printf("%.2f, %d\t", S[i][j]->v, S[i][j]->action_value);
            fprintf(fp, "%.2f, %d\t", S[i][j]->v, S[i][j]->action_value);
        }
        printf("\n");
        fprintf(fp, "\n");
    }
    fclose(fp);
}

float Poisson(int n, int lambda)
{
    if(n >= 0)
    {
        float fac = 1.0;
        int i = 1;
        while(i <= n) {fac = fac * i; i++;}
        float p = pow(lambda, n)/fac*exp(-lambda);
        return p; 
    }
    return 0;
}

float* prob_Slot(int start, int end, int rent_lam, int return_lam)
{
    float* probs = (float*)malloc((start + 2)*sizeof(float));//trick
    float rental_prob = 0;
    float return_prob = 0;
    for(int rented = 0; rented <= start; rented++)
    {
        int returned = end - start + rented;
        
        if(returned >= 0)//feasible : prob != 0
        {
            //rental probability
            if(rented != start)
            {
                rental_prob = Poisson(rented, rent_lam);
            }
            else//ensure sum of probability = 1
            {
                rental_prob = 1;
                for(int temp = 0; temp < start; temp++)
                    rental_prob -= Poisson(temp, rent_lam);
            }

            //returning probability
            if(end != 20)
            {
                return_prob = Poisson(returned, rent_lam);
            }
            else//ensure sum of probability = 1
            {
                return_prob = 1;
                for(int temp = 1; temp < returned; temp++)
                    return_prob -= Poisson(temp, rent_lam);
            }
            probs[rented] = return_prob*rental_prob;
        }
        else
        {
            probs[rented] = 0;
        }
    }
    return probs;
}

int policy(int start1, int start2, int move)
{
    if(start1 + move < 0)
        return 0;
    if(start2 - move > 20)
        return 0;
    return 1;
}

float* transition_prob(pState start, pState end, int move)//P(r, s' | s, a)
{
    float* prob_reward = (float*)malloc(2*sizeof(float));
   
    if(policy(start->nS1, start->nS2, move) * policy(start->nS2, start->nS1, -move) == 0)
    {//infeasible
        prob_reward[0] = 0;
        prob_reward[1] = 0;
        return prob_reward;
    }
     int startS1 = start->nS1 + move;
    int startS2 = start->nS2 - move;
    int endS1 = end->nS1;
    int endS2 = end->nS2;
    
    // if(startS1 > 20) startS1 = 20;//move with same payment but restricted
    // if(startS2 > 20) startS2 = 20;
    
    float* probs1 = prob_Slot(startS1, endS1, 3, 3);
    float* probs2 = prob_Slot(startS2, endS2, 4, 2);
    float prob = 0;
    float expected_reward = 0;
    for(int rented1 = 0; rented1<=startS1; rented1++)
    {
        for(int rented2 = 0; rented2<=startS2; rented2++)
        {
            prob += probs1[rented1]*probs2[rented2];
            expected_reward += (rented1 + rented2)*10*probs1[rented1]*probs2[rented2];
        }
    }
    prob_reward[0] = prob;
    prob_reward[1] = expected_reward;
    free(probs1); free(probs2);
    return prob_reward;
}

void Value_iteration(pState S[21][21], int n, float gamma)
{
    int action[11] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    //estimate optimal value
    int iters = 0;
    while(1)
    {
        float delta = 0;
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                //estimate value
                float Sa[11];
                for(int a = 0; a < 11; a++)
                {
                    int move = action[a];
                    Sa[a] = -2*abs(move)*policy(i, j, move)*policy(j, i, -move);
                    for(int h = 0; h < n; h++)
                    {
                        for(int k = 0; k < n; k++)
                        {
                            float* prob_reward = transition_prob(S[i][j], S[h][k], move);
                            Sa[a] += (prob_reward[1] + prob_reward[0]*gamma*S[h][k]->v);
                            free(prob_reward);
                        }
                    }
                }
                //improve policy
                float V = S[i][j]->v;
                float max = Sa[0];
                for(int a = 0; a< 11; a++)
                {
                    if(Sa[a] >= max)
                    {
                        S[i][j]->v = Sa[a];
                        S[i][j]->action_value = action[a];
                        max = Sa[a];
                    }
                }
                //terminate
                float error = abs(V - S[i][j]->v);
                if(delta < error) delta = error;   
            }
        }
        iters++;
        if(delta < 1 || iters == 10)
            return;
    }
}
