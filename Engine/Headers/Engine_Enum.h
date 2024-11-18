#pragma once

namespace GameState
{
	enum State { Normal, Error, Game_End };
}

enum class ShaderType { Vertex, Pixel };
enum class TextureType { Diffuse, Normal, Specular, Emission, Opacity, LightMap, End };
enum class SamplerType { Linear, End };