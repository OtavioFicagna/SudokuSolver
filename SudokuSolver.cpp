#include <iostream>

// Tamanho do SUDOKU dever ser 4 ou 9
const int N = 9;

// Elementos do SUDOKU (utilize 0 para o elemento vazio)
int sd[N][N] = {{3, 0, 0, 0, 0, 0, 0, 0, 0},
                {4, 7, 0, 3, 0, 9, 0, 0, 0},
                {0, 5, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 4, 1, 0, 0, 0, 0, 0},
                {9, 6, 0, 0, 7, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 2, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 6, 0, 7},
                {6, 0, 0, 2, 8, 7, 3, 0, 9},
                {7, 0, 9, 0, 0, 0, 0, 4, 0}};

// Função que verifica a possibilidade de atribuir um valor a uma linha e coluna
bool to_assign(int sd[N][N], int row, int col, int num)
{
    // Verificar a linha e a coluna
    for (int i = 0; i < N; i++)
    {
        if (sd[row][i] == num || sd[i][col] == num)
        {
            return false;
        }
    }

    // Verificar o bloco 3x3 (apenas para o SUDOKU 9x9)
    if (N == 9)
    {
        int first_row = row - row % 3;
        int first_col = col - col % 3;
        for (int i = first_row; i < first_row + 3; i++)
        {
            for (int j = first_col; j < first_col + 3; j++)
            {
                if (sd[i][j] == num)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

// Função principal que busca resolver o SUDOKU
bool solve(int sd[N][N])
{
    int row, col;
    bool empty = false;

    // Percorre a matriz procurando uma posição vazia ou seja = 0
    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
        {
            if (sd[row][col] == 0)
            {
                empty = true;
                break;
            }
        }
        if (empty)
        {
            break;
        }
    }

    // Se não existir mais nenhuma celula vazia o SUDOKU está resolvido
    if (!empty)
    {
        return true;
    }

    // Tentando achar qual valor pode ser atribuido para a posição vazia
    for (int i = 1; i <= N; i++)
    {
        if (to_assign(sd, row, col, i))
        {
            sd[row][col] = i;

            // Recursivamente tenta resolver o SUDOKU
            if (solve(sd))
            {
                return true;
            }

            // Se a atribuição não levou a uma solução válida, desfaça-a
            sd[row][col] = 0;
        }
    }
    return false; // Nenhuma atribuição levou a uma solução válida
}

void print(int sd[N][N])
{
    if (N == 9)
    {
        std::cout << "        SOLUTION:\n";
        for (int i = 0; i < N; i++)
        {
            if (i % 3 == 0 && i != 0)
            {
                std::cout << "* ";
                for (int j = 0; j < N; j++)
                {
                    if (j % 3 == 0 && j != 0)
                    {
                        std::cout << "| ";
                    }
                    std::cout << "- ";
                }
                std::cout << "*";
                std::cout << std::endl;
            }
            for (int j = 0; j < N; j++)
            {
                if (j % 3 == 0)
                {
                    std::cout << "| ";
                }
                if (sd[i][j] == 0)
                {
                    std::cout << "  ";
                }
                else
                {
                    std::cout << sd[i][j] << " ";
                }
            }
            std::cout << "|";
            std::cout << std::endl;
        }
    }
    if(N == 4)
    {
        std::cout << "SOLUTION:\n";
        for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    std::cout << sd[i][j] << " ";
                }
                std::cout << '\n';
        }
    }
}

int main()
{
    if (solve(sd))
    {
        print(sd);
    }
    else
    {
        std::cout << "Solution not found!\n";
    }
    return 0;
}