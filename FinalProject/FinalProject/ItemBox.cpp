#include "ItemBox.h"

const float terminalVelocity = 10.0f;






ItemBox::~ItemBox() {

}


ItemBox::ItemBox(){
	//itemBox.mat=0;
	//D3DXMatrixIdentity(&itemBox.matrix);
	//itemBox.primInfo=0;
	//itemBox.Tex=0;
	//part.LimbInit(); // for testing
	//pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//bounding.height = 0.5f;
	//bounding.radius = 0.5f;
	//velocityY=0;
	//onGround=false;
	//active=true;
}

void ItemBox::ItemBoxInit(sPoint& spawn,PrimObj a_itemObj){
	//(float a_x, float a_y, float a_z) 
	a_box=a_itemObj;
	part.LimbInit();
	velocityY=0;
	velocityXZ = D3DXVECTOR2(0.0f,0.0f);
	onGround=false;
	active=true;
	pos.x = 10;//spawn.getPos().x;
	pos.y = spawn.getPos().y;
	pos.z = spawn.getPos().z;
	prospectivePos.x = pos.x;
	prospectivePos.y = pos.y;
	prospectivePos.z = pos.z;
	bounding.height = 0.25f;
	bounding.radius = 0.25f;
	part.LimbInit(); //for testing
	rot = 0;
}

D3DXVECTOR3 ItemBox::getPos() {
	return pos;
}

D3DXVECTOR3 ItemBox::getProspectivePos() {
	return prospectivePos;
}

cylinder ItemBox::getBound() {
	return bounding;
}

void ItemBox::setBound(float a_h, float a_r) {
	bounding.height = a_h;
	bounding.radius = a_r;
}

void ItemBox::setPosInts(float a_x, float a_y, float a_z) {
	pos = D3DXVECTOR3(a_x,a_y,a_z);
}

void ItemBox::setPos(D3DXVECTOR3 a_pos) {
	pos = a_pos;
}

void ItemBox::setProspectivePos(D3DXVECTOR3 a_prospos) {
	prospectivePos = a_prospos;
}

void ItemBox::setVelocityY(float a_velocityY) {
	velocityY = a_velocityY;
}

void ItemBox::toggleGrounded(bool a_grounded) {
	onGround = a_grounded;
}

void ItemBox::Update(float a_dt) {
	float tempfloat;
	rot += a_dt;
	pos.x = prospectivePos.x;
	pos.y = prospectivePos.y;
	pos.z = prospectivePos.z;

	velocityY -= gravity*a_dt;
	if(velocityY < -terminalVelocity)
		velocityY = -terminalVelocity;
	if(velocityY > terminalVelocity)
		velocityY = terminalVelocity;

	prospectivePos.y += velocityY*a_dt;

}

void ItemBox::toggleActive(bool is_active){
	active=is_active;
}

void ItemBox::Render(DXFrame& DXVid) {
	RenInfo tempRen;
	D3DXMATRIX TransMat, RotMat, locTransMat;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;

	
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, rot);
	D3DXMatrixTranslation(&locTransMat, 0, 0.25f, 0);
	D3DXMatrixMultiply(&a_box.matrix, &RotMat, &locTransMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&a_box.matrix, &a_box.matrix, &TransMat);
	tempRen.asset = &a_box;
	DXVid.addRen(tempRen);
}



void ItemVec::Init(Map& a_map, ResourceManager& resMan) {
	for(int i = 0; i < MAXITEMS; ++i) {
		active[i] = false;
	}
	//total number of items
	numItems = 0;
	
	//item object settings
	itemMat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	itemMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	itemMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Emissive color reflected
	itemMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// Specular
	itemMat.Power = 0.0f;
	itemObj.mat = &itemMat;
	itemObj.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	itemObj.primInfo = resMan.loadPrim("ItemBox",0.25f,0.25f,0.25f);
	D3DXMatrixIdentity(&itemObj.matrix);
}


void ItemVec::Update(inputState& a_input, double a_dt, Limbase part_list) {
	for(int i = 0; i < MAXITEMS; ++i) {
		if(active[i]) {
			a_itembox[i].Update(a_dt);
		}
	}
}


void ItemVec::Render(DXFrame& DXVid) {

	for(int i = 0; i < MAXITEMS; ++i) {
		if(active[i]) {
			a_itembox[i].Render(DXVid);
		}
	}
}


bool ItemVec::GetActive(int a_index) {
	return active[a_index];
}


void ItemVec::ActivateAItem(Map& a_map) {
	int random = rand() % a_map.numSpawn();
	for(int i = 0; i < MAXITEMS; ++i) {
		if(!active[i]) {
			

			a_itembox[i].ItemBoxInit(a_map.GetSpawn(random),itemObj);
			active[i] = true;
			++numItems;
			return;
		}
	}
}


void ItemVec::DeactivateAItem(int a_index) {
	active[a_index] = false;
	--numItems;
}


ItemBox& ItemVec::GetItem(int a_index) {
	return a_itembox[a_index];
}


int ItemVec::GetNumItems() {
	return numItems;
}



ItemVec::ItemVec() {

}


ItemVec::~ItemVec() {

}

