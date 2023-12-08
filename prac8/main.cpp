#include "iostream"
#include "vector"
#include "windows.h"

using namespace std;


class Room {
public:
	int x;
	int y;
	bool isUpWall = false;
	bool isDownWall = false;
	bool isRightWall = false;
	bool isLeftWall = false;

	int *roomSize = nullptr;
	bool isVisited = false;


	Room(int x, int y) : x(x), y(y) {}
};

class Wall {
public:
	Room *fRoom;
	Room *sRoom;

	Wall(Room *fRoom, Room *sRoom) : fRoom(fRoom), sRoom(sRoom) {}

	int compute() const {
		if (fRoom->roomSize == sRoom->roomSize)
			return -1;
		return *fRoom->roomSize + *sRoom->roomSize;
	}
};

class Castle {


	int width;
	int height;
	Room ***rooms;

public:
	int count = 0;
	vector<Wall> walls;
	Room *maxRoom = nullptr;

	Castle(int height, int width) {
		this->width = width;
		this->height = height;
		rooms = new Room **[height];
		for (int i = 0; i < height; ++i) {
			rooms[i] = new Room *[width];
			for (int j = 0; j < width; ++j) {
				rooms[i][j] = new Room(j, i);
			}
		}
	}

	Room *getRoom(int x, int y) {
		return rooms[y][x];
	}

	bool isBound(int x, int y) const {
		return x < width && x >= 0 && y < height && y >= 0;
	}

	void deepAnalysis(int x, int y, int *roomSize = new int(0)) {
		Room *room = rooms[y][x];
		room->isVisited = true;
		room->roomSize = roomSize;
		*roomSize += 1;
		if (!room->isRightWall && isBound(x + 1, y) && !rooms[y][x + 1]->isVisited) {
			deepAnalysis(x + 1, y, roomSize);
		} else {
			if (isBound(x + 1, y)) {
				walls.emplace_back(Wall(room, getRoom(x + 1, y)));
			}
		}
		if (!room->isLeftWall && isBound(x - 1, y) && !rooms[y][x - 1]->isVisited) {
			deepAnalysis(x - 1, y, roomSize);
		}
		if (!room->isDownWall && isBound(x, y + 1) && !rooms[y + 1][x]->isVisited) {
			deepAnalysis(x, y + 1, roomSize);
		} else {
			if (isBound(x, y + 1)) {
				walls.emplace_back(Wall(room, getRoom(x, y + 1)));
			}
		}
		if (!room->isUpWall && isBound(x, y - 1) && !rooms[y - 1][x]->isVisited) {
			deepAnalysis(x, y - 1, roomSize);
		}
	}

	int processRooms() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (!rooms[y][x]->isVisited) {
					deepAnalysis(x, y);
					if (maxRoom == nullptr || *maxRoom->roomSize < *getRoom(x, y)->roomSize) {
						maxRoom = getRoom(x, y);
					}
					count += 1;
				}
			}
		}
		return count;
	}

};

int main() {
	SetConsoleOutputCP(CP_UTF8);
	int height, width;
	cin >> height >> width;

	Castle castle(height, width);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int walls;
			cin >> walls;
			Room *room = castle.getRoom(j, i);
			if (walls >= 8) {
				walls -= 8;
				room->isUpWall = true;
			}
			if (walls >= 4) {
				walls -= 4;
				room->isRightWall = true;
			}
			if (walls >= 2) {
				walls -= 2;
				room->isDownWall = true;
			}
			if (walls >= 1) {
				walls -= 1;
				room->isLeftWall = true;
			}
		}
	}
	castle.processRooms();
	cout << "Всего комнат - " << castle.count << endl;
	cout << "Длина самой большой комнаты - " << *castle.maxRoom->roomSize << endl;
	Wall max = castle.walls[0];
	for (auto wall: castle.walls) {
		if (max.compute() < wall.compute()) {
			max = wall;
		}
	}
	cout << "Длина самой большой комнаты после ломания стены - " << max.compute() << " сломана стена между комнатами ("
		 << max.fRoom->x << "," << max.fRoom->y << "), ("
		 << max.sRoom->x << "," << max.sRoom->y << ")";
}

/*
 *
		4 7
		2 4 3 4 1 2 0
		12 3 12 7 5 15 1
		0 10 6 13 7 13 1
		4 9 8 0 8 4 1
 *
 */