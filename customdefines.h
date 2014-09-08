#ifndef _CUSTOMDEFINES_H_
#define _CUSTOMDEFINES_H_

struct Position
{
	float x, y, z;
	float alpha, beta, gamma;
};

struct Kart
{
	float x, y, z;
	float alpha;
	int charid;
	bool visflag;
};

struct KartStats
{
	int laps;
	int cpn;
	int waypoint;
	float spd;
	bool lapflag1;
	bool lapflag2;
	bool finished;
};

struct tVertex
{
	float x, y, z, tnx, tny;
};

struct Tile
{
	tVertex Vertex[4];
};

#endif