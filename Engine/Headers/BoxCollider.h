#include "Collider.h"

namespace Engine
{
	class BoxCollider final : public Collider
	{
	public:
		explicit BoxCollider(const wchar_t* name, const Vector2& scale, const Vector2& offset);
	private:
		virtual ~BoxCollider() = default;

	public:
		// Collider��(��) ���� ��ӵ�
		bool IsCollide(Collider* other) override;

	public:
		Property<Vector2> scale;
		Property<Vector2> offset;

	private:
		Vector2 _scale;
		Vector2 _offset;
	};
}