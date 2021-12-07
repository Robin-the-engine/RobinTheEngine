#include <iostream>
#include <RTE.h>
#include "RobinTheEngine/EntryPoint.h"
#include "EditorLayer.h"

#include <fstream>

using namespace DirectX;



class EditorApp : public RTE::Application
{
public:
	EditorApp()
	{
		PushLayer(new RTE::EditorLayer());
	}

	~EditorApp()
	{

	}
};


RTE::Application* RTE::CreateApplication()
{
	return new EditorApp();
}
