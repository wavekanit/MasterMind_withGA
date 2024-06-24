
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define guessSize 5
#define choice 8
#define MIN 1
#define MAX 8

#define CROSSOVER_METHOD 1
// 1 = Unicrossover
// 2 = Singlecrossover
// 3 = Doublecrossover

#define MUTATION_METHOD 2
// 1 = RandomMutate
// 2 = InversionMutate
// 3 = ScrambleMutate
// 4 = SwapMutation

#define POPULATION_SIZE 150
#define GENERATION_SIZE 100
#define CROSSOVER_PROB 0.5
#define PERMUTATION_PROB 0.03
#define CROSSOVER_THEN_MUTATE_PROB 0.3

typedef struct E_T
{
    int guess[guessSize];
    int score[guessSize];
    int red;
    int white;
    int fitnessValue;
} E_T;

void genSecret(int *secretCode)
{
    for (int i = 0; i < guessSize; i++)
    {
        secretCode[i] = (int)(rand()) % choice + 1;
    }
}

int generateRandom(int min, int max)
{
    int range = max - min + 1;
    int randomNumber = rand() % range + min;
    return randomNumber;
}

void checkRed(int secretCode[], E_T *guess)
{
    for (int i = 0; i < guessSize; i++)
    {
        if (secretCode[i] == guess->guess[i])
        {
            guess->score[i] = 5;
            secretCode[i] = -1;
        }
    }
}

void checkWhite(int secretCode[], E_T *guess)
{
    for (int i = 0; i < guessSize; i++)
    {
        if (guess->score[i] == 5)
        {
            continue;
        }
        for (int j = 0; j < guessSize; j++)
        {
            if (secretCode[j] == guess->guess[i] && guess->score[i] == 0)
            {
                guess->score[i] = 3;
                secretCode[j] = -1;
            }
        }
    }
}

void checkAns(int secretCode[], E_T *guess, int display)
{
    int secretCopy[guessSize];
    for (int i = 0; i < guessSize; i++)
    {
        secretCopy[i] = secretCode[i];
    }
    checkRed(secretCopy, guess);
    checkWhite(secretCopy, guess);
    int redScore = 0;
    int whiteScore = 0;
    for (int i = 0; i < guessSize; i++)
    {
        if (guess->score[i] == 5)
        {
            redScore++;
        }
        else if (guess->score[i] == 3)
        {
            whiteScore++;
        }
    }
    guess->red = redScore;
    guess->white = whiteScore;
    if (display == 1)
    {
        printf("red : %d | white : %d\n", guess->red, guess->white);
    }

    for (int i = 0; i < guessSize; i++)
    {
        guess->score[i] = 0;
    }
    // printf("score : ");
    // for(int i=0;i<guessSize;i++) {
    //   printf("%d ",guess->score[i]);
    // } printf("\n");
}

int insertE_T(E_T **node, int guess[], int size)
{
    E_T *temp = (E_T *)realloc(*node, sizeof(E_T) * (size + 1));
    *node = temp;
    for (int i = 0; i < guessSize; i++)
    {
        (*node)[size].guess[i] = guess[i];
        (*node)[size].score[i] = 0;
    }
    (*node)[size].red = 0;
    (*node)[size].white = 0;
    (*node)[size].fitnessValue = 0;

    return size + 1;
}

E_T crossover(E_T code1, E_T code2)
{
    E_T child;
    if (CROSSOVER_METHOD == 1) // single
    {
        for (int i = 0; i < guessSize; i++)
        {
            float prob = (float)(generateRandom(0, 100)) / 100;
            if (prob < (CROSSOVER_PROB))
            {
                // printf("%lf\n", prob);
                child.guess[i] = code1.guess[i];
            }
            else
            {
                child.guess[i] = code2.guess[i];
            }
        }
    }
    else if (CROSSOVER_METHOD == 2)
    {
        int point = (int)(rand()) % (guessSize - 1) + 1;
        for (int i = 0; i < guessSize; i++)
        {
            if (i < point)
            {
                child.guess[i] = code1.guess[i];
            }
            else
            {
                child.guess[i] = code2.guess[i];
            }
        }
    }
    else if (CROSSOVER_METHOD == 3)
    {
        int point1 = (int)(rand()) % (guessSize - 1) + 1;
        int point2 = (int)(rand()) % (guessSize - 1) + 1;
        if (point2 < point1)
        {
            int temp = point1;
            point1 = point2;
            point2 = temp;
        }
        for (int i = 0; i < guessSize; i++)
        {
            if (i < point1 || i > point2)
            {
                child.guess[i] = code1.guess[i];
            }
            else
            {
                child.guess[i] = code2.guess[i];
            }
        }
    }
    return child;
}

void shuffle(int *array, size_t n, int (*rand_func)(void))
{
    for (int i = n - 1; i >= 0; i--)
    {
        int j = rand_func() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

E_T Mutate(E_T code)
{
    E_T child;
    if (MUTATION_METHOD == 1)
    {
        int randIndex = (int)(rand()) % (guessSize - 1) + 1;
        int randNum = (int)(rand()) % (choice) + 1;
        for (int i = 0; i < guessSize; i++)
        {
            child.guess[i] = code.guess[i];
        }
        child.guess[randIndex] = randNum;
    }
    else if (MUTATION_METHOD == 2)
    {
        int point1 = (int)(rand()) % (guessSize - 1) + 1;
        int point2 = (int)(rand()) % (guessSize - 1) + 1;
        while (point2 == point1)
        {
            point2 = (int)(rand()) % (guessSize - 1);
        }
        if (point2 < point1)
        {
            int temp = point1;
            point1 = point2;
            point2 = temp;
        }
        for (int i = 0; i < guessSize; i++)
        {
            if (i < point1 || i > point2)
            {
                child.guess[i] = code.guess[i];
            }
            else
            {
                child.guess[i] = code.guess[point1 + point2 - i];
            }
        }
    }
    else if (MUTATION_METHOD == 3)
    {
        int point1 = (int)(rand()) % (guessSize - 1);
        int point2 = (int)(rand()) % (guessSize - 1);
        while (point2 == point1)
        {
            point2 = (int)(rand()) % (guessSize - 1);
        }

        if (point2 < point1)
        {
            int temp = point1;
            point1 = point2;
            point2 = temp;
        }

        for (int i = 0; i < guessSize; i++)
        {
            if (i < point1 || i > point2)
            {
                child.guess[i] = code.guess[i];
            }
        }

        int size = point2 - point1 + 1;
        int tempArr[size];
        for (int i = 0; i < size; i++)
        {
            tempArr[i] = code.guess[point1 + i];
        }

        shuffle(tempArr, size, rand);

        for (int i = 0; i < size; i++)
        {
            child.guess[point1 + i] = tempArr[i];
        }
    }
    else if (MUTATION_METHOD == 4)
    {

        int point1 = (int)(rand()) % (guessSize - 1);
        int point2 = (int)(rand()) % (guessSize - 1);
        while (point2 == point1)
        {
            point2 = (int)(rand()) % (guessSize - 1);
        }
        for (int i = 0; i < guessSize; i++)
        {
            if (i == point1)
            {
                child.guess[i] = code.guess[point2];
            }
            else if (i == point2)
            {
                child.guess[i] = code.guess[point1];
            }
            else
            {
                child.guess[i] = code.guess[i];
            }
        }
    }
    return child;
}

E_T permute(E_T code)
{
    E_T child = code;
    for (int i = 0; i < guessSize; i++)
    {
        if ((float)(generateRandom(0, 100)) / 100 < PERMUTATION_PROB)
        {
            int randIdxA = (int)(rand()) % (guessSize - 1) + 1;
            int randIdxB = (int)(rand()) % (guessSize - 1) + 1;
            int temp = child.guess[randIdxA];
            child.guess[randIdxA] = child.guess[randIdxB];
            child.guess[randIdxB] = temp;
        }
    }
    return child;
}

int isIn(E_T *population, int guess[], int size)
{
    for (int i = 0; i < size; i++)
    {
        int count = 0;
        for (int j = 0; j < guessSize; j++)
        {
            if (population[i].guess[j] == guess[j])
            {
                count++;
            }
        }
        if (count == guessSize)
        {
            return 1;
        }
    }
    return 0;
}

E_T randPopulation(E_T *population, int size)
{
    E_T child;
    int guess[guessSize];
    for (int i = 0; i < guessSize; i++)
    {
        guess[i] = generateRandom(MIN, MAX);
    }
    while (isIn(population, guess, size))
    {
        for (int i = 0; i < guessSize; i++)
        {
            guess[i] = generateRandom(MIN, MAX);
        }
    }
    for (int i = 0; i < guessSize; i++)
    {
        child.guess[i] = guess[i];
    }
    return child;
}

E_T fitnessvalue(E_T *trail, E_T *guess, int size)
{
    E_T newTrail = *trail;
    int red = 0;
    int white = 0;
    for (int i = 0; i < size; i++)
    {
        checkAns(guess[i].guess, trail, 0);
        red += abs(trail->red - guess[i].red);
        white += abs(trail->white - guess[i].white);
        for (int i = 0; i < guessSize; i++)
        {
            trail->score[i] = 0;
        }
    }
    trail->fitnessValue = red + white;
    newTrail.fitnessValue = trail->fitnessValue;
    return *trail;
}

E_T genetic_evolution(E_T *guess, int size)
{
    E_T *population = (E_T *)malloc(sizeof(E_T) * POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        population[i] = randPopulation(population, i);
    }
    int size_population = POPULATION_SIZE;
    E_T *choosen_one = (E_T *)malloc(sizeof(E_T));
    int size_choosen_one = 0;
    int gen = 1;

    while ((int)sizeof(choosen_one) / sizeof(E_T) <= POPULATION_SIZE &&
           gen <= GENERATION_SIZE)
    {
        E_T *sons = (E_T *)malloc(sizeof(E_T));
        int size_sons = 0;

        for (int i = 0; i < size_population; i++)
        {
            if (i == size_population - 1)
            {
                size_sons = insertE_T(&sons, population[i].guess, size_sons);
                break;
            }

            E_T son = crossover(population[i], population[i + 1]);

            if ((float)(generateRandom(0, 100)) / 100 < CROSSOVER_THEN_MUTATE_PROB)
            {
                son = Mutate(son);
            }

            son = permute(son);
            //  for(int i = 0; i < guessSize; i++){
            //     printf("%d ", son.guess[i]);
            // }
            // printf("\n");

            size_sons = insertE_T(&sons, son.guess, size_sons);
        }

        E_T *pop_score = (E_T *)malloc(sizeof(E_T));
        int size_pop_score = 0;
        for (int i = 0; i < size_sons; i++)
        {
            E_T temp = fitnessvalue(&sons[i], guess, size);
            size_pop_score = insertE_T(&pop_score, temp.guess, size_pop_score);
            pop_score[size_pop_score - 1].fitnessValue = temp.fitnessValue;
            // printf("fitness value : %d\n", pop_score[size_pop_score -
            // 1].fitnessValue);
        }
        E_T *eligibles = (E_T *)malloc(sizeof(E_T));
        int size_eligibles = 0;
        for (int i = 0; i < size_pop_score; i++)
        {
            if (pop_score[i].fitnessValue == 0 &&
                isIn(eligibles, pop_score[i].guess, size_eligibles) == 0)
            {
                size_eligibles =
                    insertE_T(&eligibles, pop_score[i].guess, size_eligibles);
            }
        }
        // printf("Eligibles : %d\n", size_eligibles);

        // for(int i = 0; i < size_eligibles; i++){
        //     for(int j = 0; j < guessSize; j++){
        //         printf("%d ", eligibles[i].guess[j]);
        //     }
        //     printf(": %d", eligibles[i].fitnessValue);
        //     printf("\n");
        // }

        if (size_eligibles > 0)
        {
            for (int i = 0; i < size_eligibles; i++)
            {
                if (isIn(choosen_one, eligibles[i].guess, size_choosen_one) == 0)
                {
                    size_choosen_one =
                        insertE_T(&choosen_one, eligibles[i].guess, size_choosen_one);
                }
            }
        }
        else
        {
            gen++;
            continue;
        }

        // printf("Choosen one : %d\n", size_choosen_one);
        // for(int i = 0; i < size_choosen_one; i++){
        //     for(int j = 0; j < guessSize; j++){
        //         printf("%d ", choosen_one[i].guess[j]);
        //     }
        //     printf(": %d", choosen_one[i].fitnessValue);
        //     printf("\n");
        // }
        // printf("choosen one : %d\n", size_choosen_one);
        if (size_choosen_one >= POPULATION_SIZE)
        {
            // printf("break\n");
            break;
        }

        for (int i = 0; i < size_choosen_one; i++)
        {
            for (int j = 0; j < guessSize; j++)
            {
                population[i].guess[j] = choosen_one[i].guess[j];
            }
        }
        int j = size_choosen_one;
        while (j < POPULATION_SIZE)
        {
            population[j] = randPopulation(population, size_choosen_one);
            j++;
        }
        gen++;
    }

    return choosen_one[size_choosen_one - 1];
}

int checkWrongGen(int *arr)
{
    int check = 0;
    for (int i = 0; i < guessSize; i++)
    {
        if (arr[i] <= MAX && arr[i] >= MIN)
        {
            check++;
        }
    }
    if (check == guessSize)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    srand(time(NULL));
    int secretCode[guessSize];
    genSecret(secretCode);
    printf("Secret Code : ");
    for (int i = 0; i < guessSize; i++)
    {
        printf("%d ", secretCode[i]);
    }
    printf("\n");
    // int g1[] = {1, 1, 2, 2};
    int g1[] = {1, 1, 2, 2, 3};
    int roundGuess = 0;
    E_T *guesses = (E_T *)malloc(sizeof(E_T));
    roundGuess = insertE_T(&guesses, g1, roundGuess);
    printf("round %d guess : ", roundGuess);
    for (int i = 0; i < guessSize; i++)
    {
        printf("%d ", guesses[0].guess[i]);
    }
    printf("\n");
    checkAns(secretCode, &guesses[roundGuess - 1], 1);

    while (guesses[roundGuess - 1].red != guessSize)
    {
        E_T toguess = genetic_evolution(guesses, roundGuess);
        if (!isIn(guesses, toguess.guess, roundGuess) && checkWrongGen(toguess.guess))
        {
            printf("round %d guess : ", roundGuess + 1);
            for (int i = 0; i < guessSize; i++)
            {
                printf("%d ", toguess.guess[i]);
            }
            printf("\n");
            roundGuess = insertE_T(&guesses, toguess.guess, roundGuess);
            checkAns(secretCode, &guesses[roundGuess - 1], 1);
        }
    }
    return 0;
}