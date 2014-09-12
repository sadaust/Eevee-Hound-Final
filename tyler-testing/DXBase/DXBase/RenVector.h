enum RenType {
	none,
	sprite,
	text,
	primitive,
	model
};

struct RenInfo {
	RenType type;
	void* asset;
	//0 for world
	//1+ for anything that should only be drawn once such as huds
	//the number coresponds to the local screen number when using split screen
	unsigned short locCamNum;
};

class RenVector {
private:
	int used;
	int allocated;
	RenInfo* data;
public:
	RenVector();
	void clear();
	void setSize(int);
	void add(RenInfo&);
	int getSize(){return used;}
	RenInfo* get(int);
	~RenVector();
};