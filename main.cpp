#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
// for rand
#include <stdlib.h>
class Minesweeper : public olc::PixelGameEngine
{
public:
	Minesweeper(int rows, int columns)
	{
		sAppName = "uglysweeper";
		this->rows = rows;
		this->columns = columns;
	}
	
	Minesweeper()
	{
		rows = 16;
		columns = 10;
	}

// quick reference on the board layout
// 0 is a blank tile
// 9 is an uncovered safe tile
// 1 - 8 is an uncovered tile with a bomb next to it
// char for number, ie '1', '2', etc are tiles that have numbers but are uncovered
// 'f' is a flag, 'F' is a flag over a bomb
// 'b' is a covered bomb, 

public:
	bool OnUserCreate() override
	{
		sprCovered = std::make_unique<olc::Sprite>("./assets/covered.png");
		sprUncovered = std::make_unique<olc::Sprite>("./assets/uncovered.png");
		sprBomb = std::make_unique<olc::Sprite>("./assets/bomb.png");
		sprFlag = std::make_unique<olc::Sprite>("./assets/flag.png");
		spr1 = std::make_unique<olc::Sprite>("./assets/1.png");
		spr2 = std::make_unique<olc::Sprite>("./assets/2.png");
		spr3 = std::make_unique<olc::Sprite>("./assets/3.png");
		spr4 = std::make_unique<olc::Sprite>("./assets/4.png");
		spr5 = std::make_unique<olc::Sprite>("./assets/5.png");
		spr6 = std::make_unique<olc::Sprite>("./assets/6.png");
		spr7 = std::make_unique<olc::Sprite>("./assets/7.png");
		spr8 = std::make_unique<olc::Sprite>("./assets/8.png");
	
		decBomb = std::make_unique<olc::Decal>(sprBomb.get());
		decFlag = std::make_unique<olc::Decal>(sprFlag.get());

		for(int i = 0; i < rows * columns; i++) vectBoard.push_back(0); 
		
		// this block decides where to put down bombs
		// by using rand
		srand (time(NULL));
		int bombCount = 0;
			
		while(true)
		{
			int randx = rand() % columns;
			int randy = rand() % rows;
			
			if(bombCount == iBombs) break;
			if(vectBoard[randx * randy] != 'b')
			{
				vectBoard[randx * randy] = 'b';
				bombCount++;
			}
		}
		for(int x = 0; x < columns; x++)
		{
			for(int y = 0; y < rows; y++)
			{
				if(vectBoard[y * columns + x] != 'b')	
					vectBoard[y * columns + x] = getSurroundedBy(x, y);
			}
		}
		return true;
	}
	// search the array at a given tile, (x, y)
	// return the number of bombs it is surrounded by
	
	int getSurroundedBy(int x, int y)
	{
		int count = 0;
		for(int xLoop = x - 1; xLoop <= x + 1; xLoop++)
		{
			for(int yLoop = y - 1; yLoop <= y + 1; yLoop++)
			{
				if(yLoop == y && xLoop == x) 	{ /* don't count what's in (x,y)*/ }
				if(yLoop < 0 || xLoop < 0) 		{ /* so we don't segfault */ }
				if(yLoop > rows || xLoop > rows){ /* so we don't segfault */ }
				if(vectBoard[yLoop * columns + xLoop] == 'b')
					count++;
			}
		}
		return count;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		// user input
		if(GetMouse(0).bPressed)
		{
			// figures out what tile the player is on
			int x = GetMouseX() / 32;
			int y = GetMouseY() / 32;
			switch(vectBoard[y * columns + x])
			{
				case 0:
					// FUCK MY LIFE THIS IS A COMPLEX ALGORITHM
					floodRemove(x, y);
					break;
				case 1:
					vectBoard[y * columns + x] = '1';		
					break;
				case 2:
					vectBoard[y * columns + x] = '2';		
					break;
				case 3:
					vectBoard[y * columns + x] = '3';		
					break;
				case 4:
					vectBoard[y * columns + x] = '4';		
					break;
				case 5:
					vectBoard[y * columns + x] = '5';		
					break;
				case 6:
					vectBoard[y * columns + x] = '6';		
					break;
				case 7:
					vectBoard[y * columns + x] = '7';		
					break;
				case 8:
					vectBoard[y * columns + x] = '8';		
					break;
				case 'b': // game over!!
					return false;
				default:
					/* do nothing */
					break;
			}
		}

		if(GetMouse(1).bPressed)
		{
			int x = GetMouseX() / 32;
			int y = GetMouseY() / 32;
			if(vectBoard[y * columns + x] == 'b')
			{
				++iCoveredBombs;
				vectBoard[y * columns + x] = 'F';
			}
			else if(vectBoard[y * columns + x] == 'F')
				vectBoard[y * columns + x] = '0';
			else
				vectBoard[y * columns + x] = 'f';
		}
		for(int x = 0; x < columns; x++)
		{
			for(int y = 0; y < rows; y++)
			{
				switch(vectBoard[y * columns + x])
				{
					case 0: 
						DrawSprite(olc::vi2d(x, y) * 32, sprCovered.get());
						break;
					case '0': 
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						break;
					case 'b': case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
						DrawSprite(olc::vi2d(x, y) * 32, sprCovered.get());
						break;
					case '1':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr1.get());
						break;
					case '2':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr2.get());
						break;
					case '3':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr3.get());
						break;
					case '4':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr4.get());
						break;
					case '5':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr5.get());
						break;
					case '6':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr6.get());
						break;
					case '7':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr7.get());
						break;
					case '8':
						DrawSprite(olc::vi2d(x, y) * 32, sprUncovered.get());
						DrawSprite(olc::vi2d(x, y) * 32, spr8.get());
						break;
					case 'f': case 'F':
						DrawDecal(olc::vi2d(x, y) * 32, decFlag.get());
						break;
				}
			}
		}
		if(iCoveredBombs == iBombs) return false;
		
		return true;
	}
	// a cool implementation of a recursive flood fill algorithm
	// pretty sure that big chain of if else could be done with some
	// manipulation of the ascii table but whatever lol
	void floodRemove(int x, int y)
	{
		if(vectBoard[y * columns + x] == 0)
		{
			vectBoard[y * columns + x] = '0';
			if(x < columns)
			floodRemove(x + 1, y);
			if(y < rows)
			floodRemove(x, y + 1);
			if(x > 0)
			floodRemove(x - 1, y);
			if(y > 0)
			floodRemove(x, y - 1);
		}
		else if(vectBoard[y * columns + x] == 1)
			vectBoard[y * columns + x] = '1';
		else if(vectBoard[y * columns + x] == 2)
			vectBoard[y * columns + x] = '2';
		else if(vectBoard[y * columns + x] == 3)
			vectBoard[y * columns + x] = '3';
		else if(vectBoard[y * columns + x] == 4)
			vectBoard[y * columns + x] = '4';
		else if(vectBoard[y * columns + x] == 5)
			vectBoard[y * columns + x] = '5';
		else if(vectBoard[y * columns + x] == 6)
			vectBoard[y * columns + x] = '6';
		else if(vectBoard[y * columns + x] == 7)
			vectBoard[y * columns + x] = '7';
		else if(vectBoard[y * columns + x] == 8)
			vectBoard[y * columns + x] = '8';
	}
private:
	std::unique_ptr<olc::Sprite> sprCovered;
	std::unique_ptr<olc::Sprite> sprUncovered;
	std::unique_ptr<olc::Sprite> sprBomb;
	std::unique_ptr<olc::Sprite> sprFlag;
	std::unique_ptr<olc::Sprite> spr1;
	std::unique_ptr<olc::Sprite> spr2;
	std::unique_ptr<olc::Sprite> spr3;
	std::unique_ptr<olc::Sprite> spr4;
	std::unique_ptr<olc::Sprite> spr5;
	std::unique_ptr<olc::Sprite> spr6;
	std::unique_ptr<olc::Sprite> spr7;
	std::unique_ptr<olc::Sprite> spr8;
	
	std::unique_ptr<olc::Decal> decFlag;
	std::unique_ptr<olc::Decal> decBomb;

	int rows, columns, iBombs = 10, iCoveredBombs = 0;
	std::vector<int> vectBoard;
};
int main()
{
	int rows = 10, columns = 16;
	Minesweeper demo(rows, columns);
	if (demo.Construct(columns * 32, rows * 32, 1, 1, false, true))
		// pass rows and columns to it
		demo.Start();

	return 0;
}
