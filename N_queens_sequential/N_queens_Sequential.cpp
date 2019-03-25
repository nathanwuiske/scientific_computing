#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;

void print(int *board, int board_size)
{
    for (int i=0;i<board_size;i++)
    {
        for (int j=0;j<board_size;j++)
        {
            if (j==board[i])
            {
                cout<<"("<<board[i]<<","<<board[j]<<")"<<" ";
            }
        }
    }
    cout<<endl;
}

int checkAttack(int *board, int board_size)
{
    int h = 0; //score
    for (int i=0;i<board_size-1;i++)
    {
        for (int j=i+1;j<board_size;j++)
        {
            int row_i = board[i];
            int row_j = board[j];

            if (row_i == row_j || // check vertical attacks
                row_i - row_j == i - j || //check diagonals
                row_i - row_j ==  j - i)
            {
                h += 1; //queen is threatened, increase the score
            }
        }
    }
    return h;
}

int* hillClimbing(int *board, int board_size)
{
    int h_to_beat = checkAttack(board, board_size);
    int *board_Outter;
    board_Outter = new int [board_size];

    for (int i=0;i<board_size;i++)
    {
            board_Outter[i] = board[i];
    }
    for (int i=0;i<board_size;i++)
    {
        for (int k=0;k<board_size;k++)
        {
            board_Outter[i] = k; //move the queen to the new row
            int new_h_cost = checkAttack(board_Outter, board_size);

            if (new_h_cost < h_to_beat) // return the first better match found
            {
               board[i] = k;
            }
        }
    return board;
    }
}

bool next_state(int *board, int board_size)
{
    int eval = checkAttack(board, board_size);
    int *temp;
    temp = hillClimbing(board, board_size);
    if (checkAttack(temp, board_size)<eval) //if hillclimb heuristic is less, new board is equal to hillclimb solution
    {
        for (int i=0;i<board_size;i++)
        {
            board[i] = temp[i];
        }
        return  true;
    }
    return false; //if hillclimb has a larger heuristic
}

void randomGameState(int *board, int board_size)
{
    for (int i = 0; i < board_size; i++)
    {
        board[i] = rand() % board_size; //randomly input queens on each row
    }
}

void solve(int board_size)
{
    int *board;
    int *hillClimbing_temp;
    board = new int[board_size]; // dynamically allocates space of size board_size
    randomGameState(board, board_size); //generate random board state
    while (checkAttack(board, board_size) != 0) //while h != 0, continue
    {
        if (!next_state(board, board_size)) //if false, reset the state
        {
            randomGameState(board, board_size);
        }
    }
        cout<<endl<<"n = "<<board_size<<"\tHill Climbing Solution:"<<endl;
        hillClimbing_temp = hillClimbing(board, board_size);
        print(hillClimbing_temp, board_size);
}

int main()
{
    srand(time(NULL));
    int size_Of_Board;
    cout<<"Enter n: ";
    cin>>size_Of_Board;
    if(size_Of_Board<4)
    {
        cout<<"Board size needs to be greater than 3"<<endl;
    }
    else
    {
     solve(size_Of_Board);
    }
    return 0;
}
