struct Point {
	int x;
	int y;
}

struct Snake {
	Point head;
	int length;
}

Snake snake;

bool frame[8][8]{
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

void setPoint(Point point){
	frame[point.x][point.y] = 1;
}

void resetPoint(Point point){
	frame[point.x][point.y] = 0;
}

