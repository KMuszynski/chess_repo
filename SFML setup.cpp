
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <array>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;



enum Type
{
	BLANK,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	KING,
	QUEEN,
};
string types[7] = { "BLANK", "PAWN", "ROOK", "KNIGHT", "BISHOP", "KING", "QUEEN" };

enum Color
{
	WHITE,
	BLACK,
};
string colors[2] = { "WHITE","BLACK" };

struct Position
{
	int x;
	int y;
public:
	bool operator==(const Position& other) const
	{
		return x == other.x && y == other.y;
	}
};

struct Move
{
	Position from;
	Position to;
	Type type;
	Color color;

	bool operator==(const Move& other) const
	{
		return  to == other.to &&
			type == other.type && color == other.color;
	}
};


bool realPosition(Position pos);



class Piece
{
public:
	Color color;
	friend class Board;
protected:
	int posX;
	int posY;
public:
	Position p;
	Type type;
	char pieceChar;
	vector<Position> availableMoves;
	vector<Position> availableBeatings;
public:
	Piece(int xx, int yy, Color col)
	{
		posX = xx;
		posY = yy;
		p.x = xx;
		p.y = yy;
		color = col;
		pieceChar = '%';
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{

	}
};
class Pawn : public Piece
{
	friend class Board;
public:
	Pawn(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'p' : 'P';
		type = PAWN;
		fillAvailableMoves(*board);
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		if (color == WHITE)
		{
			Position pos;

			if (posY > 0)
			{
				pos.x = posX;
				pos.y = posY - 1;
				if (board[pos.x][pos.y]->type == BLANK)
				{
					availableMoves.push_back(pos);
				}
				if (posY == 6)
				{
					Position pos;
					pos.x = posX;
					pos.y = posY - 2;
					if (board[pos.x][pos.y]->type == BLANK)
					{
						availableMoves.push_back(pos);
					}
				}

				if (posX > 0)
				{
					Position pos;
					pos.x = posX - 1;
					pos.y = posY - 1;
					if (board[pos.x][pos.y]->type != BLANK && board[pos.x][pos.y]->color != this->color)
					{
						availableBeatings.push_back(pos);
					}
				}
				if (posX < 7)
				{
					Position pos;
					pos.x = posX + 1;
					pos.y = posY - 1;
					if (board[pos.x][pos.y]->type != BLANK && board[pos.x][pos.y]->color != this->color)
					{
						availableBeatings.push_back(pos);
					}
				}
			}
		}
		else if (color == BLACK)
		{
			Position pos;
			if (posY < 7)
			{
				pos.x = posX;
				pos.y = posY + 1;
				if (board[pos.x][pos.y]->type == BLANK)
				{
					availableMoves.push_back(pos);
				}
				if (posY == 1)
				{
					Position pos;
					pos.x = posX;
					pos.y = posY + 2;
					if (board[pos.x][pos.y]->type == BLANK)
					{
						availableMoves.push_back(pos);
					}
				}

				if (posX > 0)
				{
					Position pos;
					pos.x = posX - 1;
					pos.y = posY + 1;
					if (board[pos.x][pos.y]->type != BLANK && board[pos.x][pos.y]->color != this->color)
					{
						availableBeatings.push_back(pos);
					}
				}
				if (posX < 7)
				{
					Position pos;
					pos.x = posX + 1;
					pos.y = posY + 1;
					if (board[pos.x][pos.y]->type != BLANK && board[pos.x][pos.y]->color != this->color)
					{
						availableBeatings.push_back(pos);
					}
				}
			}
		}
	}
};
class King : public Piece
{
	friend class Board;
public:
	King(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'k' : 'K';
		type = KING;
		fillAvailableMoves(*board);
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		availableBeatings.clear();
		availableMoves.clear();
		/////
		Position pos;
		for (int i = posX - 1; i <= posX + 1; i++)
		{
			for (int j = posY - 1; j <= posY + 1; j++)
			{
				pos.x = i;
				pos.y = j;

				if (i < 0 || i>7 || j < 0 || j>7)
				{
					continue;
				}
				if (i == posX && j == posY)
				{
					continue;
				}
				if (board[i][j]->type == BLANK)
				{
					availableMoves.push_back(pos);
				}
				else if (board[i][j]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
		}
	}
};
class Queen : public Piece
{
	friend class Board;
public:
	Queen(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'q' : 'Q';
		type = QUEEN;
		fillAvailableMoves(*board);
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		for (int i = posX + 1; i < 8; i++)
		{
			Position pos;
			pos.x = i;
			pos.y = posY;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posX - 1; i >= 0; i--)
		{
			Position pos;
			pos.x = i;
			pos.y = posY;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posY + 1; i < 8; i++)
		{
			Position pos;
			pos.x = posX;
			pos.y = i;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posY - 1; i >= 0; i--)
		{
			Position pos;
			pos.x = posX;
			pos.y = i;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		// Right, up.
		for (int x = posX + 1, y = posY + 1; x < 8 && y < 8; x++, y++)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Right, down.
		for (int x = posX + 1, y = posY - 1; x < 8 && y >= 0; x++, y--)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Left, up.
		for (int x = posX - 1, y = posY + 1; x >= 0 && y < 8; x--, y++)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Left, down.
		for (int x = posX - 1, y = posY - 1; x >= 0 && y >= 0; x--, y--)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
	}
};
class Knight : public Piece
{
	friend class Board;
public:
	Knight(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'h' : 'H';
		type = KNIGHT;
		fillAvailableMoves(*board);
	}

	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		Position pos;
		pos.x = (posX - 1);
		pos.y = (posY - 2);
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}

		//Position pos;
		pos.x = posX + 1;
		pos.y = posY - 2;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		//Position pos2;
		pos.x = posX - 1;
		pos.y = posY + 2;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		//Position pos3;
		pos.x = posX + 1;
		pos.y = posY + 2;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}

		//Position pos4;
		pos.x = posX - 2;
		pos.y = posY - 1;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		//Position pos5;
		pos.x = posX + 2;
		pos.y = posY - 1;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		//Position pos6;
		pos.x = posX - 2;
		pos.y = posY + 1;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		//Position pos7;
		pos.x = posX + 2;
		pos.y = posY + 1;
		if (realPosition(pos))
		{
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}

		//availableBeatings = availableMoves;
	}
};
class Rook : public Piece
{
	friend class Board;
public:
	Rook(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'r' : 'R';
		type = ROOK;
		fillAvailableMoves(*board);
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		for (int i = posX + 1; i < 8; i++)
		{
			Position pos;
			pos.x = i;
			pos.y = posY;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posX - 1; i >= 0; i--)
		{
			Position pos;
			pos.x = i;
			pos.y = posY;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posY + 1; i < 8; i++)
		{
			Position pos;
			pos.x = posX;
			pos.y = i;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		for (int i = posY - 1; i >= 0; i--)
		{
			Position pos;
			pos.x = posX;
			pos.y = i;
			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			availableMoves.push_back(pos);
		}
		//availableBeatings = availableMoves;
	}
};
class Bishop : public Piece
{
	friend class Board;
public:
	Bishop(int xx, int yy, Color col, array<array<Piece*, 8>, 8>* board)
		:Piece(xx, yy, col)
	{
		pieceChar = (color == WHITE) ? 'b' : 'B';
		type = BISHOP;
		fillAvailableMoves(*board);
	}
	void fillAvailableMoves(array<array<Piece*, 8>, 8> board)
	{
		availableMoves.clear();
		availableBeatings.clear();
		////////////////////////////////////////////
		// Right, up.
		for (int x = posX + 1, y = posY + 1; x < 8 && y < 8; x++, y++)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Right, down.
		for (int x = posX + 1, y = posY - 1; x < 8 && y >= 0; x++, y--)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Left, up.
		for (int x = posX - 1, y = posY + 1; x >= 0 && y < 8; x--, y++)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}
		// Left, down.
		for (int x = posX - 1, y = posY - 1; x >= 0 && y >= 0; x--, y--)
		{
			Position pos;
			pos.x = x;
			pos.y = y;

			if (board[pos.x][pos.y]->type != BLANK)
			{
				if (board[pos.x][pos.y]->color != this->color)
				{
					availableBeatings.push_back(pos);
				}
				break;
			}
			else
			{
				availableMoves.push_back(pos);
			}
		}

	}
};
class Blank : public Piece
{
	friend class Board;
public:
	Blank(int xx, int yy, Color col)
		:Piece(xx, yy, col)
	{
		type = BLANK;
		pieceChar = '.';
	}
};

class Board
{
public:
	friend class Pawn;
	friend class Rook;
	friend class Bishop;
	friend class Queen;
	array<array<Piece*, 8>, 8> board;
public:
	void fillBoard()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Piece* b = new Blank(j, i, WHITE);
				board[j][i] = b;
			}
		}
	}
	void deleteBoard()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				delete board[j][i];
			}
		}
	}
	void addToBoard(Type type, int posX, int posY, Color col)
	{
		if (type == PAWN && (posY == 7 || posY == 0))
		{
			delete board[posX][posY];

			Piece* c = new Queen(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == PAWN)
		{
			delete board[posX][posY];

			Piece* c = new Pawn(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == KING)
		{
			delete board[posX][posY];

			Piece* c = new King(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == QUEEN)
		{
			delete board[posX][posY];

			Piece* c = new Queen(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == ROOK)
		{
			delete board[posX][posY];

			Piece* c = new Rook(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == KNIGHT)
		{
			delete board[posX][posY];

			Piece* c = new Knight(posX, posY, col, &board);
			board[posX][posY] = c;
		}
		else if (type == BISHOP)
		{
			delete board[posX][posY];

			Piece* c = new Bishop(posX, posY, col, &board);
			board[posX][posY] = c;
		}
	}
	void deleteFromBoard(int posX, int posY)
	{
		delete board[posX][posY];

		Piece* c = new Blank(posX, posY, WHITE);
		board[posX][posY] = c;
	}
	void printBoard()
	{
		int h = 8;
		cout << endl;
		for (int i = 0; i < 8; i++)
		{
			cout << "   ";
			for (int j = 0; j < 8; j++)
			{
				cout << board[j][i]->pieceChar << ' ';
			}
			cout << h;
			h--;
			cout << endl;
		}
		cout << "   a b c d e f g h";
		cout << endl << endl;
	}
	/*void printBoard(int x, int y)
	{
		Piece* p = board[x][y];
		int n = p->availableMoves.size();
		int* posXX = new int[n];
		int* posYY = new int[n];
		bool yes = false;

		int h = 8;
		cout << endl;
		for (int i = 0; i < 8; i++)
		{
			cout << "   ";
			for (int j = 0; j < 8; j++)
			{
				for (int a = 0; a < n; a++)
				{
					if (posXX[a] == j && posYY[a] == i)
					{
						cout << '+' << ' ';
						yes = true;
					}
				}
				if(yes==false) cout << board[j][i]->pieceChar << ' ';
			}
			cout << h;
			h--;
			cout << endl;
		}
		cout << "   a b c d e f g h";
		cout << endl << endl;
		delete[] posXX;
		delete[] posYY;
	}
	*/
};

void copyBoard(Board* from, Board* to);
bool isCheck(Board gameBoard);

bool realPosition(string input)
{
	if (input.size() != 2)
	{
		return false;
	}
	if (input[0] < 97 || input[0]>104)
	{
		return false;
	}
	if (input[1] < 49 || input[1]>56)
	{
		return false;
	}
	return true;
}
bool realPosition(Position pos)
{
	if (pos.x < 0 || pos.x > 7)
	{
		return false;
	}
	if (pos.y < 0 || pos.y > 7)
	{
		return false;
	}
	return true;
}

void setupBoard(Board* gameBoard)
{
	for (int i = 0; i < 8; i++)
	{
		gameBoard->addToBoard(PAWN, i, 1, BLACK);
		gameBoard->addToBoard(PAWN, i, 6, WHITE);
	}

	gameBoard->addToBoard(KING, 4, 0, BLACK);
	gameBoard->addToBoard(KING, 4, 7, WHITE);

	gameBoard->addToBoard(QUEEN, 3, 0, BLACK);
	gameBoard->addToBoard(QUEEN, 3, 7, WHITE);

	for (int i = 0; i < 8; i += 7)
	{
		gameBoard->addToBoard(ROOK, i, 0, BLACK);
		gameBoard->addToBoard(ROOK, i, 7, WHITE);
	}
	for (int i = 1; i < 7; i += 5)
	{
		gameBoard->addToBoard(KNIGHT, i, 0, BLACK);
		gameBoard->addToBoard(KNIGHT, i, 7, WHITE);
	}
	for (int i = 2; i < 7; i += 3)
	{
		gameBoard->addToBoard(BISHOP, i, 0, BLACK);
		gameBoard->addToBoard(BISHOP, i, 7, WHITE);
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			gameBoard->board[i][j]->fillAvailableMoves(gameBoard->board);
		}
	}
}
void stringToPos(string input, Position* pos)
{
	int j = 8;
	int yyy;
	pos->x = (input[0] - 97);
	yyy = (input[1] - 49);
	for (int i = 0; i < 8; i++)
	{
		if (yyy == i)
		{
			pos->y = j - 1;
		}
		j--;
	}
}

bool isCheckMate(Board board, Color beingChecked);
bool isCheck(Board gameBoard, Color* kCol, Position* kPos);

void findKingsPosition(Board* gameBoard, Position* wKing, Position* bKing)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* p = gameBoard->board[i][j];
			if (p->type == KING)
			{
				if (p->color == WHITE)
				{
					wKing->x = i;
					wKing->y = j;
				}
				else
				{
					bKing->x = i;
					bKing->y = j;
				}
			}
		}
	}
}

void updateAvailableMoves1(Board* gameBoard)
{
	Position wKing, bKing;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* p = gameBoard->board[i][j];
			if (p->type != BLANK)
			{
				gameBoard->addToBoard(p->type, i, j, p->color);
			}
		}
	}
}

bool isCheck(Board gameBoard, Color col);

bool containsAMove(vector<Move> prevMov, Type type, Color col, Position from)
{
	for (int i = 0, n = prevMov.size(); i < n; i++)
	{
		if ((prevMov[i].from == from) && (prevMov[i].type == type) && (prevMov[i].color == col))
		{
			return true;
		}
		else if ((prevMov[i].to == from) && (prevMov[i].type == type) && (prevMov[i].color == col))
		{
			return true;
		}
	}
	return false;
}
bool squareIsAttacked(Board board, Position square, Color attackingColor)
{
	Board b;
	Color defCol;
	b.fillBoard();
	copyBoard(&board, &b);
	Position wK, bK, kp;
	findKingsPosition(&board, &wK, &bK);
	if (attackingColor == WHITE)
	{
		defCol = BLACK;
		kp = bK;
	}
	else if (attackingColor == BLACK)
	{
		defCol = WHITE;
		kp = wK;
	}
	b.addToBoard(KING, square.x, square.y, defCol);

	b.deleteFromBoard(kp.x, kp.y);
	updateAvailableMoves1(&b);
	if (isCheck(b))
	{
		return true;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* p = board.board[i][j];
			if (p->type == BLANK || p->color != attackingColor)
			{
				continue;
			}
			for (int a = 0, n = p->availableMoves.size(); a < n; a++)
			{
				if (p->availableMoves[a] == square)
				{
					return true;
				}
			}
		}
	}
	b.deleteBoard();
	return false;
}

void updateAvailableMoves(Board* gameBoard, vector<Move> prevMov)
{
	Position wKing, bKing;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* p = gameBoard->board[i][j];
			if (p->type != BLANK)
			{
				gameBoard->addToBoard(p->type, i, j, p->color);
			}
		}
	}

	// check if king would move into check
	findKingsPosition(gameBoard, &wKing, &bKing);
	for (int i = 0; i < 2; i++)
	{
		Piece* King;
		Color otherCol;
		Position kPos;
		if (i == 0)
		{
			King = gameBoard->board[wKing.x][wKing.y];
			kPos = wKing;
		}
		else
		{
			King = gameBoard->board[bKing.x][bKing.y];
			kPos = bKing;
		}
		vector<Position> newAvailable;
		newAvailable.clear();

		Board b;
		b.fillBoard();

		for (int a = 0, n = King->availableMoves.size(); a < n; a++)
		{
			Position square;
			square = King->availableMoves[a];

			copyBoard(gameBoard, &b);
			int h;
			b.addToBoard(KING, square.x, square.y, King->color);
			b.deleteFromBoard(kPos.x, kPos.y);
			updateAvailableMoves1(&b);
			if (!isCheck(b))
			{
				newAvailable.push_back(square);
			}

		}
		King->availableMoves.clear();
		King->availableMoves = newAvailable;

		//check if king would attack into a check
		newAvailable.clear();
		for (int a = 0, n = King->availableBeatings.size(); a < n; a++)
		{
			Position square;
			square = King->availableBeatings[a];

			copyBoard(gameBoard, &b);
			int h;
			b.addToBoard(KING, square.x, square.y, King->color);
			b.deleteFromBoard(kPos.x, kPos.y);
			updateAvailableMoves1(&b);
			if (!isCheck(b))
			{
				newAvailable.push_back(square);
			}

		}
		King->availableBeatings.clear();
		King->availableBeatings = newAvailable;
	}
	// check if a move of a piece would result in a check for same color king.
	Board b;
	b.fillBoard();
	vector<Position> newAvailableM;
	newAvailableM.clear();
	vector<Position> newAvailableB;
	newAvailableB.clear();
	Piece* p;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			newAvailableM.clear();
			newAvailableB.clear();

			p = gameBoard->board[i][j];
			if (p->type == BLANK)
			{
				continue;
			}
			//updateAvailableMoves1(gameBoard);
			for (int a = 0, n = p->availableMoves.size(); a < n; a++)
			{
				copyBoard(gameBoard, &b);
				b.addToBoard(p->type, p->availableMoves[a].x, p->availableMoves[a].y, p->color);
				b.deleteFromBoard(i, j);
				updateAvailableMoves1(&b);
				if (!(isCheck(b, p->color)))
				{
					newAvailableM.push_back(p->availableMoves[a]);
				}
			}
			for (int a = 0, n = p->availableBeatings.size(); a < n; a++)
			{
				copyBoard(gameBoard, &b);
				b.addToBoard(p->type, p->availableBeatings[a].x, p->availableBeatings[a].y, p->color);
				b.deleteFromBoard(i, j);
				updateAvailableMoves1(&b);
				if (!(isCheck(b, p->color)))
				{
					newAvailableB.push_back(p->availableBeatings[a]);
				}
			}
			p->availableMoves.clear();
			p->availableMoves = newAvailableM;
			p->availableBeatings.clear();
			p->availableBeatings = newAvailableB;
		}
	}
	/// add the castle move.
	for (int i = 0; i < 8; i = i + 7)
	{
		Piece* King;
		Position wK, bK, r, m;
		Board b;
		Color ataCol;
		b.fillBoard();
		findKingsPosition(gameBoard, &wK, &bK);
		vector<Position> middleSquaresR;
		middleSquaresR.clear();
		vector<Position> middleSquaresL;
		middleSquaresL.clear();
		if (i == 0)
		{
			King = gameBoard->board[bK.x][bK.y];
			ataCol = WHITE;
		}
		else
		{
			King = gameBoard->board[wK.x][wK.y];
			ataCol = BLACK;
		}

		m.y = i;
		m.x = 5;
		middleSquaresR.push_back(m);
		m.x = 6;
		middleSquaresR.push_back(m);
		//
		m.x = 3;
		middleSquaresL.push_back(m);
		m.x = 2;
		middleSquaresL.push_back(m);
		m.x = 1;
		middleSquaresL.push_back(m);

		if (King->p.x != 4 || King->p.y != i)
		{
			continue;
		}
		// O-O
		r.x = 7; r.y = i;
		if ((!containsAMove(prevMov, ROOK, King->color, r)) && (gameBoard->board[7][i]->type == ROOK))
		{
			copyBoard(gameBoard, &b);
			if (gameBoard->board[middleSquaresR[0].x][middleSquaresR[0].y]->type == BLANK)
			{
				if (gameBoard->board[middleSquaresR[1].x][middleSquaresR[1].y]->type == BLANK)
				{
					if (!squareIsAttacked(*gameBoard, middleSquaresR[0], ataCol))
					{
						if (!squareIsAttacked(*gameBoard, middleSquaresR[1], ataCol))
						{
							Position castle;
							castle.x = 6;
							castle.y = i;
							King->availableMoves.push_back(castle);
						}
					}
				}
			}
		}
		// O-O-O
		r.x = 0; r.y = i;
		if ((!containsAMove(prevMov, ROOK, King->color, r)) && (gameBoard->board[0][i]->type == ROOK))
		{
			copyBoard(gameBoard, &b);
			if (gameBoard->board[middleSquaresL[0].x][middleSquaresL[0].y]->type == BLANK)
			{
				if (gameBoard->board[middleSquaresL[1].x][middleSquaresL[1].y]->type == BLANK)
				{
					if (gameBoard->board[middleSquaresL[2].x][middleSquaresL[2].y]->type == BLANK)
					{
						if (!squareIsAttacked(*gameBoard, middleSquaresL[0], ataCol))
						{
							if (!squareIsAttacked(*gameBoard, middleSquaresL[1], ataCol))
							{
								if (!squareIsAttacked(*gameBoard, middleSquaresL[2], ataCol))
								{
									Position castle;
									castle.x = 2;
									castle.y = i;
									King->availableMoves.push_back(castle);
								}
							}
						}
					}
				}
			}
		}
		//b.deleteBoard();
	}
	b.deleteBoard();
}

void addMove(vector<Move>* previousMoves, Position pos1, Position pos2, Type type, Color col)
{
	Move m;
	m.from = pos1;
	m.to = pos2;
	m.type = type;
	m.color = col;
	previousMoves->push_back(m);
}

void move(Board* gameBoard, int* turnCount, vector<Move>* previousMoves)
{
	updateAvailableMoves(gameBoard, *previousMoves);
	Color currentColor;


	cout << endl << "turn: " << *turnCount;
	gameBoard->printBoard();
	if (*turnCount % 2 == 0)
	{
		cout << "Its White's turn." << endl;
		currentColor = WHITE;
	}
	else
	{
		cout << "Its Black's turn." << endl;
		currentColor = BLACK;
	}
	cout << "Enter position of a piece you want to move: ";
	string input1;
	Position pos1;
	getline(cin, input1);
	if (!realPosition(input1))
	{
		move(gameBoard, turnCount, previousMoves);
		return;
	}
	stringToPos(input1, &pos1);

	if (gameBoard->board[pos1.x][pos1.y]->color != currentColor)
	{
		move(gameBoard, turnCount, previousMoves);
		return;
	}

	/*/////
	system("CLS");
	isCheck(*gameBoard);
	cout << endl << "turn: " << *turnCount;
	gameBoard->printBoard(pos1.x, pos1.y);
	if (*turnCount % 2 == 0)
	{
		cout << "Its White's turn." << endl;
		currentColor = WHITE;
	}
	else
	{
		cout << "Its Black's turn." << endl;
		currentColor = BLACK;
	}
	*//////

	cout << "Enter position you want the piece to move: ";
	string input2;
	Position pos2;
	getline(cin, input2);
	if (!realPosition(input2))
	{
		move(gameBoard, turnCount, previousMoves);
		return;
	}
	stringToPos(input2, &pos2);

	Piece* piece1 = gameBoard->board[pos1.x][pos1.y];
	Piece* piece2 = gameBoard->board[pos2.x][pos2.y];



	if (piece2->type == BLANK)
	{
		for (int i = 0; i < piece1->availableMoves.size(); i++)
		{
			if (piece1->availableMoves[i] == pos2)
			{
				Type type = piece1->type;
				Color col = piece1->color;

				// for castle
				if (piece1->type == KING)
				{
					if (pos2.x - pos1.x == 2) // O-O
					{
						gameBoard->addToBoard(ROOK, 5, pos2.y, col);
						gameBoard->deleteFromBoard(7, pos1.y);
					}
					else if (pos2.x - pos1.x == -2) // O-O-O
					{
						gameBoard->addToBoard(ROOK, 3, pos2.y, col);
						gameBoard->deleteFromBoard(0, pos1.y);
					}
				}

				//cout << "yes";
				gameBoard->deleteFromBoard(pos1.x, pos1.y);
				gameBoard->addToBoard(type, pos2.x, pos2.y, col);

				addMove(previousMoves, pos1, pos2, type, col);
				*turnCount = *turnCount + 1;
			}
		}
	}
	else
	{
		if (gameBoard->board[pos2.x][pos2.y]->color == gameBoard->board[pos1.x][pos1.y]->color)
		{
			move(gameBoard, turnCount, previousMoves);
			return;
		}
		for (int i = 0; i < piece1->availableBeatings.size(); i++)
		{
			if (piece1->availableBeatings[i] == pos2)
			{
				Type type = piece1->type;
				Color col = piece1->color;
				//cout << "yes";
				gameBoard->deleteFromBoard(pos1.x, pos1.y);
				//gameBoard->deleteFromBoard(pos2.x, pos2.y);
				gameBoard->addToBoard(type, pos2.x, pos2.y, col);

				addMove(previousMoves, pos1, pos2, type, col);

				*turnCount = *turnCount + 1;
			}
		}
	}

}
bool isCheck(Board gameBoard, Color* kCol, Position* kPos)
{
	//////////////////
	// 
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* atack = gameBoard.board[i][j];
			if (atack->type == BLANK)
			{
				continue;
			}
			for (int a = 0, n = atack->availableBeatings.size(); a < n; a++)
			{
				Piece* defend = gameBoard.board[atack->availableBeatings[a].x][atack->availableBeatings[a].y];
				if ((defend->color != atack->color) && (defend->type == KING))
				{
					*kCol = defend->color;
					//*kPos = gameBoard.board[atack->availableBeatings[a].x][atack->availableBeatings[a].y];
					kPos->x = atack->availableBeatings[a].x;
					kPos->y = atack->availableBeatings[a].y;

					return true;
				}
			}
		}
	}
	return false;

}
bool isCheck(Board gameBoard)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* atack = gameBoard.board[i][j];
			if (atack->type == BLANK)
			{
				continue;
			}
			for (int a = 0, n = atack->availableBeatings.size(); a < n; a++)
			{
				Piece* defend = gameBoard.board[atack->availableBeatings[a].x][atack->availableBeatings[a].y];
				if ((defend->color != atack->color) && (defend->type == KING))
				{
					return true;
				}
			}
		}
	}
	return false;

}
bool isCheck(Board gameBoard, Color col)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* atack = gameBoard.board[i][j];
			if ((atack->type == BLANK) || (atack->color == col))
			{
				continue;
			}
			for (int a = 0, n = atack->availableBeatings.size(); a < n; a++)
			{
				Piece* defend = gameBoard.board[atack->availableBeatings[a].x][atack->availableBeatings[a].y];
				if ((defend->color != atack->color) && (defend->type == KING))
				{
					return true;
				}
			}
		}
	}
	return false;

}
void copyBoard(Board* from, Board* to)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			Piece* p = from->board[i][j];
			to->deleteFromBoard(i, j);
			to->addToBoard(p->type, i, j, p->color);
		}
	}
}

bool isMate(Board board, Color checkedColor, Position kPos, vector<Move> prevMov)
{
	Board b1;
	b1.fillBoard();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* p = board.board[i][j];

			if ((p->type != BLANK) && p->color == checkedColor)
			{
				for (int a = 0, n = p->availableMoves.size(); a < n; a++)
				{
					copyBoard(&board, &b1);
					b1.addToBoard(p->type, p->availableMoves[a].x, p->availableMoves[a].y, p->color);
					b1.deleteFromBoard(i, j);
					updateAvailableMoves(&b1, prevMov);
					if (!isCheck(b1))
					{
						cout << endl << p->availableMoves[a].x << " " << p->availableMoves[a].y;
						return false;
					}
				}
				for (int a = 0, n = p->availableBeatings.size(); a < n; a++)
				{
					copyBoard(&board, &b1);
					b1.addToBoard(p->type, p->availableBeatings[a].x, p->availableBeatings[a].y, p->color);
					b1.deleteFromBoard(i, j);
					updateAvailableMoves(&b1, prevMov);
					if (!isCheck(b1))
					{
						cout << endl << p->availableBeatings[a].x << " " << p->availableBeatings[a].y;
						return false;
					}
				}
			}
		}
	}
	b1.deleteBoard();
	return true;
}

bool isDraw(Board board, vector<Move> previousMoves)
{
	Piece* King;
	Position wK, bK;
	findKingsPosition(&board, &wK, &bK);

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			King = board.board[wK.x][wK.y];
		}
		else
		{
			King = board.board[bK.x][bK.y];
		}

		// by stealmate
		bool zeroAvailable = true;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Piece* p = board.board[i][j];
				if ((p->color == King->color) && (p->type != BLANK))
				{
					if (p->availableMoves.size() != 0 || p->availableBeatings.size() != 0)
					{
						zeroAvailable = false;
					}
				}
			}
		}
		if ((zeroAvailable == true) && (!(isCheck(board))))
		{
			return true;
		}
	}

	// by repetition
	int last = previousMoves.size();
	if (last >= 9)
	{
		last--;
		if ((previousMoves[last] == previousMoves[last - 4]) && (previousMoves[last - 4] == previousMoves[last - 8]))
		{
			if ((previousMoves[last - 1] == previousMoves[last - 5]) && (previousMoves[last - 5] == previousMoves[last - 9]))
			{
				if (previousMoves[last - 2] == previousMoves[last - 6])
				{
					if (previousMoves[last - 3] == previousMoves[last - 7])
					{
						return true;
					}
				}
			}
		}
	}

	return false;

}

void createSquarePosVec(vector<vector<sf::Vector2f>>* sqPos, Board gameBoard, sf::Sprite board)
{
	sf::Vector2f firstSquare(board.getPosition().x + 22, board.getPosition().y + 7);
	sf::Vector2f a1(firstSquare.x , firstSquare.y + (81 * 7));
	for (int i = 0; i < 8; i++)
	{
		vector<sf::Vector2f> row;
		for (int j = 0; j < 8; j++)
		{
			row.push_back(a1);
			a1.x += 81;
		}
		a1.y -= 81;
		a1.x -= 81 * 8;
		sqPos->push_back(row);
	}
	vector<vector<sf::Vector2f>> t;
	for (int i = 7; i >= 0; i--)
	{
		t.push_back((*sqPos)[i]);
	}
	*sqPos = t;
	for (int i = 0; i < 8; i++)
	{
		for (int j = i; j < 8; j++)
		{
			sf::Vector2f temp = (*sqPos)[i][j];
			(*sqPos)[i][j] = (*sqPos)[j][i];
			(*sqPos)[j][i] = temp;
		}
	}
}

void setTextures(vector<sf::Texture>* textures)
{
	for (int t = 0; t < 2; t++)
	{
		for (int i = 0; i <= 5; i++)
		{
			sf::Texture pawnTex;
			string name;
			if (i == 0)
			{
				name = "pawn";
			}
			else if (i == 1)
			{
				name = "knight";
			}
			else if (i == 2)
			{
				name = "bishop";
			}
			else if (i == 3)
			{
				name = "rook";
			}
			else if (i == 4)
			{
				name = "queen";
			}
			else if (i == 5)
			{
				name = "king";
			}

			if (t==0)
			{
				pawnTex.loadFromFile("textures/" + name + "_white.png");
			}
			if (t==1)
			{
				pawnTex.loadFromFile("textures/" + name + "_black.png");
			}
			textures->push_back(pawnTex);
			cout << name << t;
		}
	}
}

void printBoardSFML(Board gameBoard, sf::RenderWindow &window, int turnCount, vector<pair<sf::Sprite*,
	Position>>* curSpr, vector<vector<sf::Vector2f>> sqPos, vector<sf::Texture> textures, vector<Move> prevoiusMoves)
{
	window.clear(sf::Color::Black);
	sf::Font font;
	font.loadFromFile("font/font.otf");

	sf::Text tc;
	tc.setFont(font);
	tc.setCharacterSize(40);
	tc.setPosition(730.f, 60.f);
	tc.setString("Turn: " + std::to_string(turnCount));

	sf::Text result;
	result.setFont(font);
	result.setCharacterSize(40);
	result.setPosition(730.f, 120.f);
	string res = "";
	Color kCol;
	Position kPos;
	if (isCheck(gameBoard, &kCol, &kPos))
	{
		if (isMate(gameBoard, kCol, kPos, prevoiusMoves))
		{
			res = "MATE !";
		}
		else res = "CHECK !";
	}
	if (isDraw(gameBoard, prevoiusMoves)) res = "DRAW !";

	result.setString(res);
	window.draw(result);

	sf::CircleShape circle;
	circle.setRadius(3.f);
	circle.setFillColor(sf::Color::Magenta);

	sf::Sprite board;
	sf::Texture boardTex;
	boardTex.loadFromFile("textures/board.png");
	board.setTexture(boardTex);
	board.setScale(0.5f, 0.5f);
	//board.setPosition((window.getSize().x/2) - (boardTex.getSize().x/2), (window.getSize().y / 2) + (boardTex.getSize().y / 2));
	board.setPosition(0.f, 50.f);

	curSpr->clear();

	//circle.setPosition(sqPos[1][2]);

	window.draw(board);
	//window.draw(circle);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (gameBoard.board[i][j]->type != BLANK)
			{
				int r;
				sf::Sprite pawn;
				sf::Texture pawnTex;
				string name;
				//string c;
				if (gameBoard.board[i][j]->color == WHITE) r = 0;
				else r = 6;

				if (gameBoard.board[i][j]->type == PAWN)
				{
					pawn.setTexture(textures[r]);
				}
				else if (gameBoard.board[i][j]->type == KNIGHT)
				{
					pawn.setTexture(textures[r+1]);
				}
				else if (gameBoard.board[i][j]->type == BISHOP)
				{
					pawn.setTexture(textures[r + 2]);
				}
				else if (gameBoard.board[i][j]->type == ROOK)
				{
					pawn.setTexture(textures[r + 3]);
				}
				else if (gameBoard.board[i][j]->type == QUEEN)
				{
					pawn.setTexture(textures[r + 4]);
				}
				else if (gameBoard.board[i][j]->type == KING)
				{
					pawn.setTexture(textures[r + 5]);
				}
				//.setTexture(pawnTex);
				pawn.setPosition(sqPos[i][j].x+7, sqPos[i][j].y + 5);
				pawn.setScale(0.085f,0.085f);
				window.draw(pawn);

				Position pos;
				pos.x = i;
				pos.y = j;
				curSpr->push_back(make_pair(&pawn, pos));
				
				sf::Sprite* p1;
				int c = curSpr->size() - 1;
				p1 = (*curSpr)[c].first;

				/*sf::RectangleShape outline(sf::Vector2f(p1->getGlobalBounds().width, p1->getGlobalBounds().height));
				outline.setPosition(p1->getGlobalBounds().left, p1->getGlobalBounds().top);
				outline.setFillColor(sf::Color::Transparent);
				outline.setOutlineThickness(2.f);
				outline.setOutlineColor(sf::Color::Red);
				window.draw(outline);*/
			}
			cout << "t";
		}
	}

	window.draw(tc);
	//window.draw(result);
	window.display();
}

bool sfMove(Board* gameBoard, sf::RenderWindow& window, int turnCount, vector<pair<sf::Sprite*, Position >>* curSpr,
	vector<vector<sf::Vector2f>> sqPos, sf::Vector2f mousePos, vector<Move> prevoiusMoves, vector<Position>* availableM,
	Piece** pc, Position* piecePos, vector<sf::Texture>textures)
{
	//printBoardSFML(*gameBoard, window, turnCount, curSpr, sqPos);
	//cout << "q";
	//updateAvailableMoves(gameBoard, prevoiusMoves);
	//cout << "Q";
	bool tf = false;

	Color curCol;
	if (turnCount % 2 == 0) curCol = WHITE;
	else curCol = BLACK;

	printBoardSFML(*gameBoard, window, turnCount, curSpr, sqPos, textures, prevoiusMoves);
	//cout << "E";

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position pos;
			pos.x = i;
			pos.y = j;


			sf::FloatRect bounds;
			bounds.left = sqPos[i][j].x;
			bounds.top = sqPos[i][j].y;
			bounds.width = 76;
			bounds.height = 76;
			if (bounds.contains(mousePos))
			{
				cout << "A";
				if (gameBoard->board[pos.x][pos.y]->type != BLANK && gameBoard->board[pos.x][pos.y]->color == curCol)
				{
					Piece* piece;
					piece = gameBoard->board[pos.x][pos.y];

					availableM->clear();
					for (int i = 0, n = piece->availableMoves.size(); i < n; i++)
					{
						sf::Sprite avMov;
						sf::Texture avMovTex;
						avMovTex.loadFromFile("textures/grey_dot.png");
						avMov.setTexture(avMovTex);
						avMov.setPosition(sqPos[piece->availableMoves[i].x][piece->availableMoves[i].y]);
						avMov.setScale(0.105f, 0.105f);

						Position aM;
						aM.x = piece->availableMoves[i].x;
						aM.y = piece->availableMoves[i].y;
						availableM->push_back(aM);
						pc = &piece;
						*piecePos = piece->p;


						window.draw(avMov);
						tf = true;
					}
					for (int i = 0, n = piece->availableBeatings.size(); i < n; i++)
					{
						sf::Sprite avMov;
						sf::Texture avMovTex;
						avMovTex.loadFromFile("textures/red_dot.png");
						avMov.setTexture(avMovTex);
						avMov.setPosition(sqPos[piece->availableBeatings[i].x][piece->availableBeatings[i].y]);
						avMov.setScale(0.105f, 0.105f);

						Position aM;
						aM.x = piece->availableBeatings[i].x;
						aM.y = piece->availableBeatings[i].y;
						availableM->push_back(aM);
						pc = &piece;
						*piecePos = piece->p;


						window.draw(avMov);
						tf = true;
					}
					cout << (*piecePos).x << (*piecePos).y;

				}
			}
			//cout << "q";
		}
	}
	window.display();
	return tf;
}

bool sfMovePart2(Board* gameBoard, Piece* piece, sf::RenderWindow& window, vector<vector<sf::Vector2f>> sqPos,
	sf::Vector2f mousePos, vector<Position>* availableM, Position piecePos)
{
	Piece* p1;
	p1 = gameBoard->board[piecePos.x][piecePos.y];
	for (int i=0, n=availableM->size(); i<n; i++)
	{
		sf::FloatRect bounds;
		bounds.left = sqPos[(*availableM)[i].x][(*availableM)[i].y].x;
		bounds.top = sqPos[(*availableM)[i].x][(*availableM)[i].y].y;
		bounds.width = 76;
		bounds.height = 76;

		if (bounds.contains(mousePos))
		{
			gameBoard->addToBoard(p1->type, (*availableM)[i].x, (*availableM)[i].y, p1->color);
			gameBoard->deleteFromBoard(p1->p.x, p1->p.y);
			//printBoardSFML(*gameBoard, window, turnCount, curSpr, sqPos);
			return true;
		}
	}
	return false;
}

int main()
{

	int turnCount = 0;
	vector<Move> prevoiusMoves;

	Board gameBoard;
	gameBoard.fillBoard();
	setupBoard(&gameBoard);
	//gameBoard.deleteFromBoard(4,6);

	sf::RenderWindow window(sf::VideoMode(900.f, 750.f), "CHESS !!!");
	window.setFramerateLimit(60);
	//window.clear(sf::Color::Black);

	vector<pair<sf::Sprite*, Position >> curSpr;

	vector<vector<sf::Vector2f>> sqPos;
	vector<sf::Texture> textures;
	setTextures(&textures);

	sf::Sprite board;
	sf::Texture boardTex;
	boardTex.loadFromFile("textures/board.png");
	board.setTexture(boardTex);
	board.setScale(0.5f, 0.5f);
	//board.setPosition((window.getSize().x/2) - (boardTex.getSize().x/2), (window.getSize().y / 2) + (boardTex.getSize().y / 2));
	board.setPosition(0.f, 50.f);
	createSquarePosVec(&sqPos, gameBoard, board);
	window.clear(sf::Color::White);
	printBoardSFML(gameBoard, window, turnCount, &curSpr, sqPos, textures, prevoiusMoves);
	updateAvailableMoves(&gameBoard, prevoiusMoves);
	vector<Position>availableM;
	Blank b(0,0,WHITE);
	Piece* cp = &b;
	Position piecePos;
	piecePos.x = 0; piecePos.y = 0;
	bool xyz = true;

	while (window.isOpen())
	{
		sf:: Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				window.close();

			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && xyz)
				{
					cout << "N";
					//sf::Vector2f mousePos = sf::Mouse::getPosition(window);
					sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
					sf::Vector2f mousePos = static_cast<sf::Vector2f>(mousePosInt);
					if (sfMove(&gameBoard, window, turnCount, &curSpr, sqPos, mousePos, prevoiusMoves, &availableM, &cp, &piecePos, textures))
					{

						cout << piecePos.x << piecePos.y;
						cout << "Y";
						xyz = false;
						while (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {}
						if (event.mouseButton.button == sf::Mouse::Left && !xyz)
						{
							cout << "G";
							mousePosInt = sf::Mouse::getPosition(window);
							mousePos = static_cast<sf::Vector2f>(mousePosInt);
							if (sfMovePart2(&gameBoard, cp, window, sqPos, mousePos, &availableM, piecePos))
							{
								turnCount++;
							}
							printBoardSFML(gameBoard, window, turnCount, &curSpr, sqPos, textures, prevoiusMoves);
							updateAvailableMoves(&gameBoard, prevoiusMoves);
							xyz = true;
						}
					}
				}
			}
		}

		////////////////////	CONSOLE
		Position p1, p2;
		Move m;

		Color kCol;
		Position kPos;
		if (isCheck(gameBoard, &kCol, &kPos))
		{
			//cout << "CHECK!!!" << endl;
			if (isMate(gameBoard, kCol, kPos, prevoiusMoves))
			{
				//return 0;
			}
		}
		if (isDraw(gameBoard, prevoiusMoves))
		{
			//cout << "DRAWWW!!!" << endl;
		}
	}
	/////////////////////////////
	gameBoard.deleteBoard();
	return 666;
	return 0;
}

