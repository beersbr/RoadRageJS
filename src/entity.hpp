#ifndef GENTITY_HPP
#define GENTITY_HPP

class Entity
{
public:
	Entity();
	Entity(Entity &e);
	~Entity();

	int x;
	int y;

	float vx;
	float vy;

	int type;
	int id;

	std::string fname;
	std::string email;
	std::string screenname;
};

#endif