#include <atomic>
#include <chrono>
#include <format>
#include <thread>
#include <iostream>
#include <iterator>

#include <DXWindow.hpp>

class demo
	: public DXWindow
{
	float                          hue;
	Brush                          brush;
	PathGeometry                   geometry;

	static inline std::atomic_flag print_lock;
	static inline const auto       print_timer = std::chrono::steady_clock::now();

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
		if (!DXWindow::OnPoll())
			return false;

		if ((hue += 0.001f) > 1.0f)
			hue = 0;

		if (!brush.SetColor(Color::FromHSL(hue, 1.0f, 0.5f)))
			return false;

		return true;
	}

	virtual bool OnDraw() override
	{
		if (!DXWindow::OnDraw())
			return false;

		if (!DrawSolidGeometry(100, 100, brush, geometry))
			return false;

		if (!DrawSolidRectangle(400, 400, brush, 100, 100))
			return false;

		return true;
	}

private:
	template<typename ... TArgs>
	static void Print(std::wstring_view format, TArgs ... args)
	{
		std::ostreambuf_iterator<wchar_t> it(std::wcout);
		auto                              time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - print_timer).count();

		while (print_lock.test_and_set(std::memory_order_acquire))
			while (print_lock.test(std::memory_order_relaxed))
				;

		std::format_to(it, L"[{:.6f}] ", time / 1000000.0f);
		std::wcout << '[' << std::this_thread::get_id() << "] ";

		if constexpr (sizeof...(TArgs) == 0)
			std::wcout << format;
		else
			std::vformat_to(it, format, std::make_wformat_args(args ...));

		std::wcout << std::endl;

		print_lock.clear(std::memory_order_release);
	}
};

int main(int argc, char* argv[])
{
	demo d;

	if (d.Open())
		while (d.Poll() && d.Draw())
			;

	return 0;
}
