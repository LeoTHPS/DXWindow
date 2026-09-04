#pragma once
#include <set>
#include <list>
#include <cmath>
#include <stack>
#include <string>
#include <vector>
#include <cstdint>
#include <numbers>
#include <type_traits>

#include <d2d1_1.h>
#include <d3d11_1.h>
#include <dwrite_1.h>
#include <wincodec.h>

#include <dsound.h>
#include <mmsystem.h>

#include <commdlg.h>
#include <Windows.h>

#ifdef DXWINDOW_IMGUI
	#include <imgui.h>
	#include <imgui_impl_dx11.h>
	#include <imgui_impl_win32.h>
#endif

#undef MessageBox

class DXWindow
{
	DXWindow(DXWindow&&) = delete;
	DXWindow(const DXWindow&) = delete;

public:
	enum class FileDialogFlags
	{
		None             = 0,
		ReadOnly         = OFN_READONLY,
		NoChangeDir      = OFN_NOCHANGEDIR,
		CreatePrompt     = OFN_CREATEPROMPT,
		EnableSizing     = OFN_ENABLESIZING,
		HideReadOnly     = OFN_HIDEREADONLY,
		FileMustExist    = OFN_FILEMUSTEXIST,
		PathMustExist    = OFN_PATHMUSTEXIST,
		OverwritePrompt  = OFN_OVERWRITEPROMPT,
		AllowMultiselect = OFN_ALLOWMULTISELECT,
		DontAddToRecent  = OFN_DONTADDTORECENT,
		NoReadOnlyReturn = OFN_NOREADONLYRETURN
	};

	enum class Keys
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		Tilde,

		Alt,
		LAlt,
		RAlt,

		Clear,
		Delete,
		Enter,
		Escape,
		Backspace,

		Control,
		LControl,
		RControl,

		Shift,
		LShift,
		RShift,

		Spacebar,
		Tab,

		Up,
		Down,
		Left,
		Right,

		COUNT
	};

	enum class Icons
	{
		None,

		Error,
		Asterisk,
		Question,
		Application,
		Exclamation,

		UserDefined,

		COUNT
	};

	enum class Cursors
	{
		None,

		No,
		Hand,
		Help,
		Wait,
		Arrow,
		Cross,
		IBeam,
		UpArrow,
		Size,
		SizeX,
		SizeY,
		SizeNWSE,
		SizeNESW,
		AppStarting,

		UserDefined,

		COUNT
	};

	enum class ArcSizes
	{
		Small = D2D1_ARC_SIZE_SMALL,
		Large = D2D1_ARC_SIZE_LARGE
	};

	enum class FillModes
	{
		Winding   = D2D1_FILL_MODE_WINDING,
		Alternate = D2D1_FILL_MODE_ALTERNATE
	};

	enum class BrushTypes
	{
		Bitmap,
		SolidColor,

		COUNT
	};

	enum class MouseButtons
	{
		Left,
		Right,
		Middle,
		X1,
		X2,

		COUNT
	};

	enum class FontStyles
	{
		Normal  = DWRITE_FONT_STYLE_NORMAL,
		Oblique = DWRITE_FONT_STYLE_OBLIQUE,
		Italic  = DWRITE_FONT_STYLE_ITALIC
	};

	enum class FontWeights
	{
		Light  = 300,
		Normal = 400,
		Medium = 500,
		Bold   = 600
	};

	enum class FontStretches
	{
		Normal    = DWRITE_FONT_STRETCH_NORMAL,
		Expanded  = DWRITE_FONT_STRETCH_EXPANDED,
		Condensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED
	};

	enum class TextAlignments
	{
		TopLeft      = (DWRITE_TEXT_ALIGNMENT_LEADING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		TopRight     = (DWRITE_TEXT_ALIGNMENT_TRAILING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		TopCenter    = (DWRITE_TEXT_ALIGNMENT_CENTER << 8) | DWRITE_PARAGRAPH_ALIGNMENT_NEAR,

		BottomLeft   = (DWRITE_TEXT_ALIGNMENT_LEADING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		BottomRight  = (DWRITE_TEXT_ALIGNMENT_TRAILING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		BottomCenter = (DWRITE_TEXT_ALIGNMENT_CENTER << 8) | DWRITE_PARAGRAPH_ALIGNMENT_FAR,

		Center       = (DWRITE_TEXT_ALIGNMENT_CENTER << 8) | DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
		CenterLeft   = (DWRITE_TEXT_ALIGNMENT_LEADING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
		CenterRight  = (DWRITE_TEXT_ALIGNMENT_TRAILING << 8) | DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	};

	enum class SweepDirections
	{
		Clockwise        = D2D1_SWEEP_DIRECTION_CLOCKWISE,
		CounterClockwise = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE
	};

	enum class BitmapInterpolationModes
	{
		Linear          = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		NearestNeighbor = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
	};

	enum class MessageBoxIcons
	{
		None,

		Hand        = MB_ICONHAND,
		Stop        = MB_ICONSTOP,
		Error       = MB_ICONERROR,
		Warning     = MB_ICONWARNING,
		Asterisk    = MB_ICONASTERISK,
		Question    = MB_ICONQUESTION,
		Exclamation = MB_ICONEXCLAMATION,
		Information = MB_ICONINFORMATION
	};

	enum class MessageBoxButtons
	{
		Ok                = MB_OK,
		OkCancel          = MB_OKCANCEL,
		Help              = MB_HELP,
		YesNo             = MB_YESNO,
		YesNoCancel       = MB_YESNOCANCEL,
		RetryCancel       = MB_RETRYCANCEL,
		AbortRetryIgnore  = MB_ABORTRETRYIGNORE,
		CancelTryContinue = MB_CANCELTRYCONTINUE
	};

	enum class MessageBoxResults
	{
		Error,

		Ok       = IDOK,
		No       = IDNO,
		Yes      = IDYES,
		Abort    = IDABORT,
		Retry    = IDRETRY,
		Cancel   = IDCANCEL,
		Ignore   = IDIGNORE,
		TryAgain = IDTRYAGAIN,
		Continue = IDCONTINUE
	};

	enum class MessageBoxDefaultButtons
	{
		Ok,
		No,
		Try,
		Yes,
		Help,
		Abort,
		Retry,
		Cancel,
		Ignore,
		Continue
	};

	struct Font
	{
		std::wstring  Name;
		float         Size;
		FontStyles    Style;
		FontWeights   Weight;
		FontStretches Stretch;

		Font();
		Font(std::wstring_view name, float size, FontStyles style = FontStyles::Normal, FontWeights weight = FontWeights::Normal, FontStretches stretch = FontStretches::Normal);
	};

	struct Color
	{
		static const Color Black;
		static const Color White;
		static const Color Gray;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Orange;
		static const Color Yellow;
		static const Color Chartreuse;
		static const Color SpringGreen;
		static const Color Cyan;
		static const Color Azure;
		static const Color Violet;
		static const Color Magenta;
		static const Color Rose;
		static const Color CornflowerBlue;
		static const Color Transparent;

		float A;
		float R;
		float G;
		float B;

		static constexpr Color FromHSL(float h, float s, float l, float a = 1)
		{
			float q     = (l < 0.5f) ? (l * (1 + s)) : (l + s - l * s);
			float p     = 2 * l - q;
			Color color = { .A = a, .R = l, .G = l, .B = l };

			h = std::fmodf(h, 1.0f);

			if (h < 0.0f)
				h += 1.0f;

			if (s > 0.00001f)
			{
				color.R = FromHue(p, q, h + (1.0f / 3.0f));
				color.G = FromHue(p, q, h);
				color.B = FromHue(p, q, h - (1.0f / 3.0f));
			}

			return color;
		}
		static constexpr Color FromHSL(uint8_t h, uint8_t s, uint8_t l, uint8_t a = 0xFF)
		{
			return FromHSL(h / 255.0f, s / 255.0f, l / 255.0f, a / 255.0f);
		}

		static constexpr Color FromARGB(int r, int g, int b, int a = 0xFF)
		{
			return FromARGB((uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);
		}
		static constexpr Color FromARGB(float r, float g, float b, float a = 1)
		{
			return Color { .A = a, .R = r, .G = g, .B = b };
		}
		static constexpr Color FromARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
		{
			return FromARGB(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
		}

		static constexpr Color FromARGB(uint32_t argb)
		{
			return FromARGB((uint8_t)((argb >> 16) & 0xFF), (uint8_t)((argb >> 8) & 0xFF), (uint8_t)(argb & 0xFF), (uint8_t)((argb >> 24) & 0xFF));
		}

		static constexpr Color FromColor(const Color& color, float a = 1)
		{
			return FromARGB(color.R, color.G, color.B, a);
		}
		static constexpr Color FromColor(const Color& color, uint8_t a = 0xFF)
		{
			return FromARGB(color.R, color.G, color.B, a / 255.0f);
		}

		constexpr bool operator == (const Color& value) const
		{
			return (A == value.A) && (R == value.R) && (G == value.G) && (B == value.B);
		}
		constexpr bool operator != (const Color& value) const
		{
			return !operator==(value);
		}

	private:
		static constexpr float FromHue(float p, float q, float t)
		{
			t = std::fmodf(t, 1.0f);

			if (t < 0.0f)
				t += 1.0f;

			if (t < (1.0f / 6.0f))
				return p + (q - p) * 6.0f * t;

			if (t < 0.5f)
				return q;

			if (t < (2.0f / 3.0f))
				return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;

			return p;
		}
	};

	struct TextMetrics
	{
		float Top;
		float Left;
		float Width;
		float WidthIncludingTrailingWhitespace;
		float Height;
		float LineCount;
		float LayoutWidth;
		float LayoutHeight;
	};

	struct FileDialogFilter
	{
		std::wstring_view Key;
		std::wstring_view Value;
	};

	struct FileDialogResult
	{
		std::vector<std::wstring> Paths;
		DWORD                     ErrorCode;
	};

	class TimeSpan
	{
		uint64_t value;

	public:
		typedef uint64_t Type;

		static const TimeSpan Zero;
		static const TimeSpan Infinite;

		static constexpr TimeSpan FromDays(uint64_t value)
		{
			return TimeSpan(value * 86400000000000);
		}
		static constexpr TimeSpan FromHours(uint64_t value)
		{
			return TimeSpan(value * 3600000000000);
		}
		static constexpr TimeSpan FromMinutes(uint64_t value)
		{
			return TimeSpan(value * 60000000000);
		}
		static constexpr TimeSpan FromSeconds(uint64_t value)
		{
			return TimeSpan(value * 1000000000);
		}
		static constexpr TimeSpan FromMilliseconds(uint64_t value)
		{
			return TimeSpan(value * 1000000);
		}
		static constexpr TimeSpan FromMicroseconds(uint64_t value)
		{
			return TimeSpan(value * 1000);
		}
		static constexpr TimeSpan FromNanoseconds(uint64_t value)
		{
			return TimeSpan(value);
		}

		constexpr TimeSpan()
			: TimeSpan(0)
		{
		}
		constexpr TimeSpan(uint64_t nanoseconds)
			: value(nanoseconds)
		{
		}

		constexpr uint64_t ToDays() const
		{
			return value / 86400000000000;
		}
		constexpr uint64_t ToHours() const
		{
			return value / 3600000000000;
		}
		constexpr uint64_t ToMinutes() const
		{
			return value / 60000000000;
		}
		constexpr uint64_t ToSeconds() const
		{
			return value / 1000000000;
		}
		constexpr uint64_t ToMilliseconds() const
		{
			return value / 1000000;
		}
		constexpr uint64_t ToMicroseconds() const
		{
			return value / 1000;
		}
		constexpr uint64_t ToNanoseconds() const
		{
			return value;
		}

		constexpr TimeSpan  operator *  (float value) const
		{
			return TimeSpan::FromNanoseconds(this->value * value);
		}
		constexpr TimeSpan& operator *= (float value)
		{
			this->value *= value;

			return *this;
		}

		constexpr TimeSpan  operator /  (float value) const
		{
			return TimeSpan::FromNanoseconds(this->value / value);
		}
		constexpr TimeSpan& operator /= (float value)
		{
			this->value /= value;

			return *this;
		}

		constexpr TimeSpan& operator += (TimeSpan value)
		{
			this->value += value.value;

			return *this;
		}
		constexpr TimeSpan& operator -= (TimeSpan value)
		{
			this->value -= value.value;

			return *this;
		}
		constexpr TimeSpan& operator *= (TimeSpan value)
		{
			this->value *= value.value;

			return *this;
		}
		constexpr TimeSpan& operator /= (TimeSpan value)
		{
			this->value /= value.value;

			return *this;
		}

		constexpr TimeSpan operator + (TimeSpan value) const
		{
			return TimeSpan(this->value + value.value);
		}
		constexpr TimeSpan operator - (TimeSpan value) const
		{
			return TimeSpan(this->value - value.value);
		}
		constexpr TimeSpan operator * (TimeSpan value) const
		{
			return TimeSpan(this->value * value.value);
		}
		constexpr TimeSpan operator / (TimeSpan value) const
		{
			return TimeSpan(this->value / value.value);
		}

		constexpr bool operator == (TimeSpan value) const
		{
			return this->value == value.value;
		}
		constexpr bool operator != (TimeSpan value) const
		{
			return this->value != value.value;
		}

		constexpr bool operator > (TimeSpan value) const
		{
			return this->value > value.value;
		}
		constexpr bool operator >= (TimeSpan value) const
		{
			return this->value >= value.value;
		}

		constexpr bool operator < (TimeSpan value) const
		{
			return this->value < value.value;
		}
		constexpr bool operator <= (TimeSpan value) const
		{
			return this->value <= value.value;
		}
	};

	template<typename T>
	struct Size
	{
		typedef T Type;

		T Width;
		T Height;

		constexpr bool operator == (const Size<T>& value) const
		{
			return (Width == value.Width) && (Height == value.Height);
		}
		constexpr bool operator != (const Size<T>& value) const
		{
			return !operator==(value);
		}
	};
	typedef Size<float>    SizeF;
	typedef Size<uint32_t> SizeU32;

	template<typename T>
	struct Vector2
	{
		typedef T Type;

		T X;
		T Y;

		constexpr bool  IsParallel(const Vector2<T>& value) const
		{
			return (X == value.X) ^ (Y == value.Y);
		}

		constexpr float GetAngle(const Vector2<T>& value) const
		{
			float angle = atan2((double)value.Y - Y, (double)value.X - X) * (180.0f / std::numbers::pi);

			if (angle < 0)
				angle += 360;

			return angle;
		}

		constexpr T     GetDistance(const Vector2<T>& value) const
		{
			return (X >= value.X ? X - value.X : value.X - X) + (Y >= value.Y ? Y - value.Y : value.Y - Y);
		}

		constexpr bool        operator == (const Vector2<T>& value) const
		{
			return (X == value.X) && (Y == value.Y);
		}
		constexpr bool        operator != (const Vector2<T>& value) const
		{
			return !operator==(value);
		}

		constexpr Vector2<T>  operator + (const Vector2<T>& value) const
		{
			return { .X = X + value.X, .Y = Y + value.Y };
		}
		constexpr Vector2<T>  operator - (const Vector2<T>& value) const
		{
			return { .X = X - value.X, .Y = Y - value.Y };
		}

		constexpr Vector2<T>& operator += (const Vector2<T>& value)
		{
			X += value.X;
			Y += value.Y;

			return *this;
		}
		constexpr Vector2<T>& operator -= (const Vector2<T>& value)
		{
			X -= value.X;
			Y -= value.Y;

			return *this;
		}
	};
	typedef Vector2<float> Vector2F;

	template<typename T>
	struct Rectangle
	{
		typedef T Type;

		T Left;
		T Right;
		T Top;
		T Bottom;

		constexpr T    GetWidth() const
		{
			return Right - Left;
		}
		constexpr T    GetHeight() const
		{
			return Bottom - Top;
		}

		constexpr T    GetArea() const
		{
			return GetWidth() * GetHeight();
		}
		constexpr T    GetPerimeter() const
		{
			return (GetWidth() * 2) * (GetHeight() * 2);
		}

		constexpr bool Contains(T x, T y) const
		{
			if ((x < Left) || (x > Right))
				return false;

			if ((y < Top) || (y > Bottom))
				return false;

			return true;
		}

		constexpr bool Intersects(const Rectangle<T>& value) const
		{
			return Intersects(value.Left, value.Right, value.Top, value.Bottom);
		}
		constexpr bool Intersects(T left, T right, T top, T bottom) const
		{
			if ((Right < left) || (Left > right))
				return false;

			if ((Bottom < top) || (Top > bottom))
				return false;

			return true;
		}

		constexpr bool operator == (const Rectangle<T>& value) const
		{
			return (Left == value.Left) && (Right == value.Right) && (Top == value.Top) && (Bottom == value.Bottom);
		}
		constexpr bool operator != (const Rectangle<T>& value) const
		{
			return !operator==(value);
		}
	};
	typedef Rectangle<float> RectangleF;

	class Bitmap;

	class IResource
	{
	public:
		virtual bool IsLoaded() const = 0;

		virtual bool IsReloadable() const = 0;

		virtual bool Load() = 0;
		virtual void Unload() = 0;
	};
	class Brush
		: public IResource
	{
		BrushTypes    type;
		ID2D1Brush*   brush;
		Color         color;
		const Bitmap* bitmap;
		DXWindow*     window;

	public:
		Brush();

		Brush(Brush&& brush);
		Brush(const Brush& brush);

		Brush(DXWindow& window, const Color& color);

		Brush(DXWindow& window, const Bitmap& bitmap);

		virtual ~Brush();

		virtual   bool  IsLoaded() const override
		{
			return brush != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return true;
		}

		constexpr auto  GetHandle() const
		{
			return brush;
		}

		constexpr auto  GetType() const
		{
			return type;
		}

		constexpr auto& GetColor() const
		{
			return color;
		}

		constexpr auto& GetBitmap() const
		{
			return *bitmap;
		}

		constexpr float GetOpacity() const
		{
			return color.A;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		bool SetColor(const Color& value);

		bool SetOpacity(float value);

		virtual bool Load() override;
		virtual void Unload() override;

		Brush& operator = (Brush&& brush);
		Brush& operator = (const Brush& brush);
	};
	class Sound
		: public IResource
	{
#pragma pack(push, 1)
		struct WaveHeader
		{
			char     ChunkID[4];     // RIFF
			uint32_t ChunkSize;      // 
			char     Format[4];      // WAVE

			char     SubChunk1Id[4]; // fmt | data
			uint32_t SubChunk1Size;  // PCM == 16
			uint16_t AudioFormat;    // PCM == 1
			uint16_t NumChannels;    // Mono == 1 | Stereo = 2
			uint32_t SampleRate;     // 44100
			uint32_t ByteRate;       // SampleRate * NumChannels * BitsPerSample / 8
			uint16_t BlockAlign;     // NumChannels * BitsPerSample / 8
			uint16_t BitsPerSample;  // 8/16/32/64

			char     SubChunk2Id[4]; // data
			uint32_t SubChunk2Size;  // NumSamples * NumChannels * BitsPerSample / 8
		};
#pragma pack(pop)

		enum class Types
		{
			File,
			Memory
		};

		bool                 is_paused;
		bool                 is_playing;
		bool                 is_repeating;

		Types                type;
		std::wstring         path;
		IDirectSoundBuffer*  sound;
		std::string_view     buffer;
		DXWindow*            window;
		float                volume;
		DWORD                position;

	public:
		Sound();

		Sound(Sound&& sound);
		Sound(const Sound& sound);

		Sound(DXWindow& window, std::wstring_view path);
		Sound(DXWindow& window, const void* buffer, size_t size);

		virtual ~Sound();

		virtual   bool  IsLoaded() const override
		{
			return sound != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return false;
		}

		constexpr bool  IsPaused() const
		{
			return is_paused;
		}

		constexpr bool  IsPlaying() const
		{
			return is_playing;
		}

		constexpr bool  IsRepeating() const
		{
			return is_repeating;
		}

		constexpr auto  GetHandle() const
		{
			return sound;
		}

		constexpr auto  GetVolume() const
		{
			return volume;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		bool SetVolume(float value);

		bool Play(bool repeat = false);

		bool Pause();

		bool Stop();

		virtual bool Load() override;
		virtual void Unload() override;

		Sound& operator = (Sound&& sound);
		Sound& operator = (const Sound& sound);

	private:
		bool LoadWave(const void* buffer, size_t size);
	};
	class Bitmap
		: public IResource
	{
		enum class Types
		{
			File,
			Memory
		};

		Types                    type;
		std::wstring             path;
		std::string_view         buffer;
		ID2D1Bitmap1*            bitmap;
		DXWindow*                window;
		SizeU32                  pixel_size;
		SizeU32                  bitmap_size;
		std::vector<Color>       transparency_keys;
		BitmapInterpolationModes interpolation_mode;

	public:
		Bitmap();

		Bitmap(Bitmap&& bitmap);
		Bitmap(const Bitmap& bitmap);

		Bitmap(DXWindow& window, std::wstring_view path);
		Bitmap(DXWindow& window, const void* buffer, size_t size);

		virtual ~Bitmap();

		virtual   bool  IsLoaded() const override
		{
			return bitmap != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return true;
		}

		constexpr auto  GetHandle() const
		{
			return bitmap;
		}

		constexpr auto& GetPath() const
		{
			return path;
		}

		constexpr auto& GetSize() const
		{
			return bitmap_size;
		}

		constexpr auto& GetBuffer() const
		{
			return buffer;
		}

		constexpr auto& GetPixelSize() const
		{
			return pixel_size;
		}

		constexpr auto  GetTransparencyKeys() const
		{
			return transparency_keys.data();
		}

		constexpr auto  GetInterpolationMode() const
		{
			return interpolation_mode;
		}

		constexpr auto  GetTransparencyKeyCount() const
		{
			return transparency_keys.size();
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		bool AddTransparencyKey(const Color& value);
		bool RemoveTransparencyKey(const Color& value);

		void SetInterpolationMode(BitmapInterpolationModes value);

		virtual bool Load() override;
		virtual void Unload() override;

		Bitmap& operator = (Bitmap&& bitmap);
		Bitmap& operator = (const Bitmap& bitmap);
	};
	class TextFormat
		: public IResource
	{
		Font               font;
		IDWriteTextFormat* format;
		DXWindow*          window;
		TextAlignments     alignment;
		bool               word_wrapping;

	public:
		TextFormat();

		TextFormat(TextFormat&& format);
		TextFormat(const TextFormat& format);

		TextFormat(DXWindow& window, Font&& font, TextAlignments alignment, bool word_wrapping);
		TextFormat(DXWindow& window, const Font& font, TextAlignments alignment, bool word_wrapping)
			: TextFormat(window, Font(font), alignment, word_wrapping)
		{
		}

		virtual ~TextFormat();

		virtual   bool  IsLoaded() const override
		{
			return format != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return false;
		}

		constexpr auto  GetHandle() const
		{
			return format;
		}

		constexpr auto& GetFont() const
		{
			return font;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		constexpr auto  GetAlignment() const
		{
			return alignment;
		}

		constexpr bool  GetWordWrapping() const
		{
			return word_wrapping;
		}

		bool SetFont(Font&& value);
		bool SetFont(const Font& value)
		{
			return SetFont(Font(value));
		}

		bool SetFontSize(float value);

		bool SetAlignment(TextAlignments value);

		bool SetWordWrapping(bool value);

		bool CalculateMetrics(TextMetrics& value, std::wstring_view string) const;

		virtual bool Load() override;
		virtual void Unload() override;

		TextFormat& operator = (TextFormat&& format);
		TextFormat& operator = (const TextFormat& format);
	};
	class TextLayout
		: public IResource
	{
		SizeF              size;
		const TextFormat*  format;
		IDWriteTextLayout* layout;
		std::wstring       string;
		DXWindow*          window;
		TextMetrics        metrics;
		bool               clipping;
		TextAlignments     alignment;
		bool               word_wrapping;

	public:
		TextLayout();

		TextLayout(TextLayout&& layout);
		TextLayout(const TextLayout& layout);

		TextLayout(DXWindow& window, const TextFormat& format, std::wstring_view string, float width, float height);

		virtual ~TextLayout();

		virtual   bool  IsLoaded() const override
		{
			return layout != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return false;
		}

		constexpr auto& GetSize() const
		{
			return size;
		}

		constexpr auto  GetHandle() const
		{
			return layout;
		}

		constexpr auto& GetFormat() const
		{
			return *format;
		}

		constexpr auto& GetString() const
		{
			return string;
		}

		constexpr auto& GetMetrics() const
		{
			return metrics;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		constexpr auto  GetClipping() const
		{
			return clipping;
		}

		constexpr auto  GetAlignment() const
		{
			return alignment;
		}

		constexpr bool  GetWordWrapping() const
		{
			return word_wrapping;
		}

		bool SetSize(float width, float height);

		bool SetFormat(const TextFormat& value);

		bool SetString(std::wstring_view value);

		void SetClipping(bool value);

		bool SetAlignment(TextAlignments value);

		bool SetWordWrapping(bool value);

		virtual bool Load() override;
		virtual void Unload() override;

		TextLayout& operator = (TextLayout&& layout);
		TextLayout& operator = (const TextLayout& layout);

	private:
		static bool CalculateMetrics(TextMetrics& value, IDWriteTextLayout* layout);
	};
	class PathGeometry
		: public IResource
	{
		DXWindow*          window;
		ID2D1PathGeometry* geometry;
		ID2D1GeometrySink* geometry_sink;
		FillModes          geometry_fill_mode;

		PathGeometry(const PathGeometry&) = delete;

	public:
		PathGeometry();

		PathGeometry(PathGeometry&& geometry);

		PathGeometry(DXWindow& window, FillModes fill_mode);

		virtual ~PathGeometry();

		virtual   bool  IsLoaded() const override
		{
			return geometry != nullptr;
		}

		constexpr bool  IsFinalized() const
		{
			return geometry_sink == nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return false;
		}

		constexpr auto  GetHandle() const
		{
			return geometry;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		constexpr auto  GetFillMode() const
		{
			return geometry_fill_mode;
		}

		bool AddArc(float x, float y, float width, float height, float angle, SweepDirections direction, ArcSizes size);
		bool AddLine(float x, float y);
		bool AddLines(const Vector2F* points, size_t count);
		bool AddBezier(const Vector2F& point1, const Vector2F& point2, const Vector2F& point3);
		bool AddQBezier(const Vector2F& point1, const Vector2F& point2);

		bool Finalize();

		virtual bool Load() override;
		virtual void Unload() override;

		PathGeometry& operator = (PathGeometry&& geometry);
	};
	class Texture2D
		: public IResource
	{
		const Bitmap*    bitmap;
		DXWindow*        window;
		ID3D11Texture2D* texture;

	public:
		Texture2D();

		Texture2D(Texture2D&& texture);
		Texture2D(const Texture2D& texture);

		Texture2D(DXWindow& window, const Bitmap& bitmap);

		virtual ~Texture2D();

		virtual   bool  IsLoaded() const override
		{
			return texture != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return true;
		}

		constexpr auto& GetBitmap() const
		{
			return *bitmap;
		}

		constexpr auto  GetHandle() const
		{
			return texture;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		virtual bool Load() override;
		virtual void Unload() override;

		Texture2D& operator = (Texture2D&& texture);
		Texture2D& operator = (const Texture2D& texture);
	};
	class ShaderResourceView
		: public IResource
	{
		ID3D11ShaderResourceView* view;
		DXWindow*                 window;
		const Texture2D*          texture;

	public:
		ShaderResourceView();

		ShaderResourceView(ShaderResourceView&& view);
		ShaderResourceView(const ShaderResourceView& view);

		ShaderResourceView(DXWindow& window, const Texture2D& texture);

		virtual ~ShaderResourceView();

		virtual   bool  IsLoaded() const override
		{
			return view != nullptr;
		}

		virtual   bool  IsReloadable() const override
		{
			return true;
		}

		constexpr auto  GetHandle() const
		{
			return view;
		}

		constexpr auto& GetWindow()
		{
			return *window;
		}
		constexpr auto& GetWindow() const
		{
			return *(const DXWindow*)window;
		}

		constexpr auto& GetTexture() const
		{
			return *texture;
		}

		virtual bool Load() override;
		virtual void Unload() override;

		ShaderResourceView& operator = (ShaderResourceView&& view);
		ShaderResourceView& operator = (const ShaderResourceView& view);
	};

	class Transform
	{
		D2D1::Matrix3x2F matrix;

	public:
		static Transform Skew(float x, float y, const Vector2F& center = {});

		static Transform Scale(float x, float y, const Vector2F& center = {});
		static Transform Scale(const SizeF& size, const Vector2F& center = {});

		static Transform Identity();

		static Transform Rotation(float angle, const Vector2F& center = {});

		static Transform Translation(float x, float y);
		static Transform Translation(const SizeF& size);

		Transform();
		Transform(Transform&& transform);
		Transform(const Transform& transform);
		Transform(const D2D1::Matrix3x2F& matrix);
		Transform(const D2D1_MATRIX_3X2_F& matrix);

		virtual ~Transform();

		bool IsIdentity() const;

		bool IsInvertible() const;

		float Determinant() const;

		bool Invert();

		void SetProduct(const Transform& a, const Transform& b);

		Vector2F TransformPoint(const Vector2F& value) const;

		operator const D2D1::Matrix3x2F& () const;

		Transform operator * (const Transform& value) const;

		Transform& operator = (Transform&& transform);
		Transform& operator = (const Transform& transform);
		Transform& operator = (const D2D1::Matrix3x2F& matrix);
		Transform& operator = (const D2D1_MATRIX_3X2_F& matrix);

		bool operator == (const Transform& transform) const;
		bool operator != (const Transform& transform) const;
	};

	class MouseState
	{
		friend DXWindow;

		Vector2F position                                                    = {};
		bool     buttons_down[static_cast<size_t>(MouseButtons::COUNT)]      = {};
		bool     buttons_down_prev[static_cast<size_t>(MouseButtons::COUNT)] = {};

	public:
		constexpr auto& GetPosition() const
		{
			return position;
		}

		constexpr bool IsButtonDown(MouseButtons value) const
		{
			if (value >= MouseButtons::COUNT)
				return false;

			return buttons_down[static_cast<size_t>(value)];
		}

		constexpr bool WasButtonDown(MouseButtons value) const
		{
			if (value >= MouseButtons::COUNT)
				return false;

			return buttons_down_prev[static_cast<size_t>(value)];
		}
	};

	class KeyboardState
	{
		friend DXWindow;

		bool keys_down[static_cast<size_t>(Keys::COUNT)]      = {};
		bool keys_down_prev[static_cast<size_t>(Keys::COUNT)] = {};

	public:
		constexpr bool IsKeyDown(Keys value) const
		{
			if (value >= Keys::COUNT)
				return false;

			return keys_down[static_cast<size_t>(value)];
		}

		constexpr bool WasKeyDown(Keys value) const
		{
			if (value >= Keys::COUNT)
				return false;

			return keys_down_prev[static_cast<size_t>(value)];
		}
	};

	DXWindow(std::wstring_view name, std::wstring_view title, uint32_t width, uint32_t height);

	virtual ~DXWindow();

	constexpr bool  IsOpen() const
	{
		return is_open;
	}

	constexpr bool  IsFocus() const
	{
		return is_focus;
	}

	constexpr bool  IsVSync() const
	{
		return is_vsync_enabled;
	}

	constexpr bool  IsClosing() const
	{
		return is_closing;
	}

	constexpr bool  IsResizable() const
	{
		return is_resizable;
	}

	constexpr bool  IsMinimized() const
	{
		return is_minimized;
	}

	constexpr bool  IsMaximized() const
	{
		return is_maximized;
	}

	constexpr bool  IsMaximizable() const
	{
		return is_maximizable;
	}

	constexpr bool  IsContentLoaded() const
	{
		return is_content_loaded;
	}

	constexpr auto  GetIcon() const
	{
		return icon.Type;
	}

	constexpr auto& GetName() const
	{
		return name;
	}

	constexpr auto& GetSize() const
	{
		return size;
	}

	constexpr auto& GetTitle() const
	{
		return title;
	}

	constexpr auto  GetCursor() const
	{
		return cursor ? cursor->Type : Cursors::None;
	}

	constexpr auto& GetMouse() const
	{
		return mouse;
	}

	constexpr auto& GetKeyboard() const
	{
		return keyboard;
	}

	constexpr auto  GetClipboard() const
	{
		return clipboard.IsSet ? &clipboard.String: nullptr;
	}

	constexpr auto& GetTransform() const
	{
		return transform->Original;
	}

	constexpr auto& GetBackground() const
	{
		return background_color;
	}

	constexpr auto& GetResolution() const
	{
		return resolution;
	}

	bool              SetIcon(Icons value);
	bool              SetIcon(HICON value);

	bool              SetSize(uint32_t width, uint32_t height);

	void              SetVSync(bool value);

	void              SetBackground(const Color& value);

	bool              PopClip();
	bool              PopClip(RectangleF& bounds, bool& aa);
	void              PushClip(const RectangleF& bounds, bool aa);

	bool              PopCursor();
	bool              PopCursor(Cursors& value);
	bool              PushCursor(Cursors value);
	void              PushCursor(HCURSOR value);

	bool              PopTransform();
	bool              PopTransform(Transform& value);
	void              PushTransform(const Transform& value);

	bool              DrawLine(const Vector2F* points, size_t count, const Brush& brush, float stroke_width);
	bool              DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity);
	bool              DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity, const RectangleF& source);
	bool              DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity, float source_x, float source_y, float source_width, float source_height);
	bool              DrawCircle(float x, float y, const Brush& brush, float radius, float stroke_width);
	bool              DrawString(float x, float y, const Brush& brush, const TextFormat& format, std::wstring_view string);
	bool              DrawString(float x, float y, const Brush& brush, const TextFormat& format, std::wstring_view string, float width, float height);
	bool              DrawEllipse(float x, float y, const Brush& brush, float width, float height, float stroke_width);
	bool              DrawGeometry(float x, float y, const Brush& brush, const PathGeometry& geometry, float stroke_width);
	bool              DrawRectangle(float x, float y, const Brush& brush, float width, float height, float stroke_width);
	bool              DrawRectangle(float x, float y, const Brush& brush, float width, float height, float stroke_width, float corner_radius);
	bool              DrawTextLayout(float x, float y, const Brush& brush, const TextLayout& layout);
	bool              DrawSolidCircle(float x, float y, const Brush& brush, float radius);
	bool              DrawSolidEllipse(float x, float y, const Brush& brush, float width, float height);
	bool              DrawSolidGeometry(float x, float y, const Brush& brush, const PathGeometry& geometry);
	bool              DrawSolidRectangle(float x, float y, const Brush& brush, float width, float height);
	bool              DrawSolidRectangle(float x, float y, const Brush& brush, float width, float height, float corner_radius);

	MessageBoxResults MessageBox(std::wstring_view title, std::wstring_view message, MessageBoxIcons icon, MessageBoxButtons buttons);
	MessageBoxResults MessageBox(std::wstring_view title, std::wstring_view message, MessageBoxIcons icon, MessageBoxButtons buttons, MessageBoxDefaultButtons default_button);

	FileDialogResult  OpenFileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags = FileDialogFlags::None);
	FileDialogResult  SaveFileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags = FileDialogFlags::None);

	// @return 0 on error
	// @return -1 on close
	int  Poll();
	bool Draw();

	void Close();

protected:
	virtual bool OnPoll() = 0;

	virtual bool OnDraw() = 0;
	virtual bool OnDrawGUI() = 0;

	virtual bool OnDropFile(const Vector2F& position, std::wstring_view path) = 0;

	virtual bool OnMouseMove(const Vector2F& position) = 0;
	virtual bool OnMouseScroll(const Vector2F& position, int delta) = 0;
	virtual bool OnMouseButtonUp(const Vector2F& position, MouseButtons button) = 0;
	virtual bool OnMouseButtonDown(const Vector2F& position, MouseButtons button) = 0;

	virtual bool OnKeyboardChar(wchar_t value) = 0;
	virtual bool OnKeyboardKeyUp(Keys key) = 0;
	virtual bool OnKeyboardKeyDown(Keys key) = 0;

	virtual bool OnResolutionChanged(uint32_t width, uint32_t height) = 0;

private:
	bool Create();
	void Destroy();

	bool Audio_Create();
	void Audio_Destroy();

	bool Graphics_Create();
	void Graphics_Destroy();
	bool Graphics_Target_Create();
	void Graphics_Target_Destroy();
	bool Graphics_Target_Clear();
	// @return 0 on error
	// @return -1 on D2DERR_RECREATE_TARGET
	int  Graphics_Target_Present();

	bool Resources_Load(bool is_reload);
	void Resources_Unload(bool is_reload);

	bool Resource_Add(IResource* resource);
	bool Resource_Remove(IResource* resource);
	void Resource_Move(IResource* source, IResource* destination);
	bool Resource_Copy(const IResource* source, const IResource* destination);

	FileDialogResult FileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags, BOOL(*function)(LPOPENFILENAMEW));

private:
	static LRESULT  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static UINT_PTR CALLBACK FileDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	friend Brush;
	friend Sound;
	friend Bitmap;
	friend TextFormat;
	friend TextLayout;
	friend PathGeometry;
	friend Texture2D;
	friend ShaderResourceView;

	struct Clip
	{
		bool       AA;
		RectangleF Bounds;
	};

	struct Clipboard
	{
		bool         IsSet;

		std::wstring String;
	};

	struct IconContext
	{
		Icons Type;
		HICON Handle;
	};

	struct CursorContext
	{
		Cursors Type;
		HCURSOR Handle;
	};

	struct TransformContext
	{
		Transform Original;
		Transform Modified;
	};

	bool                         is_open;
	bool                         is_focus;
	bool                         is_closing;
	bool                         is_resizable;
	bool                         is_minimized;
	bool                         is_maximized;
	bool                         is_maximizable;
	bool                         is_vsync_enabled;
	bool                         is_resize_pending;
	bool                         is_content_loaded;
	bool                         is_mouse_in_client;
	bool                         is_mouse_track_enabled;

	MSG                          msg;
	ATOM                         atom;
	WNDCLASSEXW                  clazz;
	HWND                         handle;

	SizeU32                      size;
	std::wstring                 name;
	std::wstring                 title;
	Vector2<uint32_t>            position;
	SizeU32                      resolution;
	Color                        background_color;

	IconContext                  icon;
	CursorContext*               cursor;
	std::stack<CursorContext>    cursors;

	MouseState                   mouse;
	KeyboardState                keyboard;
	Clipboard                    clipboard;

	IWICImagingFactory*          wic_factory;
	ID2D1Factory1*               d2d1_factory;
	ID2D1RenderTarget*           d2d1_render_target;
	ID2D1DeviceContext*          d2d1_device_context;
	ID3D11Device*                d3d11_device;
	ID3D11DeviceContext*         d3d11_device_context;
	ID3D11RenderTargetView*      d3d11_render_target_view;
	IDWriteFactory1*             dwrite_factory;
	IDirectSound8*               dsound_factory;
	IDXGISwapChain*              dxgi_swap_chain;

	std::list<Clip>              clips;
	std::set<IResource*>         resources;
	TransformContext*            transform;
	std::stack<TransformContext> transforms;
};
