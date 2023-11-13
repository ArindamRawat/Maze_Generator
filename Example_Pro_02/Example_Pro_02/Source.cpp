#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include "olcConsoleGameEngine.h"

using namespace std;

class MazeGenerator : public olcConsoleGameEngine
{
public:
    MazeGenerator()
    {
        m_sAppName = L"Maze Generator";
    }

private:
    int nMazeWidth;
    int nMazeHeight;
    vector<int> maze;
    stack<pair<int, int>> stack;
    int dx[4] = { 2, 0, -2, 0 };
    int dy[4] = { 0, 2, 0, -2 };

protected:
    virtual bool OnUserCreate()
    {
        nMazeWidth = ScreenWidth() / 2;
        nMazeHeight = ScreenHeight() / 2;
        maze = vector<int>(nMazeWidth * nMazeHeight, 0);
        stack.push({ 1, 1 });

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime)
    {
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_HALF);

        // Generate maze using depth-first search algorithm
        while (!stack.empty())
        {
            int x = stack.top().first;
            int y = stack.top().second;
            maze[y * nMazeWidth + x] = 1;

            vector<int> neighbors;

            // Check for unvisited neighbors
            for (int i = 0; i < 4; i++)
            {
                int nextX = x + dx[i];
                int nextY = y + dy[i];

                if (nextX >= 0 && nextX < nMazeWidth && nextY >= 0 && nextY < nMazeHeight)
                {
                    if (maze[nextY * nMazeWidth + nextX] == 0)
                        neighbors.push_back(i);
                }
            }

            if (!neighbors.empty())
            {
                // Randomly select a neighbor
                int nextDir = neighbors[rand() % neighbors.size()];

                int nextX = x + dx[nextDir];
                int nextY = y + dy[nextDir];

                // Remove the wall between the current cell and the chosen neighbor
                maze[nextY * nMazeWidth + nextX] = 1;

                // Move to the neighbor
                stack.push({ nextX, nextY });
            }
            else
            {
                // No unvisited neighbors, backtrack
                stack.pop();
            }
        }

        // Draw the maze
        for (int x = 0; x < nMazeWidth; x++)
        {
            for (int y = 0; y < nMazeHeight; y++)
            {
                if (maze[y * nMazeWidth + x] == 1)
                    Draw(x * 2, y * 2, PIXEL_SOLID, FG_WHITE);
            }
        }

        return true;
    }
};

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    MazeGenerator mazeGen;
    mazeGen.ConstructConsole(80, 50, 12, 12);
    mazeGen.Start();

    return 0;
}
