#include "DXFrame.h"
#include "ResDefs.h"
#include <sstream>

DXFrame::DXFrame()
{
	//window stuff
	m_hWnd = 0;
	m_hInts	= 0;
	m_bVsync = false;
	m_bFullScreen = false;
	m_bLost = false;
	m_bSplitScreen = false;
	m_vSplit = true;
	showFPS = true;
	w_width = 0;
	w_height = 0;

	fps = 0;
	waitTime = 0;
	dt = 0;
	ltime = 0;
	cTime = 0;
	framCount = 0;
	numViewPorts = 1;

	renVec.setSize(50);

	//DX stuff
	m_pD3DObject = 0;
	m_pD3DDevice = 0;

	//font handler
	m_pD3DFont = 0;
	//2D object handler
	m_pD3DSprite = 0;
	//line
	m_pD3DLine = 0;

	viewPorts = new view[numViewPorts];
}

DXFrame::~DXFrame() {
	Shutdown();
}

void DXFrame::init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed)
{
	m_hWnd = hWnd;
	m_bFullScreen = bWindowed;
	//create d3d object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	//find height and width of window
	RECT rect;
	GetWindowRect(hWnd, &rect);
	w_width = rect.right - rect.left;
	w_height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= w_height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= w_width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else {
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	if(m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice)==D3D_OK) {			// returned device pointer
		// Create a Font Object
		D3DXCreateFont(m_pD3DDevice,30,0,FW_BOLD,0,false,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"),&m_pD3DFont);

		// Create Sprite Object and Textures
		D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

		//create line object
		D3DXCreateLine(m_pD3DDevice, &m_pD3DLine);

		m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));
		m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pD3DDevice->GetViewport(&defaultView);
		m_bLost = false;
		m_bSplitScreen = false;
		viewPorts[0].viewPort = defaultView;
		hLine[0] = D3DXVECTOR2(0,w_height/2);
		hLine[1] = D3DXVECTOR2(w_width,w_height/2);
		vLine[0] = D3DXVECTOR2(w_width/2,0);
		vLine[1] = D3DXVECTOR2(w_width/2,w_height);
	}
	else
		m_bLost = true;
	
}

void DXFrame::rotateCam(cam& camr, float dist, float rot, float angle) {
	D3DXMATRIX total;
	D3DXMATRIX temp;
	D3DXVECTOR3 out;
	D3DXVECTOR3 scal;
	D3DXQUATERNION r;
	D3DXMatrixIdentity(&total);
	temp = total;
	D3DXMatrixTranslation(&total,0,0,-dist);
	D3DXMatrixRotationYawPitchRoll(&temp,D3DXToRadian(rot),D3DXToRadian(angle),0);
	total *= temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixTranslation(&temp,camr.cam_look_pos.x,camr.cam_look_pos.y,camr.cam_look_pos.z);
	total *= temp;
	D3DXMatrixDecompose(&scal,&r,&out,&total);
	camr.cam_pos = out;
}

bool DXFrame::setCam(int camNum,cam* camInfo) {
	camNum -= 1;
	if(camNum < numViewPorts) {
		viewPorts[camNum].camera.cam_look_pos = camInfo->cam_look_pos;
		viewPorts[camNum].camera.cam_pos = camInfo->cam_pos;
		viewPorts[camNum].camera.cam_up_vec = camInfo->cam_up_vec;
		viewPorts[camNum].camera.drawDist = camInfo->drawDist;
		viewPorts[camNum].camera.fov_deg = camInfo->fov_deg;
		return true;
	}
	return false;
}

void DXFrame::resetDev(HWND& hWnd,HINSTANCE& hInsts) {
	reSize(hWnd,hInsts,m_bFullScreen);
}

void DXFrame::reSize(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	IDirect3DDevice9* tempDevice = 0;
	m_bFullScreen = bWindowed;
	m_hWnd = hWnd;
	//get window size
	RECT rect;
	GetWindowRect(hWnd, &rect);
	w_width = rect.right - rect.left;
	w_height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	D3DPRESENT_PARAMETERS D3Dpp;
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= w_height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= w_width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);
	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else {
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&tempDevice);			// returned device pointer	
	//delete old device stuff and replace it with new stuff
	if(tempDevice) {
		m_pD3DLine->Release();
		m_pD3DLine = 0;
		m_pD3DSprite->Release();
		m_pD3DSprite = 0;
		m_pD3DFont->Release();
		m_pD3DSprite = 0;
		m_pD3DDevice->Release();
		m_pD3DDevice = tempDevice;

		// Create a Font Object
		D3DXCreateFont(m_pD3DDevice,0.1,0.1,FW_THIN,0,false,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE, TEXT("times.ttf"),&m_pD3DFont);

		// Create Sprite Object and Textures
		D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

		//create line object
		D3DXCreateLine(m_pD3DDevice,&m_pD3DLine);

		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE );
		m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));
		m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_bLost = false;

		m_pD3DDevice->GetViewport(&defaultView);
		m_pD3DDevice->SetViewport(&defaultView);
		setViewCount(numViewPorts);
		hLine[0] = D3DXVECTOR2(0,w_height/2);
		hLine[1] = D3DXVECTOR2(w_width,w_height/2);
		vLine[0] = D3DXVECTOR2(w_width/2,0);
		vLine[1] = D3DXVECTOR2(w_width/2,w_height);
	}
	else
		m_bLost = true;
}

void DXFrame::Shutdown()
{
	//line
	if(m_pD3DLine != NULL) {
		m_pD3DLine->Release();
		m_pD3DLine = NULL;
	}
	//Sprite
	if(m_pD3DSprite != NULL) {
		m_pD3DSprite->Release();
		m_pD3DSprite = NULL;
	}
	// Font
	if(m_pD3DFont != NULL) {
		m_pD3DFont->Release();
		m_pD3DFont = NULL;
	}
	// 3DDevice	
	if(m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	// 3DObject
	if(m_pD3DObject != NULL) {
		m_pD3DObject->Release();
		m_pD3DObject = NULL;
	}
}

void DXFrame::addRen(RenInfo &render) {
	renVec.add(render);
}

void DXFrame::clearRen() {
	renVec.clear();
}

void DXFrame::setSSvSplit(bool vSplit) {
	m_vSplit = vSplit;
	setViewCount(numViewPorts);
}

void DXFrame::toggleSS() {
	m_bSplitScreen = !m_bSplitScreen;
	if(!m_bSplitScreen)
		m_pD3DDevice->SetViewport(&defaultView);
}

void DXFrame::Render() {
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************
	RECT rect,tempRec;
	GetWindowRect(m_hWnd, &rect);
	std::stringstream ss;
	D3DRECT winRect;
	winRect.x1 = rect.left;
	winRect.x2 = rect.right;
	winRect.y1 = rect.top;
	winRect.y2 = rect.bottom;

	rect.right -= rect.left;
	rect.left = 0;
	rect.bottom -= rect.top;
	rect.top = 0;

	if(D3D_OK==(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0))) {
		if(SUCCEEDED(m_pD3DDevice->BeginScene())) {
			D3DXMATRIX TransMat;
			RenInfo* tempRen;
			PrimObj* tempPrim;
			SpriteObj* tempSprite;
			TextStruct* tempText;
			ModelObj* tempMod;
			D3DXMATRIX scale,loc;
			for(int i = 0;i<=numViewPorts;++i) {
				//set viewport
				if(m_bSplitScreen&&i!= numViewPorts)
					m_pD3DDevice->SetViewport(&viewPorts[i].viewPort);
				else
					m_pD3DDevice->SetViewport(&defaultView);
				//set cam
				D3DXMatrixLookAtLH(&TransMat,&viewPorts[i].camera.cam_pos,&viewPorts[i].camera.cam_look_pos,&viewPorts[i].camera.cam_up_vec);
				m_pD3DDevice->SetTransform(D3DTS_VIEW,&TransMat);
				D3DXMatrixPerspectiveFovLH(&TransMat,D3DXToRadian(viewPorts[i].camera.fov_deg),w_width/w_height,1.0f,viewPorts[i].camera.drawDist);
				m_pD3DDevice->SetTransform(D3DTS_PROJECTION,&TransMat);
				//start render
				for(int z = 0;z < renVec.getSize();++z)
				{
					tempRen = renVec.get(z);
					if(tempRen->locCamNum==0 ||tempRen->locCamNum == i+1)
					switch(tempRen->type)
					{
					case primitive:
						tempPrim = (PrimObj*)tempRen->asset;
						m_pD3DDevice->SetStreamSource(0,tempPrim->primInfo->obj, 0, sizeof(Vertex));
						m_pD3DDevice->SetIndices(tempPrim->primInfo->objInd);
						m_pD3DDevice->SetVertexDeclaration(tempPrim->primInfo->objDec);
						//set texture
						m_pD3DDevice->SetTexture(0,tempPrim->Tex->objTex);
						m_pD3DDevice->SetMaterial(tempPrim->mat);
						//set trans
						m_pD3DDevice->SetTransform(D3DTS_WORLD, &tempPrim->matrix);
						//render
						m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,tempPrim->primInfo->numVerts,0,tempPrim->primInfo->numPrim);
						break;
					case model:
						tempMod = (ModelObj*)tempRen->asset;
						m_pD3DDevice->SetTransform(D3DTS_WORLD,&tempMod->matrix);
						for(int i = 0;i < tempMod->mod->numMats;++i) {
							//set texture
							m_pD3DDevice->SetTexture(0,tempMod->mod->textures[i]->objTex);
							m_pD3DDevice->SetMaterial(&tempMod->mod->mats[i].MatD3D);
							//draw model
							tempMod->mod->mesh->DrawSubset(i);
						}
						break;
					case sprite:
						tempSprite = (SpriteObj*)tempRen->asset;
						m_pD3DSprite->Begin(NULL);
						if(tempSprite->posX>1) {
							tempSprite->posX /= w_width;
						}
						if(tempSprite->posY>1) {
							tempSprite->posY /= w_height;
						}
						D3DXMatrixScaling(&scale,tempSprite->scalX,tempSprite->scalY,1);
						D3DXMatrixTranslation(&loc,(tempSprite->posX*viewPorts[i].viewPort.Width)+viewPorts[i].viewPort.X,(tempSprite->posY*viewPorts[i].viewPort.Height)+viewPorts[i].viewPort.Y,1);
						loc = scale*loc;
						//set sprite trans
						m_pD3DSprite->SetTransform(&loc);
						//Draw sprite
						m_pD3DSprite->Draw(tempSprite->image->objTex,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
						m_pD3DSprite->End();
						D3DXMatrixIdentity(&loc);
						D3DXMatrixIdentity(&scale);
						break;
					case text:
						tempText = (TextStruct*)tempRen->asset;
						if(tempText->rec.bottom > 1) {
							tempText->rec.bottom /= w_height;
						}
						if(tempText->rec.left > 1) {
							tempText->rec.left /= w_width;
						}
						if(tempText->rec.right > 1) {
							tempText->rec.right /= w_width;
						}
						if(tempText->rec.top > 1) {
							tempText->rec.top /= w_height;
						}
						//copy
						tempRec.bottom = tempText->rec.bottom*viewPorts[i].viewPort.Height;
						tempRec.top = tempText->rec.top*viewPorts[i].viewPort.Height;
						tempRec.left = tempText->rec.left*viewPorts[i].viewPort.Width;
						tempRec.right = tempText->rec.right*viewPorts[i].viewPort.Width;
						//offset
						tempRec.bottom += viewPorts[i].viewPort.Y;
						tempRec.top += viewPorts[i].viewPort.Y;
						tempRec.left += viewPorts[i].viewPort.X;
						tempRec.right += viewPorts[i].viewPort.X;
						m_pD3DFont->DrawText(0,tempText->text.c_str(),-1,&tempRec,DT_TOP|DT_CENTER|DT_NOCLIP,tempText->textColor);
						break;
					default:
						break;
					}
				}
			}
			//fps counter
			if(m_bSplitScreen)
				m_pD3DDevice->SetViewport(&defaultView);
			switch(numViewPorts) {
			case 2:
				m_pD3DLine->Begin();
				if(m_vSplit)
					m_pD3DLine->Draw(vLine , 2, 0xFFFFFFFF);
				else
					m_pD3DLine->Draw(hLine , 2, 0xFFFFFFFF);
				m_pD3DLine->End();
				break;
			case 4:
				m_pD3DLine->Begin();
				m_pD3DLine->Draw(vLine, 2, 0xFFFFFFFF);
				m_pD3DLine->Draw(hLine, 2, 0xFFFFFFFF);
				m_pD3DLine->End();
				break;
			}
			if(showFPS) 
			{
				ss<<" FPS: "<<fps;
				m_pD3DFont->DrawText(0, ss.str().c_str(), -1, &rect, DT_TOP | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			m_pD3DDevice->EndScene();	
		}
		if(!SUCCEEDED(m_pD3DDevice->Present(NULL,NULL,NULL,NULL)))
			m_bLost = true;
	}
	//update fps counter
	cTime = GetTickCount();
	if(cTime-ltime >= 1000.0f)
	{
		fps = (int)framCount;
		framCount = 0;
		ltime = cTime;
		++waitTime;
	}
	else
	{
		++framCount;
	}
}

//return true if failed
bool DXFrame::setViewCount(int numView) {
	view* tempViews;
	if(numView == 1||numView == 2||numView==4) {
		tempViews = new view[numView];
		for(int i = 0;i<numView&&i<numViewPorts;++i) {
			tempViews[i].camera = viewPorts[i].camera;
		}
		delete [] viewPorts;
		viewPorts = tempViews;
		numViewPorts = numView;
		switch(numViewPorts) {
		case 1:
			viewPorts[0].viewPort.MaxZ = defaultView.MaxZ;
			viewPorts[0].viewPort.MinZ = defaultView.MinZ;
			viewPorts[0].viewPort.X = defaultView.X;
			viewPorts[0].viewPort.Y = defaultView.Y;
			viewPorts[0].viewPort.Width = defaultView.Width;
			viewPorts[0].viewPort.Height = defaultView.Height;
			break;
		case 2:
			if(m_vSplit) {
				viewPorts[0].viewPort.MaxZ = defaultView.MaxZ;
				viewPorts[0].viewPort.MinZ = defaultView.MinZ;
				viewPorts[0].viewPort.X = defaultView.X;
				viewPorts[0].viewPort.Y = defaultView.Y;
				viewPorts[0].viewPort.Width = defaultView.Width/2;
				viewPorts[0].viewPort.Height = defaultView.Height;

				viewPorts[1].viewPort.MaxZ = defaultView.MaxZ;
				viewPorts[1].viewPort.MinZ = defaultView.MinZ;
				viewPorts[1].viewPort.X = defaultView.X+viewPorts[0].viewPort.Width;
				viewPorts[1].viewPort.Y = defaultView.Y;
				viewPorts[1].viewPort.Width = defaultView.Width/2;
				viewPorts[1].viewPort.Height = defaultView.Height;
			} else {
				viewPorts[0].viewPort.MaxZ = defaultView.MaxZ;
				viewPorts[0].viewPort.MinZ = defaultView.MinZ;
				viewPorts[0].viewPort.X = defaultView.X;
				viewPorts[0].viewPort.Y = defaultView.Y;
				viewPorts[0].viewPort.Width = defaultView.Width;
				viewPorts[0].viewPort.Height = defaultView.Height/2;

				viewPorts[1].viewPort.MaxZ = defaultView.MaxZ;
				viewPorts[1].viewPort.MinZ = defaultView.MinZ;
				viewPorts[1].viewPort.X = defaultView.X;
				viewPorts[1].viewPort.Y = defaultView.Y+viewPorts[0].viewPort.Height;
				viewPorts[1].viewPort.Width = defaultView.Width;
				viewPorts[1].viewPort.Height = defaultView.Height/2;
			}
			break;
		case 4:
			viewPorts[0].viewPort = defaultView;
			viewPorts[0].viewPort.X = 0;
			viewPorts[0].viewPort.Y = 0;
			viewPorts[0].viewPort.Width = defaultView.Width/2;
			viewPorts[0].viewPort.Height = defaultView.Height/2;
			
			viewPorts[1].viewPort = defaultView;
			viewPorts[1].viewPort.X = defaultView.Width/2;
			viewPorts[1].viewPort.Y = 0;
			viewPorts[1].viewPort.Width = defaultView.Width/2;
			viewPorts[1].viewPort.Height = defaultView.Height/2;

			viewPorts[2].viewPort = defaultView;
			viewPorts[2].viewPort.X = 0;
			viewPorts[2].viewPort.Y = defaultView.Height/2;
			viewPorts[2].viewPort.Width = defaultView.Width/2;
			viewPorts[2].viewPort.Height = defaultView.Height/2;

			viewPorts[3].viewPort = defaultView;
			viewPorts[3].viewPort.X = defaultView.Width/2;
			viewPorts[3].viewPort.Y = defaultView.Height/2;
			viewPorts[3].viewPort.Width = defaultView.Width/2;
			viewPorts[3].viewPort.Height = defaultView.Height/2;
			break;
		}
		return false;
	}
	return true;
}

void DXFrame::Load2D(LPCSTR FileName,UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,D3DXIMAGE_INFO *SrcInfo,PALETTEENTRY *pPalette,LPDIRECT3DTEXTURE9 *Tex) {
	D3DXCreateTextureFromFileEx(m_pD3DDevice,FileName,Width,Height,MipLevels,Usage,Format,Pool,Filter,MipFilter,ColorKey,SrcInfo,pPalette,Tex);
}

void DXFrame::loadDX(LPCTSTR name,LPD3DXBUFFER* buffer,DWORD& numMat, LPD3DXMESH* mesh) {
	D3DXLoadMeshFromX(name,D3DXMESH_SYSTEMMEM,m_pD3DDevice,buffer,buffer,buffer,&numMat,mesh);
}

void DXFrame::loadXFile(LPCTSTR name,ModelStruct& obj) {

	D3DXLoadMeshFromX(name, // file to load
		D3DXMESH_MANAGED, // flags for memory management
		m_pD3DDevice, // DXD3D device
		&obj.adj, // adjacency buffer for mesh
		&obj.matPoint, // materials in mesh
		0, // effects (such as lighting)
		// similar to those in a .fx file
		&obj.numMats, // count of materials in file
		&obj.mesh); // reference to the mesh object
	obj.textures = new TextureStruct*[obj.numMats];
	obj.mats = (D3DXMATERIAL*)obj.matPoint->GetBufferPointer();
}

void DXFrame::CreateUVCube(PrimStruct& obj,float height,float width,float depth) {
	float h = height/2;
	float w = width/2;
	float d = depth/2;
	obj.bottom = -h;
	obj.top = h;
	obj.left = -w;
	obj.right = w;
	obj.back = -d;
	obj.front = d;
	Vertex m_cubeVerts[24];
	WORD m_cubeIndices[36];
	// Load vertex info, listed per cube face quads
	// back
	m_cubeVerts[0].position = D3DXVECTOR3(-w, -h, -d); 
	m_cubeVerts[1].position = D3DXVECTOR3(-w, h, -d); 
	m_cubeVerts[2].position = D3DXVECTOR3(w, h, -d); 
	m_cubeVerts[3].position = D3DXVECTOR3(w, -h, -d); 
	D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	m_cubeVerts[1].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[2].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[3].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[0].uv = D3DXVECTOR2(0.25f, 0.875f);

	// front 
	m_cubeVerts[4].position = D3DXVECTOR3(-w, -h, d); 
	m_cubeVerts[5].position = D3DXVECTOR3(w, -h, d); 
	m_cubeVerts[6].position = D3DXVECTOR3(w, h, d); 
	m_cubeVerts[7].position = D3DXVECTOR3(-w, h, d); 
	D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	m_cubeVerts[7].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[4].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);

	// top
	m_cubeVerts[8].position = D3DXVECTOR3(-w, h, -d); 
	m_cubeVerts[9].position = D3DXVECTOR3(-w, h, d); 
	m_cubeVerts[10].position = D3DXVECTOR3(w, h, d); 
	m_cubeVerts[11].position = D3DXVECTOR3(w, h, -d); 
	D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f); 
	m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[10].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[11].uv = D3DXVECTOR2(0.25f, 1.0f);

	// bottom
	m_cubeVerts[12].position = D3DXVECTOR3(-w, -h, -d); 
	m_cubeVerts[13].position = D3DXVECTOR3(w, -h, -d); 
	m_cubeVerts[14].position = D3DXVECTOR3(w, -h, d); 
	m_cubeVerts[15].position = D3DXVECTOR3(-w, -h, d); 
	D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_cubeVerts[14].uv = D3DXVECTOR2(0.5f, 1.0f); 
	m_cubeVerts[15].uv = D3DXVECTOR2(0.25f, 1.0f); 
	m_cubeVerts[12].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[13].uv = D3DXVECTOR2(0.5f, 0.875f);

	// Left 
	m_cubeVerts[16].position = D3DXVECTOR3(-w, -h, d); 
	m_cubeVerts[17].position = D3DXVECTOR3(-w, h, d); 
	m_cubeVerts[18].position = D3DXVECTOR3(-w, h, -d); 
	m_cubeVerts[19].position = D3DXVECTOR3(-w, -h, -d); 
	D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[19].uv = D3DXVECTOR2(0.75f, 0.875f); 
	m_cubeVerts[16].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[17].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[18].uv = D3DXVECTOR2(0.75f, 0.0f);

	// Right 
	m_cubeVerts[20].position = D3DXVECTOR3(w, -h, -d); 
	m_cubeVerts[21].position = D3DXVECTOR3(w, h, -d); 
	m_cubeVerts[22].position = D3DXVECTOR3(w, h, d); 
	m_cubeVerts[23].position = D3DXVECTOR3(w, -h, d); 
	D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[21].uv = D3DXVECTOR2(0.75f, 0.0f); 
	m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f); 
	m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 0.875f); 
	m_cubeVerts[20].uv = D3DXVECTOR2(0.75f, 0.875f);

	// Load index info, refers into index into verts array to compose triangles 
	// Note: A clockwise winding order of verts will show the front face.

	// Front 
	m_cubeIndices[0] = 0; m_cubeIndices[1] = 1; m_cubeIndices[2] = 2; // Triangle 0 
	m_cubeIndices[3] = 0; m_cubeIndices[4] = 2; m_cubeIndices[5] = 3; // Triangle 1

	// Back 
	m_cubeIndices[6] = 4; m_cubeIndices[7] = 5; m_cubeIndices[8] = 6; // Triangle 2 
	m_cubeIndices[9] = 4; m_cubeIndices[10] = 6; m_cubeIndices[11] = 7; // Triangle 3

	// Top 
	m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10; // Triangle 4 
	m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11; // Triangle 5

	// Bottom 
	m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14; // Triangle 6 
	m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15; // Triangle 7

	// Left 
	m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18; // Triangle 8 
	m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19; // Triangle 9

	// Right 
	m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22; // Triangle 10 
	m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23; // Triangle 11
	//create decloration
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pD3DDevice->CreateVertexDeclaration(decl, &obj.objDec);
	//create memory
	m_pD3DDevice->CreateVertexBuffer(24*sizeof(Vertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&obj.obj,0);
	m_pD3DDevice->CreateIndexBuffer(36*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&obj.objInd,0);
	//copy info to memory
	VOID* pVertices; VOID* pInd;
	obj.obj->Lock(0,0,&pVertices,0);
	memcpy(pVertices,m_cubeVerts,24*sizeof(Vertex));
	obj.obj->Unlock();

	obj.objInd->Lock(0,0,&pInd,0);
	memcpy(pInd,m_cubeIndices,36*sizeof(WORD));
	obj.objInd->Unlock();

	obj.numVerts = 24;
	obj.numPrim = 12;
}


void DXFrame::uncenterdUVCube(PrimStruct& obj,float bottom,float top,float left,float right,float front, float back) {
	obj.bottom = bottom;
	obj.top = top;
	obj.left = left;
	obj.right = right;
	obj.back = back;
	obj.front = front;
	Vertex m_cubeVerts[24];
	WORD m_cubeIndices[36];
	// Load vertex info, listed per cube face quads
	// back
	m_cubeVerts[0].position = D3DXVECTOR3(left, bottom, back); 
	m_cubeVerts[1].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[2].position = D3DXVECTOR3(right, top, back); 
	m_cubeVerts[3].position = D3DXVECTOR3(right, bottom, back); 
	D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	m_cubeVerts[1].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[2].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[3].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[0].uv = D3DXVECTOR2(0.25f, 0.875f);

	// front 
	m_cubeVerts[4].position = D3DXVECTOR3(left, bottom, front); 
	m_cubeVerts[5].position = D3DXVECTOR3(right, bottom, front); 
	m_cubeVerts[6].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[7].position = D3DXVECTOR3(left, top, front); 
	D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	m_cubeVerts[7].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[4].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);

	// top
	m_cubeVerts[8].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[9].position = D3DXVECTOR3(left, top, front); 
	m_cubeVerts[10].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[11].position = D3DXVECTOR3(right, top, back); 
	D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f); 
	m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[10].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[11].uv = D3DXVECTOR2(0.25f, 1.0f);

	// bottom
	m_cubeVerts[12].position = D3DXVECTOR3(left, bottom, back); 
	m_cubeVerts[13].position = D3DXVECTOR3(right, bottom, back); 
	m_cubeVerts[14].position = D3DXVECTOR3(right, bottom,  front); 
	m_cubeVerts[15].position = D3DXVECTOR3(left, bottom, front); 
	D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_cubeVerts[14].uv = D3DXVECTOR2(0.5f, 1.0f); 
	m_cubeVerts[15].uv = D3DXVECTOR2(0.25f, 1.0f); 
	m_cubeVerts[12].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[13].uv = D3DXVECTOR2(0.5f, 0.875f);

	// Left 
	m_cubeVerts[16].position = D3DXVECTOR3(left, bottom, front); 
	m_cubeVerts[17].position = D3DXVECTOR3(left, top, front); 
	m_cubeVerts[18].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[19].position = D3DXVECTOR3(left, bottom, back); 
	D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[19].uv = D3DXVECTOR2(0.75f, 0.875f); 
	m_cubeVerts[16].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[17].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[18].uv = D3DXVECTOR2(0.75f, 0.0f);

	// Right 
	m_cubeVerts[20].position = D3DXVECTOR3(right, bottom, back); 
	m_cubeVerts[21].position = D3DXVECTOR3(right, top, back); 
	m_cubeVerts[22].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[23].position = D3DXVECTOR3(right, bottom, front); 
	D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[21].uv = D3DXVECTOR2(0.75f, 0.0f); 
	m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f); 
	m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 0.875f); 
	m_cubeVerts[20].uv = D3DXVECTOR2(0.75f, 0.875f);

	// Load index info, refers into index into verts array to compose triangles 
	// Note: A clockwise winding order of verts will show the front face.

	// Front 
	m_cubeIndices[0] = 0; m_cubeIndices[1] = 1; m_cubeIndices[2] = 2; // Triangle 0 
	m_cubeIndices[3] = 0; m_cubeIndices[4] = 2; m_cubeIndices[5] = 3; // Triangle 1

	// Back 
	m_cubeIndices[6] = 4; m_cubeIndices[7] = 5; m_cubeIndices[8] = 6; // Triangle 2 
	m_cubeIndices[9] = 4; m_cubeIndices[10] = 6; m_cubeIndices[11] = 7; // Triangle 3

	// Top 
	m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10; // Triangle 4 
	m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11; // Triangle 5

	// Bottom 
	m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14; // Triangle 6 
	m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15; // Triangle 7

	// Left 
	m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18; // Triangle 8 
	m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19; // Triangle 9

	// Right 
	m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22; // Triangle 10 
	m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23; // Triangle 11
	//create decloration
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pD3DDevice->CreateVertexDeclaration(decl, &obj.objDec);
	//create memory
	m_pD3DDevice->CreateVertexBuffer(24*sizeof(Vertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&obj.obj,0);
	m_pD3DDevice->CreateIndexBuffer(36*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&obj.objInd,0);
	//copy info to memory
	VOID* pVertices; VOID* pInd;
	obj.obj->Lock(0,0,&pVertices,0);
	memcpy(pVertices,m_cubeVerts,24*sizeof(Vertex));
	obj.obj->Unlock();

	obj.objInd->Lock(0,0,&pInd,0);
	memcpy(pInd,m_cubeIndices,36*sizeof(WORD));
	obj.objInd->Unlock();

	obj.numVerts = 24;
	obj.numPrim = 12;
}