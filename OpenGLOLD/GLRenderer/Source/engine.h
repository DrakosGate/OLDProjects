#pragma once

#include "graphics.h"

#include "gameobject.h"
#include "model.h"

class Engine
{
//Member functions
public:
	~Engine( );

	//Singleton
	static Engine* GetInstance( );

	//Initialisation
	bool Initialise( );
	bool InitialiseGraphics( HWND _hWnd );

	void Run( );
	Graphics* GetGraphics( ) const;
	void Release( );

private:
	Engine( );

//Member variables
private:
	static Engine* m_sEngine;
	Graphics* m_pGraphics;

	GameObject test;
	Model testModel;
};