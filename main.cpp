#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <array>
#include "piece.h"

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BLOCK_SIZE_PIXELS = 36;
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;

const int BOARD_TOP_X = WINDOW_WIDTH / 2 - BOARD_WIDTH / 2 * BLOCK_SIZE_PIXELS;
const int BOARD_TOP_Y = WINDOW_HEIGHT / 2 - BOARD_HEIGHT / 2 * BLOCK_SIZE_PIXELS;

std::vector<std::vector<int>> board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));

int pieceSpeed = 48;
int framecount = 0;

int score = 0;
int linesCleared = 0;
int level = 0;

std::array<int, 30> levelSpeeds{ 48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

bool gameOver = false;

sf::RenderWindow window;
sf::Texture blockTexture;
sf::Sprite blockSprite;
sf::RectangleShape boardRect(sf::Vector2f(BOARD_WIDTH * BLOCK_SIZE_PIXELS, BOARD_HEIGHT * BLOCK_SIZE_PIXELS));

sf::Font font;

sf::Music music;

std::map<int, sf::Color> colors{
	{0, sf::Color(231, 227, 232)}, //white
	{1, sf::Color(0, 93, 189)}, //blue
	{2, sf::Color(33, 142, 222)}, //cyan
	{3, sf::Color(74, 158, 33)}, //green
	{4, sf::Color(249, 106, 44)}, //orange
	{5, sf::Color(148, 44, 156)}, //purple
	{6, sf::Color(148, 8, 8)}, // red
	{7, sf::Color(255, 195, 41)}, //yellow
};

enum class Direction
{
	Right,
	Left,
	Down
};

Piece generateNewPiece() {
	Piece piece;
	return piece;
}

void printBoard() {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			std::cout << board[y][x];
		}
		std::cout << std::endl;
	}
}

bool canMove(Piece* piece, Direction dir) {
	switch (dir) {
		case Direction::Right: 
		{
			vec2d newPosition{ piece->getPosition().x + 1, piece->getPosition().y };
			int size = piece->getShape()[0].size();
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					if (piece->getShape()[y][x] != 0) {
						if (newPosition.x + x > 9) {
							return false;
						}
						if (board[newPosition.y + y][newPosition.x + x] != 0) {
							return false;
						}
					}
				}
			}
			break;
		}
		case Direction::Left:
		{
			vec2d newPos{ piece->getPosition().x - 1, piece->getPosition().y };
			int size = piece->getShape()[0].size();
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					if (piece->getShape()[y][x] != 0) {
						if (newPos.x + x < 0) {
							return false;
						}
						if (board[newPos.y + y][newPos.x + x] != 0) {
							return false;
						}
					}
				}
			}
			break;
		}
		case Direction::Down:
		{
			vec2d newPos{ piece->getPosition().x, piece->getPosition().y + 1 };
			int size = piece->getShape()[0].size();
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					if (piece->getShape()[y][x] != 0) {
						if (newPos.y + y > 19) {
							return false;
						}
						if (board[newPos.y + y][newPos.x + x] != 0) {
							return false;
						}
					}
				}
			}
			break;
		}
	}
	return true;
}

bool canRotate(Piece* piece, Direction dir) {
	if (dir == Direction::Right) {
		std::vector<std::vector<int>> rotated = piece->getRotatedCW();
		int size = rotated.size();
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				if (rotated[y][x] != 0) {
					if (y + piece->getPosition().y < 0 || y + piece->getPosition().y > BOARD_HEIGHT) return false;
					if (x + piece->getPosition().x < 0 || x + 1 + piece->getPosition().x > BOARD_WIDTH) return false;
					if (board[y + piece->getPosition().y][x + piece->getPosition().x] != 0) {
						return false;
					}
				}
			}
		}
	}
	else {
		std::vector<std::vector<int>> rotated = piece->getRotatedCCW();
		int size = rotated.size();
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				if (rotated[y][x] != 0) {
					if (y + piece->getPosition().y < 0 || y + piece->getPosition().y > BOARD_HEIGHT) return false;
					if (x + piece->getPosition().x < 0 || x + 1 + piece->getPosition().x > BOARD_WIDTH) return false;
					if (board[y + piece->getPosition().y][x + piece->getPosition().x] != 0) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void checkForLineClears() {
	int lineClears = 0;
	for (int row = 0; row < BOARD_HEIGHT; row++) {
		bool full = true;
		for (int col = 0; col < BOARD_WIDTH; col++) {
			if (board[row][col] == 0) {
				full = false;
			}
		}
		if (full) {
			lineClears++;
			board.erase(board.begin() + row);
			board.insert(board.begin(), { 0,0,0,0,0,0,0,0,0,0 });
		}
	}
	switch (lineClears) {
		case 0:
			break;
		case 1:
			score += 40 * (level + 1);
			break;
		case 2:
			score += 100 * (level + 1);
			break;
		case 3:
			score += 300 * (level + 1);
			break;
		case 4:
			score += 1200 * (level + 1);
			break;
	}
	linesCleared += lineClears;
	if (lineClears != 0 && (linesCleared % 10 == 0)) {
		level++;
		pieceSpeed = levelSpeeds[level];
	}
}

void landPiece(Piece& currentPiece, Piece& nextPiece) {
	int size = currentPiece.getShape()[0].size();
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if (currentPiece.getShape()[y][x] != 0) {
				board[currentPiece.getPosition().y + y][currentPiece.getPosition().x + x] = currentPiece.getType();
			}
		}
	}
	checkForLineClears();
	Piece newPiece;
	currentPiece = nextPiece;
	nextPiece = newPiece;
	//check for game over
	for (int i = 3; i < 7; i++) {
		if (board[0][i] != 0)
			gameOver = true;
	}
}


void updateGame(Piece& currentPiece, Piece& nextPiece) {
	framecount++;
	//Make the pieces fall every x amount of frames
	if (framecount % pieceSpeed == 0) {
		if (canMove(&currentPiece, Direction::Down)) {
			currentPiece.moveDown();
		}
		else { //land the piece
			landPiece(currentPiece, nextPiece);
		}
	}
}

void draw(Piece currentPiece, Piece nextPiece) {
	//Clear
	window.clear();
	//Draw the board rectangle outline
	window.draw(boardRect);
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] != 0) {
				blockSprite.setColor(colors[board[y][x]]);
				blockSprite.setPosition(sf::Vector2f(x * BLOCK_SIZE_PIXELS + BOARD_TOP_X,
													(y * BLOCK_SIZE_PIXELS + BOARD_TOP_Y)));
				window.draw(blockSprite);
			}
		}
	}

	//debugging text for piece position
	/*sf::Text text(sf::String(std::to_string(currentPiece.getPosition().x) + ", " + std::to_string(currentPiece.getPosition().y)), font, 20);
	window.draw(text);*/

	sf::Text nextText(sf::String("Next"), font, 30);
	nextText.setPosition(sf::Vector2f(700, BOARD_TOP_Y));
	window.draw(nextText);

	blockSprite.setColor(colors[nextPiece.getType()]);
	int s = nextPiece.getShape()[0].size();
	for (int y = 0; y < s; y++) {
		for (int x = 0; x < s; x++) {
			if (nextPiece.getShape()[y][x] != 0) {
				blockSprite.setPosition(sf::Vector2f(700 + (BLOCK_SIZE_PIXELS * x), BOARD_TOP_Y + 45 + (y * BLOCK_SIZE_PIXELS)));
				window.draw(blockSprite);
			}
		}
	}
	

	sf::Text scoreText(sf::String("Score\n" + std::to_string(score)), font, 30);
	scoreText.setPosition(sf::Vector2f(700, 250));
	window.draw(scoreText);

	//Draw stuff
	blockSprite.setColor(colors[currentPiece.getType()]);
	int size = currentPiece.getShape()[0].size();
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if (currentPiece.getShape()[y][x] != 0) {
				blockSprite.setPosition(sf::Vector2f((x + currentPiece.getPosition().x) * BLOCK_SIZE_PIXELS + BOARD_TOP_X,
													 (y + currentPiece.getPosition().y) * BLOCK_SIZE_PIXELS + BOARD_TOP_Y));
				window.draw(blockSprite);
			}
		}
	}

	sf::Text levelText(sf::String("Level\n" + std::to_string(level)), font, 30);
	levelText.setPosition(sf::Vector2f(700, 375));
	window.draw(levelText);

	if (gameOver) {
		sf::Text gameOverText(sf::String("Game Over!"), font, 30);
		gameOverText.setPosition(sf::Vector2f(650, 450));
		window.draw(gameOverText);
	}

	//Update screen once per frame
	window.display();
}

int main() {
	Piece currentPiece;
	Piece* p = &currentPiece;
	Piece nextPiece;
	//Define the window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
	boardRect.setFillColor(sf::Color::Black);
	boardRect.setOutlineColor(sf::Color::White);
	boardRect.setOutlineThickness(1.0f);

	sf::Clock clock;
	sf::Time time = sf::Time::Zero;
	sf::Time fps = sf::seconds(1.0f / 60.0f);

	if (!blockTexture.loadFromFile("whiteblock.png")) {
		std::cout << "oops";
	}
	if (!font.loadFromFile("retro.ttf")) {
		std::cout << "font oops";
	}
	if (!music.openFromFile("TetrisThemeACover.wav")) {
		return -1;
	}
	music.setLoop(true);
	music.play();
	
	blockSprite.setTexture(blockTexture);
	
	boardRect.setPosition(BOARD_TOP_X, BOARD_TOP_Y);

	pieceSpeed = levelSpeeds[level];
	
	//Game loop
	while (window.isOpen()) {
		//Event stuff
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == event.Closed) {
				window.close();
			}

			if (event.type == event.KeyPressed && !gameOver) {
				switch (event.key.code) {
					case sf::Keyboard::Right:
						if (canMove(p, Direction::Right)) {
							currentPiece.moveRight();
						}
						break;
					case sf::Keyboard::Left:
						if (canMove(p, Direction::Left)) {
							currentPiece.moveLeft();
						}
						break;
					case sf::Keyboard::Down:
						if (canMove(p, Direction::Down)) {
							currentPiece.moveDown();
						}
						break;
					case sf::Keyboard::Space:
						while (canMove(p, Direction::Down)) {
							currentPiece.moveDown();
						}
						landPiece(currentPiece, nextPiece);
						break;
					case sf::Keyboard::X:{
						if (canRotate(p, Direction::Right)) {
							currentPiece.rotateCW();
						}
						break;
					}
					case sf::Keyboard::Z: {
						if (canRotate(p, Direction::Left)) {
							currentPiece.rotateCCW();
						}
						break;
					}
				}
			}
		}
		time += clock.restart();

		while (time > fps && !gameOver) {
			time -= fps;
			updateGame(currentPiece, nextPiece);
			draw(currentPiece, nextPiece);
		}
		if (gameOver) {
			music.stop();
			draw(currentPiece, nextPiece);
		}
	}
	return 0;
}
