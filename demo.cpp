#include <format>
#include <thread>
#include <iostream>
#include <iterator>

#include <DXWindow.hpp>

// inherited from DXWindow
#include <FIO/Timer.hpp>
#include <FIO/SpinLock.hpp>

class demo
	: public DXWindow
{
	float                          hue;
	Brush                          brush;
	PathGeometry                   geometry;

	static inline FIO::SpinLock    print_lock;
	static inline const FIO::Timer print_timer;

public:
	demo()
		: DXWindow(L"DXWindow", L"Window", 1024, 768),
		hue(0),
		brush(*this, Color::Black),
		geometry(*this, FillModes::Alternate)
	{
		geometry.AddLine(0,   0);   // TL
		geometry.AddLine(100, 0);   // TR
		geometry.AddLine(100, 100); // BR
		geometry.AddLine(0,   100); // BL
		geometry.AddLine(0,   0);   // TL
		geometry.Finalize();
	}

protected:
	virtual bool OnPoll() override
	{
		if ((hue += 0.001f) > 1.0f)
			hue = 0;

		if (!brush.SetColor(Color::FromHSL(hue, 1.0f, 0.5f)))
			return false;

		return true;
	}

	virtual bool OnDraw() override
	{
		if (!DrawSolidGeometry(100, 100, brush, geometry))
			return false;

		if (!DrawSolidRectangle(400, 400, brush, 100, 100))
			return false;

		return true;
	}
	virtual bool OnDrawGUI() override
	{
		ImGui::ShowDemoWindow();

		return true;
	}

	virtual bool OnDropFile(const Vector2F& position, std::wstring_view path) override
	{
		Print(L"OnDropFile([X:{}, Y:{}], L\"{}\")", position.X, position.Y, path);

		return true;
	}

	virtual bool OnMouseMove(const Vector2F& position) override
	{
		Print(L"OnMouseMove([X:{}, Y:{}])", position.X, position.Y);

		return true;
	}
	virtual bool OnMouseScroll(const Vector2F& position, int delta) override
	{
		Print(L"OnMouseScroll([X:{}, Y:{}], {})", position.X, position.Y, delta);

		return true;
	}
	virtual bool OnMouseButtonUp(const Vector2F& position, MouseButtons button) override
	{
		Print(L"OnMouseButtonUp([X:{}, Y:{}], {})", position.X, position.Y, (int)button);

		return true;
	}
	virtual bool OnMouseButtonDown(const Vector2F& position, MouseButtons button) override
	{
		Print(L"OnMouseButtonDown([X:{}, Y:{}], {})", position.X, position.Y, (int)button);

		return true;
	}

	virtual bool OnKeyboardChar(wchar_t value) override
	{
		Print(L"OnKeyboardChar(L'{}')", value);

		return true;
	}
	virtual bool OnKeyboardKeyUp(Keys key) override
	{
		Print(L"OnKeyboardKeyUp({})", (int)key);

		return true;
	}
	virtual bool OnKeyboardKeyDown(Keys key) override
	{
		Print(L"OnKeyboardKeyDown({})", (int)key);

		return true;
	}

	virtual bool OnResolutionChanged(uint32_t width, uint32_t height) override
	{
		Print(L"OnResolutionChanged({}, {})", width, height);

		return true;
	}

private:
	template<typename ... TArgs>
	static void Print(std::wstring_view format, TArgs ... args)
	{
		std::ostreambuf_iterator<wchar_t> it(std::wcout);

		FIO::SpinLockGuard lock(print_lock);

		std::format_to(it, L"[{:.6f}] ", print_timer.GetElapsed().ToMicroseconds() / 1000000.0f);
		std::wcout << '[' << std::this_thread::get_id() << "] ";

		if constexpr (sizeof...(TArgs) == 0)
			std::wcout << format;
		else
			std::vformat_to(it, format, std::make_wformat_args(args ...));

		std::wcout << std::endl;
	}
};

int main(int argc, char* argv[])
{
	demo d;

	while (d.IsOpen() && d.Poll() && d.Draw())
		;

	return 0;
}
