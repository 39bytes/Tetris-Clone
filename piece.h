#pragma once
#include <map>
#include <vector>
#include <random>
struct vec2d {
	int x, y;
};

enum PieceType {
	None, L, I, S, J, T, Z, O
};

class Piece
{
public:
	vec2d getPosition();
	PieceType getType();
	void moveRight();
	void moveLeft();
	void moveDown();
	void rotateCW();
	void rotateCCW();
	std::vector<std::vector<int>> getShape();
	std::vector<std::vector<int>> getRotatedCW();
	std::vector<std::vector<int>> getRotatedCCW();
	static const std::map<PieceType, std::vector<std::vector<int>>> pieceShapes;
	Piece();
private:
	vec2d position; //the top left of the piece
	PieceType pieceType;
	std::vector<std::vector<int>> shape;
};

