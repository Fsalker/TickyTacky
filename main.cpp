#include<iostream>
using namespace std;

const int BASE = 3; /// "SUPER" CONSTANT - DO NOT MODIFY
const int BOARD_WIDTH = 3;
const int BOARD_HEIGHT = 3;
const int SOLUTION_WIDTH = 3;
int MAX_HASH; /// "SEMI" CONSTANT - needs to be computed before it can be initialised (FIXME?)

struct state
{
    unsigned long long stateHash; /// I'd name this variable 'hash', but it would collide with the already existing 'hash' from <some-kind-of-library.h>
    int stateArray[BOARD_HEIGHT][BOARD_WIDTH]; /// FIXME: Change me into 'char', please (:

    void hash_to_array() /// Convert hash into a TicTacToe match
    {
        unsigned long long auxStateHash;
        int rem,x,y,arrIndex;

        auxStateHash = stateHash;
        for(arrIndex = 0; arrIndex < BOARD_WIDTH * BOARD_HEIGHT; ++arrIndex)
        {
            y = arrIndex / BOARD_HEIGHT;
            x = arrIndex % BOARD_HEIGHT;

            stateArray[y][x] = auxStateHash % BASE;

            auxStateHash /= BASE;
        }
    }

    void print() /// Currently, for debugging
    {
        cout<<"hash = "<<stateHash<<"\n";
        for(int y=0; y<BOARD_HEIGHT; ++y)
        {
            for(int x=0; x<BOARD_WIDTH; ++x)
            {
                char c;

                /// Yes, I do hate switch statements
                if(stateArray[y][x] == 0) c = ' ';
                else if(stateArray[y][x] == 1) c = 'X';
                else if(stateArray[y][x] == 2) c = 'O';
                else c = 'Error! Invalid state';
                cout<<c;
                if(x+1 < BOARD_WIDTH) cout<<"|";
            }
            cout<<"\n";
            if(y+1 < BOARD_HEIGHT)
            {
                for(int x=0; x<BOARD_WIDTH*2-1; ++x)
                    cout<<"_";
                cout<<"\n";
            }
        }
        cout<<"\n";
    }
};

state GLOBAL_stateArray[100000]; /// FIXME: I'm evil

void compute_solutions(int arrSol[], int boardLength, int boardHeight, int solutionLength)
{
    /// LEFT / UP-LEFT / UP / UP-RIGHT directions == horizontal / main diagonal / vertical / secondary diagonal directions
    int dir_x[] = {-1, -1, 0, 1};
    int dir_y[] = {0, -1, -1, -1};
    int x,y,dir_index,sol_index,newx,newy,sol_hash;

    for(y = solutionLength-1; y < boardHeight; ++y) /// These nested for loops always find solutions inside the game board (4th case is slightly more special)
        for(x = solutionLength-1; x < boardLength; ++x)
            for(dir_index = 0; dir_index < 4; ++dir_index) /// Calculate solution hashes for each 4 of the possible directions
            {
                if(dir_index == 3 && x >boardLength - solutionLength) continue; /// Ugly but effective: avoid out-of-bounds m Secondary diagonal solutions

                newy = y; /// We're going to use 'newy' and 'newx' in order to find the points belonging in the current solution
                newx = x;
                sol_hash = 0;
                for(sol_index = 0; sol_index < solutionLength; ++sol_index, newy += dir_y[dir_index], newx += dir_x[dir_index])
                {

                }

            }
}

unsigned long long pow(unsigned long long base, int exp) /// No need for a quick exponentiation
{
    if(exp == 0) return 1;
    return base * pow(base, exp-1);
}

unsigned long long compute_max_hash(int base, int boardLength, int boardHeight)
{
    return pow(base, boardLength * boardHeight); /// For some unproven reason, it works!
}

int point_is_inside_board(int y, int x)
{
    if(y<0) return 0;
    if(y>=BOARD_HEIGHT) return 0;
    if(x<0) return 0;
    if(x>=BOARD_WIDTH) return 0;
    return 1;
}

void compute_victory_state(state crtState)
{
    /// LEFT / UP-LEFT / UP / UP-RIGHT directions == horizontal / main diagonal / vertical / secondary diagonal directions
    int dir_x[] = {-1, -1, 0, 1};
    int dir_y[] = {0, -1, -1, -1};
    int DP[BOARD_HEIGHT][BOARD_WIDTH][4][2]; /// DP[Y][X][DIR][SYMBOL] = Longest subsequence of SYMBOL (0, 1 or 2 - note: 1=X, 2=O) on point (Y, X) from direction DIR
    int y,x,dirIndex,symbol, newy,newx;

    /*/// Initialization
    for(y=0; y<BOARD_HEIGHT; ++y)
        for(dirIndex=0; dirIndex<4; ++dirIndex)
            for(symbol=1; symbol<=2; ++symbol)
                DP[y][0][dirIndex][symbol-1] = (crtState.stateArray[y][0] == symbol); /// Does my current symbol equal to the symbol in the state? Yes = 1, No = 0

    for(x=0; x<BOARD_WIDTH; ++x)
        for(dirIndex=0; dirIndex<4; ++dirIndex)
            for(symbol=1; symbol<=2; ++symbol)
                DP[0][x][dirIndex][symbol-1] = (crtState.stateArray[0][x] == symbol); /// Same as above*/

    /// Recursion
    for(y=0; y<BOARD_HEIGHT; ++y)
        for(x=0; x<BOARD_WIDTH; ++x)
            for(dirIndex=0; dirIndex<4; ++dirIndex)
            {
                newy = y + dir_y[dirIndex];
                newx = x + dir_x[dirIndex];
                for(symbol=1; symbol<=2; ++symbol)
                {
                    if(!point_is_inside_board(newy, newx))
                        DP[y][x][dirIndex][symbol-1] = (crtState.stateArray[y][x] == symbol);
                    else
                    {
                        if(crtState.stateArray[y][x] == symbol)
                        {
                            if(crtState.stateArray[newy][newx] == symbol)
                            {
                                DP[y][x][dirIndex][symbol-1] = DP[newy][newx][dirIndex][symbol-1] + 1;
                            }
                            else DP[y][x][dirIndex][symbol-1] = 1;
                        }
                        else DP[y][x][dirIndex][symbol-1] = 0;
                    }
                }
            }

    /// Debugging
    {
        for(dirIndex=0; dirIndex<4; ++dirIndex)
        {
            for(y=0; y<BOARD_HEIGHT; ++y)
            {
                for(x=0; x<BOARD_WIDTH; ++x) cout<<DP[y][x][dirIndex][symbol-1]<<" ";
                cout<<"\n";
            }
            cout<<"\n";
        }
        cout<<"\n";
}

void compute_states(int boardLength, int boardHeight, state lastState) /// Top-to-bottom dynamic programming
{
    state crtState;

    for(int crtStateHash = lastState.stateHash; crtStateHash >= lastState.stateHash; --crtStateHash) // lastState.stateHash
    {
        crtStateHash = 6721;
        crtState.stateHash = crtStateHash;
        crtState.hash_to_array();
        crtState.print();

        compute_victory_state(crtState);
        break;
    }
}

int main()
{
    MAX_HASH = compute_max_hash(BASE, BOARD_WIDTH, BOARD_HEIGHT) - 1;
    state lastState;
    lastState.stateHash = MAX_HASH;
    lastState.hash_to_array();
    ///lastState.print();

    compute_states(BOARD_WIDTH, BOARD_HEIGHT, lastState);

    return 0;
}
