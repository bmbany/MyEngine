#pragma once

namespace Engine
{
	struct WVP
	{
		Matrix world;
		Matrix view;
		Matrix projection;
		Matrix WVP;
	};

	struct LightDesc
	{
		Vector3 direction;
		float padding;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
	};
}