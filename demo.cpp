#include "DXWindow.hpp"

class demo
	: public DXWindow
{
	Brush        brush;
	PathGeometry geometry;

public:
	demo()
		: DXWindow(L"DXWindow", L"Window", 1024, 768),
		brush(*this, Color::Black),
		geometry(*this, FillModes::Winding)
	{
	}

protected:
	virtual bool OnPoll() override
	{
		return true;
	}

	virtual bool OnDraw() override
	{
		return true;
	}
	virtual bool OnDrawGUI() override
	{
		return true;
	}

	virtual bool OnDropFile(const Vector2F& position, std::wstring_view path) override
	{
		return true;
	}

	virtual bool OnMouseMove(const Vector2F& position) override
	{
		return true;
	}
	virtual bool OnMouseScroll(const Vector2F& position, int delta) override
	{
		return true;
	}
	virtual bool OnMouseButtonUp(const Vector2F& position, MouseButtons button) override
	{
		return true;
	}
	virtual bool OnMouseButtonDown(const Vector2F& position, MouseButtons button) override
	{
		return true;
	}

	virtual bool OnKeyboardChar(wchar_t value) override
	{
		return true;
	}
	virtual bool OnKeyboardKeyUp(Keys key) override
	{
		return true;
	}
	virtual bool OnKeyboardKeyDown(Keys key) override
	{
		return true;
	}

	virtual bool OnResolutionChanged(uint32_t width, uint32_t height) override
	{
		return true;
	}
};

int main(int argc, char* argv[])
{
	demo d;

	while (d.IsOpen() && d.Poll() && d.Draw())
		;

	return 0;
}
