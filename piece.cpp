#include "piece.h"
#include <vector>

void Piece::moveRight() {
	position.x++;
}

void Piece::moveLeft() {
	position.x--;
}

void Piece::moveDown() {
	position.y++;
}

void Piece::rotateCW() {
	shape = getRotatedCW();
}

void Piece::rotateCCW() {
	shape = getRotatedCCW();
}

std::vector<std::vector<int>> Piece::getRotatedCW() {
	int size = shape[0].size();
	std::vector<std::vector<int>> rotated = shape;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			rotated[i][j] = shape[size - j - 1][i];
		}
	}
	return rotated;
}

std::vector<std::vector<int>> Piece::getRotatedCCW() {
	int size = shape[0].size();
	std::vector<std::vector<int>> rotated = shape;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			rotated[i][j] = shape[j][size - i - 1];
		}
	}
	return rotated;
}

std::vector<std::vector<int>> Piece::getShape() {
	return shape;
}

vec2d Piece::getPosition() {
	return position;
};

PieceType Piece::getType() {
	return pieceType;
}

Piece::Piece()
	: position({ 3,0 }),
	pieceType(PieceType::I),
	shape(Piece::pieceShapes.at(PieceType::I))
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 7);
	pieceType = (PieceType)dist(rng);
	if (pieceType == PieceType::O) {
		position.x++;
	}
	if (pieceType == PieceType::I) {
		position.y--;
	}
	shape = pieceShapes.at(pieceType);
}



const std::map<PieceType, std::vector<std::vector<int>>> Piece::pieceShapes{
	{PieceType::I, {{0,0,0,0},
					{2,2,2,2},
					{0,0,0,0},
					{0,0,0,0}}},
	{PieceType::O, {{7,7},
					{7,7}}},
	{PieceType::T, {{0,5,0},
					{5,5,5},
					{0,0,0}}},
	{PieceType::S, {{0,3,3},
					{3,3,0},
					{0,0,0}}},
	{PieceType::Z, {{6,6,0},
					{0,6,6},
					{0,0,0}}},
	{PieceType::J, {{4,0,0},
					{4,4,4},
					{0,0,0}}},
	{PieceType::L, {{0,0,1},
					{1,1,1},
					{0,0,0}}}
};