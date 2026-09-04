#include "DXWindow.hpp"

#ifdef DXWINDOW_FIO
	#include <FIO/File.hpp>
#endif

#include <cwctype>

#include <shellapi.h>

#ifdef DXWINDOW_IMGUI
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

struct WindowIcon
{
	DXWindow::Icons Icon;
	HICON(*         LoadIcon)();
};

constexpr const WindowIcon WINDOW_ICONS[(size_t)DXWindow::Icons::COUNT] =
{
	{ DXWindow::Icons::None,        []() { return (HICON)NULL; }                     },
	{ DXWindow::Icons::Error,       []() { return LoadIconA(NULL, IDI_HAND); }        },
	{ DXWindow::Icons::Asterisk,    []() { return LoadIconA(NULL, IDI_ASTERISK); }    },
	{ DXWindow::Icons::Question,    []() { return LoadIconA(NULL, IDI_QUESTION); }    },
	{ DXWindow::Icons::Application, []() { return LoadIconA(NULL, IDI_APPLICATION); } },
	{ DXWindow::Icons::Exclamation, []() { return LoadIconA(NULL, IDI_EXCLAMATION); } },
	{ DXWindow::Icons::UserDefined, []() { return (HICON)NULL; }                     }
};
template<size_t ... I>
consteval bool static_assert_window_icons(std::index_sequence<I ...>)
{
	return ((WINDOW_ICONS[I].Icon == (DXWindow::Icons)I) && ...);
}
static_assert(static_assert_window_icons(std::make_index_sequence<(size_t)DXWindow::Icons::COUNT> {}));

struct WindowCursor
{
	DXWindow::Cursors Cursor;
	HCURSOR(*         LoadCursor)();
};

constexpr const WindowCursor WINDOW_CURSORS[(size_t)DXWindow::Cursors::COUNT] =
{
	{ DXWindow::Cursors::None,        []() { return (HCURSOR)NULL; }                      },
	{ DXWindow::Cursors::No,          []() { return LoadCursorA(NULL, IDC_NO); }          },
	{ DXWindow::Cursors::Hand,        []() { return LoadCursorA(NULL, IDC_HAND); }        },
	{ DXWindow::Cursors::Help,        []() { return LoadCursorA(NULL, IDC_HELP); }        },
	{ DXWindow::Cursors::Wait,        []() { return LoadCursorA(NULL, IDC_WAIT); }        },
	{ DXWindow::Cursors::Arrow,       []() { return LoadCursorA(NULL, IDC_ARROW); }       },
	{ DXWindow::Cursors::Cross,       []() { return LoadCursorA(NULL, IDC_CROSS); }       },
	{ DXWindow::Cursors::IBeam,       []() { return LoadCursorA(NULL, IDC_IBEAM); }       },
	{ DXWindow::Cursors::UpArrow,     []() { return LoadCursorA(NULL, IDC_UPARROW); }     },
	{ DXWindow::Cursors::Size,        []() { return LoadCursorA(NULL, IDC_SIZEALL); }     },
	{ DXWindow::Cursors::SizeX,       []() { return LoadCursorA(NULL, IDC_SIZEWE); }      },
	{ DXWindow::Cursors::SizeY,       []() { return LoadCursorA(NULL, IDC_SIZENS); }      },
	{ DXWindow::Cursors::SizeNWSE,    []() { return LoadCursorA(NULL, IDC_SIZENWSE); }    },
	{ DXWindow::Cursors::SizeNESW,    []() { return LoadCursorA(NULL, IDC_SIZENESW); }    },
	{ DXWindow::Cursors::AppStarting, []() { return LoadCursorA(NULL, IDC_APPSTARTING); } },
	{ DXWindow::Cursors::UserDefined, []() { return (HCURSOR)NULL; }                      }
};
template<size_t ... I>
consteval bool static_assert_window_cursors(std::index_sequence<I ...>)
{
	return ((WINDOW_CURSORS[I].Cursor == (DXWindow::Cursors)I) && ...);
}
static_assert(static_assert_window_cursors(std::make_index_sequence<(size_t)DXWindow::Cursors::COUNT> {}));

const DXWindow::Color    DXWindow::Color::Black          = DXWindow::Color::FromARGB(0xFF000000);
const DXWindow::Color    DXWindow::Color::White          = DXWindow::Color::FromARGB(0xFFFFFFFF);
const DXWindow::Color    DXWindow::Color::Gray           = DXWindow::Color::FromARGB(0xFF808080);
const DXWindow::Color    DXWindow::Color::Red            = DXWindow::Color::FromARGB(0xFFFF0000);
const DXWindow::Color    DXWindow::Color::Green          = DXWindow::Color::FromARGB(0xFF00FF00);
const DXWindow::Color    DXWindow::Color::Blue           = DXWindow::Color::FromARGB(0xFF0000FF);
const DXWindow::Color    DXWindow::Color::Orange         = DXWindow::Color::FromARGB(0xFFFF7F00);
const DXWindow::Color    DXWindow::Color::Yellow         = DXWindow::Color::FromARGB(0xFFFFFF00);
const DXWindow::Color    DXWindow::Color::Chartreuse     = DXWindow::Color::FromARGB(0xFF7FFF00);
const DXWindow::Color    DXWindow::Color::SpringGreen    = DXWindow::Color::FromARGB(0xFF00FF7F);
const DXWindow::Color    DXWindow::Color::Cyan           = DXWindow::Color::FromARGB(0xFF00FFFF);
const DXWindow::Color    DXWindow::Color::Azure          = DXWindow::Color::FromARGB(0xFF007FFF);
const DXWindow::Color    DXWindow::Color::Violet         = DXWindow::Color::FromARGB(0xFF7F00FF);
const DXWindow::Color    DXWindow::Color::Magenta        = DXWindow::Color::FromARGB(0xFFFF00FF);
const DXWindow::Color    DXWindow::Color::Rose           = DXWindow::Color::FromARGB(0xFFFF007F);
const DXWindow::Color    DXWindow::Color::CornflowerBlue = DXWindow::Color::FromARGB(0xFF6495ED);
const DXWindow::Color    DXWindow::Color::Transparent    = DXWindow::Color::FromARGB(0x00000000);

const DXWindow::TimeSpan DXWindow::TimeSpan::Zero     = 0;
const DXWindow::TimeSpan DXWindow::TimeSpan::Infinite = UINT64_MAX;

DXWindow::Font::Font()
	: Size(0),
	Style(FontStyles::Normal),
	Weight(FontWeights::Normal),
	Stretch(FontStretches::Normal)
{
}
DXWindow::Font::Font(std::wstring_view name, float size, FontStyles style, FontWeights weight, FontStretches stretch)
	: Name(name),
	Size(size),
	Style(style),
	Weight(weight),
	Stretch(stretch)
{
}

DXWindow::Brush::Brush(Brush&& brush)
	: type(brush.type),
	brush(brush.brush),
	color(brush.color),
	bitmap(brush.bitmap),
	window(brush.window)
{
	brush.type   = (BrushTypes)-1;
	brush.brush  = nullptr;
	brush.window = nullptr;

	window->Resource_Move(&brush, this);
}
DXWindow::Brush::Brush(const Brush& brush)
	: type(brush.type),
	brush(nullptr),
	color(brush.color),
	bitmap(brush.bitmap),
	window(brush.window)
{
	window->Resource_Copy(&brush, this);
}
DXWindow::Brush::Brush(DXWindow& window, const Color& color)
	: type(BrushTypes::SolidColor),
	brush(nullptr),
	color(color),
	bitmap(nullptr),
	window(&window)
{
	window.Resource_Add(this);
}
DXWindow::Brush::Brush(DXWindow& window, const Bitmap& bitmap)
	: type(BrushTypes::Bitmap),
	brush(nullptr),
	color(Color::Black),
	bitmap(&bitmap),
	window(&window)
{
	window.Resource_Add(this);
}
DXWindow::Brush::~Brush()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::Brush::SetColor(const Color& value)
{
	if (type != BrushTypes::SolidColor)
		return false;

	if (IsLoaded())
		((ID2D1SolidColorBrush*)brush)->SetColor(D2D1::ColorF(value.R, value.G, value.B, value.A));

	color = value;

	return true;
}
bool DXWindow::Brush::SetOpacity(float value)
{
	if (IsLoaded())
		switch (type)
		{
			case BrushTypes::Bitmap:     ((ID2D1BitmapBrush*)brush)->SetOpacity(value); break;
			case BrushTypes::SolidColor: ((ID2D1SolidColorBrush*)brush)->SetOpacity(value); break;
		}

	color.A = value;

	return true;
}
bool DXWindow::Brush::Load()
{
	if (!window->IsOpen())
		return false;

	HRESULT hResult;

	switch (type)
	{
		case BrushTypes::Bitmap:
			if (FAILED((hResult = window->d2d1_render_target->CreateBitmapBrush(GetBitmap().GetHandle(), (ID2D1BitmapBrush**)&brush))))
				return false;
			break;

		case BrushTypes::SolidColor:
			if (FAILED((hResult = window->d2d1_render_target->CreateSolidColorBrush(D2D1::ColorF(GetColor().R, GetColor().G, GetColor().B, GetColor().A), (ID2D1SolidColorBrush**)&brush))))
				return false;
			break;
	}

	return true;
}
void DXWindow::Brush::Unload()
{
	if (brush)
	{
		brush->Release();
		brush = nullptr;
	}
}
DXWindow::Brush& DXWindow::Brush::operator=(Brush&& brush)
{
	Unload();

	this->type   = brush.type;
	this->brush  = brush.brush;
	this->color  = brush.color;
	this->bitmap = brush.bitmap;
	this->window = brush.window;

	brush.type = (BrushTypes)-1;
	brush.bitmap = nullptr;
	brush.window = nullptr;

	window->Resource_Move(&brush, this);

	return *this;
}
DXWindow::Brush& DXWindow::Brush::operator=(const Brush& brush)
{
	Unload();

	this->type   = brush.type;
	this->color  = brush.color;
	this->bitmap = brush.bitmap;
	this->window = brush.window;

	window->Resource_Copy(&brush, this);

	return *this;
}

DXWindow::Sound::Sound()
	: is_paused(false),
	is_playing(false),
	is_repeating(false),
	type(Types::File),
	sound(nullptr),
	volume(1),
	window(nullptr),
	position(0)
{
}
DXWindow::Sound::Sound(Sound&& sound)
	: is_paused(sound.is_paused),
	is_playing(sound.is_playing),
	is_repeating(sound.is_repeating),
	type(sound.type),
	path(std::move(sound.path)),
	sound(sound.sound),
	buffer(std::move(sound.buffer)),
	volume(sound.volume),
	window(sound.window),
	position(sound.position)
{
	sound.is_paused    = false;
	sound.is_playing   = false;
	sound.is_repeating = false;
	sound.type         = (Types)-1;
	sound.sound        = nullptr;
	sound.volume       = 1;
	sound.window       = nullptr;
	sound.position     = 0;

	window->Resource_Move(&sound, this);
}
DXWindow::Sound::Sound(const Sound& sound)
	: is_paused(sound.is_paused),
	is_playing(sound.is_playing),
	is_repeating(sound.is_repeating),
	type(sound.type),
	path(sound.path),
	sound(nullptr),
	buffer(sound.buffer),
	volume(sound.volume),
	window(sound.window),
	position(sound.position)
{
	window->Resource_Copy(&sound, this);
}
DXWindow::Sound::Sound(DXWindow& window, std::wstring_view path)
	: is_paused(false),
	is_playing(false),
	is_repeating(false),
	type(Types::File),
	path(path),
	sound(nullptr),
	volume(1),
	window(&window),
	position(0)
{
	window.Resource_Add(this);
}
DXWindow::Sound::Sound(DXWindow& window, const void* buffer, size_t size)
	: is_paused(false),
	is_playing(false),
	is_repeating(false),
	type(Types::Memory),
	sound(nullptr),
	buffer((const char*)buffer, size),
	volume(1),
	window(&window),
	position(0)
{
	window.Resource_Add(this);
}
DXWindow::Sound::~Sound()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::Sound::SetVolume(float value)
{
	if (value < 0)
		value = 0;
	else if (value > 1)
		value = 1;

	HRESULT hResult;

	if (IsLoaded() && FAILED((hResult = sound->SetVolume((LONG)(DSBVOLUME_MIN - (DSBVOLUME_MIN * value))))))
		return false;

	volume = value;

	return true;
}
bool DXWindow::Sound::Play(bool repeat)
{
	if (IsLoaded() && !IsPlaying())
	{
		HRESULT hResult;

		if (IsPaused() && FAILED((hResult = sound->SetCurrentPosition((DWORD)position))))
			return false;

		if (FAILED((hResult = sound->Play(0, 0, repeat ? DSBPLAY_LOOPING : 0))))
			return false;

		is_paused    = false;
		is_playing   = true;
		is_repeating = repeat;
	}

	return true;
}
bool DXWindow::Sound::Pause()
{
	if (IsLoaded() && IsPlaying())
	{
		HRESULT hResult;
		DWORD   position_play;
		DWORD   position_write;

		if (FAILED((hResult = sound->GetCurrentPosition(&position_play, &position_write))))
			return false;

		if (FAILED((hResult = sound->Stop())))
			return false;

		position   = position_play;
		is_paused  = true;
		is_playing = false;
	}

	return true;
}
bool DXWindow::Sound::Stop()
{
	if (IsLoaded() && (IsPlaying() || IsPaused()))
	{
		HRESULT hResult;

		if (FAILED((hResult = sound->Stop())))
			return false;

		is_paused    = false;
		is_playing   = false;
		is_repeating = false;
	}

	return true;
}
bool DXWindow::Sound::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	switch (type)
	{
		case Types::File:
		{
#ifdef DXWINDOW_FIO
			FIO::File file(path, FIO::File::MODE_READ);

			switch (file.Open())
			{
				case 0:  return false;
				case -1: return false;
			}

			std::vector<uint8_t> buffer((size_t)file.GetSize());

			for (size_t i = 0; i < buffer.size(); )
			{
				size_t num_bytes_read;

				if (!file.Read(&buffer[i], buffer.size() - i, num_bytes_read))
					return false;

				i += num_bytes_read;
			}

			return LoadWave(buffer.data(), buffer.size());
#endif
		}
		break;

		case Types::Memory:
			return LoadWave(buffer.data(), buffer.length());
	}

	return false;
}
void DXWindow::Sound::Unload()
{
	if (sound)
	{
		sound->Release();
		sound = nullptr;
	}
}
DXWindow::Sound& DXWindow::Sound::operator=(Sound&& sound)
{
	Unload();

	this->is_paused    = sound.is_paused;
	this->is_playing   = sound.is_playing;
	this->is_repeating = sound.is_repeating;
	this->type         = sound.type;
	this->path         = std::move(sound.path);
	this->sound        = sound.sound;
	this->buffer       = std::move(sound.buffer);
	this->volume       = sound.volume;
	this->window       = sound.window;
	this->position     = sound.position;

	sound.is_paused    = false;
	sound.is_playing   = false;
	sound.is_repeating = false;
	sound.type         = (Types)-1;
	sound.sound        = nullptr;
	sound.volume       = 1;
	sound.window       = nullptr;
	sound.position     = 0;

	window->Resource_Move(&sound, this);

	return *this;
}
DXWindow::Sound& DXWindow::Sound::operator=(const Sound& sound)
{
	Unload();

	this->is_paused    = sound.is_paused;
	this->is_playing   = sound.is_playing;
	this->is_repeating = sound.is_repeating;
	this->type         = sound.type;
	this->path         = sound.path;
	this->sound        = nullptr;
	this->buffer       = sound.buffer;
	this->volume       = sound.volume;
	this->window       = sound.window;
	this->position     = sound.position;

	window->Resource_Copy(&sound, this);

	return *this;
}
bool DXWindow::Sound::LoadWave(const void* buffer, size_t size)
{
	if (size < sizeof(WaveHeader))
		return false;

	auto header = (const WaveHeader*)buffer;

	if (header->Format[0] != 'W' || header->Format[1] != 'A' || header->Format[2] != 'V' || header->Format[3] != 'E')
		return false;

	if (header->ChunkID[0] != 'R' || header->ChunkID[1] != 'I' || header->ChunkID[2] != 'F' || header->ChunkID[3] != 'F')
		return false;

	if (header->SubChunk1Id[0] == 'f' || header->SubChunk1Id[1] == 'm' || header->SubChunk1Id[2] == 't' || header->SubChunk1Id[3] == ' ')
		if (header->AudioFormat != WAVE_FORMAT_PCM)
			return false;

	WAVEFORMATEX format    = {};
	format.nChannels       = header->NumChannels;
	format.wFormatTag      = WAVE_FORMAT_PCM;
	format.nBlockAlign     = (format.wBitsPerSample / 8) * format.nChannels;
	format.wBitsPerSample  = header->BitsPerSample;
	format.nSamplesPerSec  = header->SampleRate;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	DSBUFFERDESC desc    = {};
	desc.dwSize          = sizeof(DSBUFFERDESC);
	desc.dwFlags         = DSBCAPS_CTRLVOLUME;
	desc.lpwfxFormat     = &format;
	desc.dwBufferBytes   = (header->ChunkSize + 8) - sizeof(WaveHeader);
	desc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* sound;
	HRESULT             hResult;

	if (FAILED((hResult = window->dsound_factory->CreateSoundBuffer(&desc, &sound, nullptr))))
		return false;

	LPVOID buffer_data_ptr;

	if (FAILED((hResult = sound->Lock(0, desc.dwBufferBytes, &buffer_data_ptr, &desc.dwBufferBytes, nullptr, nullptr, 0))))
	{
		sound->Release();

		return false;
	}

	memcpy(buffer_data_ptr, &((const uint8_t*)buffer)[sizeof(WaveHeader)], desc.dwBufferBytes);

	if (FAILED((hResult = sound->Unlock(buffer_data_ptr, desc.dwBufferBytes, nullptr, 0))))
	{
		sound->Release();

		return false;
	}

	this->sound = sound;

	return true;
}

DXWindow::Bitmap::Bitmap()
	: type(Types::File),
	bitmap(nullptr),
	window(nullptr),
	interpolation_mode(BitmapInterpolationModes::Linear)
{
}
DXWindow::Bitmap::Bitmap(Bitmap&& bitmap)
	: type(bitmap.type),
	path(std::move(bitmap.path)),
	buffer(std::move(bitmap.buffer)),
	bitmap(bitmap.bitmap),
	window(bitmap.window),
	pixel_size(bitmap.pixel_size),
	bitmap_size(bitmap.bitmap_size),
	transparency_keys(std::move(bitmap.transparency_keys)),
	interpolation_mode(bitmap.interpolation_mode)
{
	bitmap.type               = (Types)-1;
	bitmap.bitmap             = nullptr;
	bitmap.window             = nullptr;
	bitmap.interpolation_mode = BitmapInterpolationModes::Linear;

	window->Resource_Move(&bitmap, this);
}
DXWindow::Bitmap::Bitmap(const Bitmap& bitmap)
	: type(bitmap.type),
	path(bitmap.path),
	buffer(bitmap.buffer),
	bitmap(nullptr),
	window(bitmap.window),
	pixel_size(bitmap.pixel_size),
	bitmap_size(bitmap.bitmap_size),
	transparency_keys(bitmap.transparency_keys),
	interpolation_mode(bitmap.interpolation_mode)
{
	window->Resource_Copy(&bitmap, this);
}
DXWindow::Bitmap::Bitmap(DXWindow& window, std::wstring_view path)
	: type(Types::File),
	path(path),
	bitmap(nullptr),
	window(&window),
	interpolation_mode(BitmapInterpolationModes::Linear)
{
	window.Resource_Add(this);
}
DXWindow::Bitmap::Bitmap(DXWindow& window, const void* buffer, size_t size)
	: type(Types::Memory),
	buffer((const char*)buffer, size),
	bitmap(nullptr),
	window(&window),
	interpolation_mode(BitmapInterpolationModes::Linear)
{
	window.Resource_Add(this);
}
DXWindow::Bitmap::~Bitmap()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::Bitmap::AddTransparencyKey(const Color& value)
{
	transparency_keys.push_back(value);

	return !IsLoaded() || (Unload(), Load());
}
bool DXWindow::Bitmap::RemoveTransparencyKey(const Color& value)
{
	for (auto it = transparency_keys.rbegin(); it != transparency_keys.rend(); ++it)
	{
		if (*it == value)
		{
			transparency_keys.erase(it.base());

			return !IsLoaded() || (Unload(), Load());
		}
	}

	return true;
}
void DXWindow::Bitmap::SetInterpolationMode(BitmapInterpolationModes value)
{
	interpolation_mode = value;
}
bool DXWindow::Bitmap::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	HRESULT            hResult;
	IWICBitmapDecoder* decoder;

	switch (type)
	{
		case Types::File:
			if (FAILED((hResult = window->wic_factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder))))
				return false;
			break;

		case Types::Memory:
		{
			IWICStream* stream;

			if (FAILED((hResult = window->wic_factory->CreateStream(&stream))))
				return false;

			if (FAILED((hResult = stream->InitializeFromMemory((BYTE*)buffer.data(), buffer.length()))))
			{
				stream->Release();

				return false;
			}

			if (FAILED((hResult = window->wic_factory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, &decoder))))
			{
				stream->Release();

				return false;
			}

			stream->Release();
		}
		break;

		default:
			return false;
	}

	IWICBitmapFrameDecode* frame;

	if (FAILED((hResult = decoder->GetFrame(0, &frame))))
	{
		decoder->Release();

		return false;
	}

	IWICFormatConverter* converter;

	if (FAILED((hResult = window->wic_factory->CreateFormatConverter(&converter))))
	{
		frame->Release();
		decoder->Release();

		return false;
	}

	if (FAILED((hResult = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut))))
	{
		converter->Release();
		frame->Release();
		decoder->Release();

		return false;
	}

	IWICBitmap* wic_bitmap;
	UINT        wic_bitmap_width;
	UINT        wic_bitmap_height;

	if (FAILED((hResult = frame->GetSize(&wic_bitmap_width, &wic_bitmap_height))))
	{
		converter->Release();
		frame->Release();
		decoder->Release();

		return false;
	}

	if (FAILED((hResult = window->wic_factory->CreateBitmapFromSource(converter, WICBitmapCacheOnLoad, &wic_bitmap))))
	{
		converter->Release();
		frame->Release();
		decoder->Release();

		return false;
	}

	for (auto& key : transparency_keys)
	{
		IWICBitmapLock* wic_bitmap_lock;
		WICRect         wic_bitmap_rect =
		{
			.X      = 0,
			.Y      = 0,
			.Width  = (INT)wic_bitmap_width,
			.Height = (INT)wic_bitmap_height
		};

		if (FAILED((hResult = wic_bitmap->Lock(&wic_bitmap_rect, WICBitmapLockWrite, &wic_bitmap_lock))))
		{
			wic_bitmap->Release();
			converter->Release();
			frame->Release();
			decoder->Release();

			return false;
		}

		BYTE* wic_bitmap_buffer;
		UINT  wic_bitmap_buffer_size;
		UINT  wic_bitmap_buffer_stride;

		if (FAILED((hResult = wic_bitmap_lock->GetDataPointer(&wic_bitmap_buffer_size, &wic_bitmap_buffer))))
		{
			wic_bitmap_lock->Release();
			wic_bitmap->Release();
			converter->Release();
			frame->Release();
			decoder->Release();

			return false;
		}

		if (FAILED((hResult = wic_bitmap_lock->GetStride(&wic_bitmap_buffer_stride))))
		{
			wic_bitmap_lock->Release();
			wic_bitmap->Release();
			converter->Release();
			frame->Release();
			decoder->Release();

			return false;
		}

		BYTE key_bgra[4] =
		{
			(BYTE)(key.B * 255),
			(BYTE)(key.G * 255),
			(BYTE)(key.R * 255),
			(BYTE)(key.A * 255)
		};

		for (UINT y = 0; y < wic_bitmap_height; ++y)
		{
			for (UINT x = 0; x < wic_bitmap_width; ++x)
			{
				auto wic_bitmap_buffer_pixel = &wic_bitmap_buffer[(y * wic_bitmap_buffer_stride) + (x * 4)];

				if (!memcmp(key_bgra, wic_bitmap_buffer_pixel, 4))
					memset(wic_bitmap_buffer_pixel, 0, 4);
			}
		}

		wic_bitmap_lock->Release();
	}

	if (FAILED((hResult = window->d2d1_device_context->CreateBitmapFromWicBitmap(wic_bitmap, nullptr, &bitmap))))
	{
		wic_bitmap->Release();
		converter->Release();
		frame->Release();
		decoder->Release();

		return false;
	}

	wic_bitmap->Release();
	converter->Release();
	frame->Release();
	decoder->Release();

	auto bitmap_size       = bitmap->GetSize();
	auto bitmap_pixel_size = bitmap->GetPixelSize();

	this->pixel_size =
	{
		.Width  = bitmap_pixel_size.width,
		.Height = bitmap_pixel_size.height
	};

	this->bitmap_size =
	{
		.Width  = (uint32_t)bitmap_size.width,
		.Height = (uint32_t)bitmap_size.height
	};

	return true;
}
void DXWindow::Bitmap::Unload()
{
	if (bitmap)
	{
		bitmap->Release();
		bitmap = nullptr;
	}
}
DXWindow::Bitmap& DXWindow::Bitmap::operator=(Bitmap&& bitmap)
{
	Unload();

	this->type               = bitmap.type;
	this->path               = std::move(bitmap.path);
	this->buffer             = std::move(bitmap.buffer);
	this->bitmap             = bitmap.bitmap;
	this->window             = bitmap.window;
	this->pixel_size         = bitmap.pixel_size;
	this->bitmap_size        = bitmap.bitmap_size;
	this->transparency_keys  = std::move(bitmap.transparency_keys);
	this->interpolation_mode = bitmap.interpolation_mode;

	bitmap.type               = (Types)-1;
	bitmap.bitmap             = nullptr;
	bitmap.window             = nullptr;
	bitmap.interpolation_mode = BitmapInterpolationModes::Linear;

	window->Resource_Move(&bitmap, this);

	return *this;
}
DXWindow::Bitmap& DXWindow::Bitmap::operator=(const Bitmap& bitmap)
{
	Unload();

	this->type               = bitmap.type;
	this->path               = bitmap.path;
	this->buffer             = bitmap.buffer;
	this->window             = bitmap.window;
	this->pixel_size         = bitmap.pixel_size;
	this->bitmap_size        = bitmap.bitmap_size;
	this->transparency_keys  = bitmap.transparency_keys;
	this->interpolation_mode = bitmap.interpolation_mode;

	window->Resource_Copy(&bitmap, this);

	return *this;
}

DXWindow::TextFormat::TextFormat()
	: format(nullptr),
	window(nullptr)
{
}
DXWindow::TextFormat::TextFormat(TextFormat&& format)
	: font(std::move(format.font)),
	format(format.format),
	window(format.window),
	alignment(format.alignment),
	word_wrapping(format.word_wrapping)
{
	format.format = nullptr;
	format.window = nullptr;

	window->Resource_Move(&format, this);
}
DXWindow::TextFormat::TextFormat(const TextFormat& format)
	: font(format.font),
	format(nullptr),
	window(format.window),
	alignment(format.alignment),
	word_wrapping(format.word_wrapping)
{
	window->Resource_Copy(&format, this);
}
DXWindow::TextFormat::TextFormat(DXWindow& window, Font&& font, TextAlignments alignment, bool word_wrapping)
	: font(std::move(font)),
	format(nullptr),
	window(&window),
	alignment(alignment),
	word_wrapping(word_wrapping)
{
	if (!(int)this->font.Weight)
		this->font.Weight = FontWeights::Normal;

	if (!(int)this->font.Stretch)
		this->font.Stretch = FontStretches::Normal;

	window.Resource_Add(this);
}
DXWindow::TextFormat::~TextFormat()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::TextFormat::SetFont(Font&& value)
{
	if (IsLoaded())
	{
		IDWriteTextFormat* format;
		HRESULT            hResult;

		if (!(int)value.Weight)
			value.Weight = FontWeights::Normal;

		if (!(int)value.Stretch)
			value.Stretch = FontStretches::Normal;

		if (FAILED((hResult = window->dwrite_factory->CreateTextFormat(value.Name.c_str(), nullptr, (DWRITE_FONT_WEIGHT)value.Weight, (DWRITE_FONT_STYLE)value.Style, (DWRITE_FONT_STRETCH)value.Stretch, value.Size, L"", &format))))
			return false;

		if (FAILED((hResult = format->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)(((uint16_t)GetAlignment() & 0xFF00) >> 8)))))
		{
			format->Release();

			return false;
		}

		if (FAILED((hResult = format->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)((uint16_t)GetAlignment() & 0xFF)))))
		{
			format->Release();

			return false;
		}

		if (FAILED((hResult = format->SetWordWrapping(GetWordWrapping() ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP))))
		{
			format->Release();

			return false;
		}

		this->format->Release();
		this->format = format;
	}

	font = std::move(value);

	return true;
}
bool DXWindow::TextFormat::SetFontSize(float value)
{
	if (IsLoaded())
	{
		auto font = GetFont();
		font.Size = value;

		return SetFont(std::move(font));
	}

	font.Size = value;

	return true;
}
bool DXWindow::TextFormat::SetAlignment(TextAlignments value)
{
	if (IsLoaded())
	{
		HRESULT hResult;

		if (FAILED((hResult = format->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)(((uint16_t)value & 0xFF00) >> 8)))))
			return false;

		if (FAILED((hResult = format->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)((uint16_t)value & 0xFF)))))
			return false;
	}

	alignment = value;

	return true;
}
bool DXWindow::TextFormat::SetWordWrapping(bool value)
{
	if (IsLoaded())
	{
		HRESULT hResult;

		if (FAILED((hResult = format->SetWordWrapping(value ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP))))
			return false;
	}

	word_wrapping = value;

	return true;
}
bool DXWindow::TextFormat::CalculateMetrics(TextMetrics& value, std::wstring_view string) const
{
	if (!IsLoaded())
		return false;

	IDWriteTextLayout*  layout;
	DWRITE_TEXT_METRICS metrics;
	HRESULT             hResult;
	auto                resolution = window->GetResolution();

	if (FAILED((hResult = window->dwrite_factory->CreateTextLayout(string.data(), string.length(), format, resolution.Width, resolution.Height, &layout))))
		return false;

	if (FAILED((hResult = layout->GetMetrics(&metrics))))
	{
		layout->Release();

		return false;
	}

	layout->Release();

	value.Left                             = metrics.left;
	value.Top                              = metrics.top;
	value.Width                            = metrics.width;
	value.WidthIncludingTrailingWhitespace = metrics.widthIncludingTrailingWhitespace;
	value.Height                           = metrics.height;
	value.LayoutWidth                      = metrics.layoutWidth;
	value.LayoutHeight                     = metrics.layoutHeight;
	value.LineCount                        = metrics.lineCount;

	return true;
}
bool DXWindow::TextFormat::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	HRESULT hResult;

	if (FAILED((hResult = window->dwrite_factory->CreateTextFormat(GetFont().Name.c_str(), nullptr, (DWRITE_FONT_WEIGHT)GetFont().Weight, (DWRITE_FONT_STYLE)GetFont().Style, (DWRITE_FONT_STRETCH)GetFont().Stretch, GetFont().Size, L"", &format))))
		return false;

	if (!SetAlignment(GetAlignment()))
	{
		format->Release();
		format = nullptr;

		return false;
	}

	if (!SetWordWrapping(GetWordWrapping()))
	{
		format->Release();
		format = nullptr;

		return false;
	}

	return true;
}
void DXWindow::TextFormat::Unload()
{
	if (format)
	{
		format->Release();
		format = nullptr;
	}
}
DXWindow::TextFormat& DXWindow::TextFormat::operator=(TextFormat&& format)
{
	Unload();

	this->font          = std::move(format.font);
	this->format        = format.format;
	this->window        = format.window;
	this->alignment     = format.alignment;
	this->word_wrapping = format.word_wrapping;

	format.format = nullptr;
	format.window = nullptr;

	window->Resource_Move(&format, this);

	return *this;
}
DXWindow::TextFormat& DXWindow::TextFormat::operator=(const TextFormat& format)
{
	Unload();

	this->font          = format.font;
	this->window        = format.window;
	this->alignment     = format.alignment;
	this->word_wrapping = format.word_wrapping;

	window->Resource_Copy(&format, this);

	return *this;
}

DXWindow::TextLayout::TextLayout()
	: format(nullptr),
	layout(nullptr),
	window(nullptr),
	clipping(true)
{
}
DXWindow::TextLayout::TextLayout(TextLayout&& layout)
	: size(layout.size),
	format(layout.format),
	layout(layout.layout),
	string(std::move(layout.string)),
	window(layout.window),
	metrics(layout.metrics),
	clipping(layout.clipping),
	alignment(layout.alignment),
	word_wrapping(layout.word_wrapping)
{
	layout.format = nullptr;
	layout.layout = nullptr;
	layout.window = nullptr;

	window->Resource_Move(&layout, this);
}
DXWindow::TextLayout::TextLayout(const TextLayout& layout)
	: size(layout.size),
	format(layout.format),
	layout(nullptr),
	string(layout.string),
	window(layout.window),
	metrics(layout.metrics),
	clipping(layout.clipping),
	alignment(layout.alignment),
	word_wrapping(layout.word_wrapping)
{
	window->Resource_Copy(&layout, this);
}
DXWindow::TextLayout::TextLayout(DXWindow& window, const TextFormat& format, std::wstring_view string, float width, float height)
	: size{
		.Width  = width,
		.Height = height
	},
	format(&format),
	layout(nullptr),
	string(string),
	window(&window),
	clipping(true),
	alignment(format.GetAlignment()),
	word_wrapping(format.GetWordWrapping())
{
	if (window.Resource_Add(this))
		CalculateMetrics(metrics, layout);
}
DXWindow::TextLayout::~TextLayout()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::TextLayout::SetSize(float width, float height)
{
	if (IsLoaded())
	{
		HRESULT hResult;

		if (FAILED((hResult = layout->SetMaxWidth(width))))
			return false;

		if (FAILED((hResult = layout->SetMaxHeight(height))))
			return false;
	}

	size.Width  = width;
	size.Height = height;

	return true;
}
bool DXWindow::TextLayout::SetFormat(const TextFormat& value)
{
	if (IsLoaded())
	{
		IDWriteTextLayout* layout;
		HRESULT            hResult;

		if (FAILED((hResult = window->dwrite_factory->CreateTextLayout(GetString().c_str(), GetString().length(), value.GetHandle(), GetSize().Width, GetSize().Height, &layout))))
			return false;

		if (!CalculateMetrics(metrics, layout))
		{
			layout->Release();

			return false;
		}

		this->layout->Release();
		this->layout = layout;
	}

	format = &value;

	return true;
}
bool DXWindow::TextLayout::SetString(std::wstring_view value)
{
	if (IsLoaded())
	{
		IDWriteTextLayout* layout;
		HRESULT            hResult;

		if (FAILED((hResult = window->dwrite_factory->CreateTextLayout(value.data(), value.length(), GetFormat().GetHandle(), GetSize().Width, GetSize().Height, &layout))))
			return false;

		if (!CalculateMetrics(metrics, layout))
		{
			layout->Release();

			return false;
		}

		this->layout->Release();
		this->layout = layout;
	}

	string = std::move(value);

	return true;
}
void DXWindow::TextLayout::SetClipping(bool value)
{
	clipping = value;
}
bool DXWindow::TextLayout::SetAlignment(TextAlignments value)
{
	if (IsLoaded())
	{
		HRESULT hResult;

		if (FAILED((hResult = layout->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)(((uint16_t)value & 0xFF00) >> 8)))))
			return false;

		if (FAILED((hResult = layout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)((uint16_t)value & 0xFF)))))
			return false;
	}

	alignment = value;

	return true;
}
bool DXWindow::TextLayout::SetWordWrapping(bool value)
{
	if (IsLoaded())
	{
		HRESULT hResult;

		if (FAILED((hResult = layout->SetWordWrapping(value ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP))))
			return false;
	}

	word_wrapping = value;

	return true;
}
bool DXWindow::TextLayout::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	HRESULT hResult;

	if (FAILED((hResult = window->dwrite_factory->CreateTextLayout(GetString().c_str(), GetString().length(), GetFormat().GetHandle(), GetSize().Width, GetSize().Height, &layout))))
		return false;

	if (GetAlignment() != format->GetAlignment())
	{
		if (FAILED(layout->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)(((uint16_t)GetAlignment() & 0xFF00) >> 8))))
		{
			layout->Release();
			layout = nullptr;

			return false;
		}

		if (FAILED(layout->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)((uint16_t)GetAlignment() & 0xFF))))
		{
			layout->Release();
			layout = nullptr;

			return false;
		}
	}

	if (GetWordWrapping() != format->GetWordWrapping())
		if (FAILED((hResult = layout->SetWordWrapping(GetWordWrapping() ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP))))
		{
			layout->Release();
			layout = nullptr;

			return false;
		}

	if (!CalculateMetrics(metrics, layout))
	{
		layout->Release();
		layout = nullptr;

		return false;
	}

	return true;
}
void DXWindow::TextLayout::Unload()
{
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}
}
DXWindow::TextLayout& DXWindow::TextLayout::operator=(TextLayout&& layout)
{
	Unload();

	this->size          = layout.size;
	this->format        = layout.format;
	this->layout        = layout.layout;
	this->string        = std::move(layout.string);
	this->window        = layout.window;
	this->metrics       = layout.metrics;
	this->clipping      = layout.clipping;
	this->alignment     = layout.alignment;
	this->word_wrapping = layout.word_wrapping;

	layout.format = nullptr;
	layout.layout = nullptr;
	layout.window = nullptr;

	window->Resource_Move(&layout, this);

	return *this;
}
DXWindow::TextLayout& DXWindow::TextLayout::operator=(const TextLayout& layout)
{
	Unload();

	this->size          = layout.size;
	this->format        = layout.format;
	this->string        = layout.string;
	this->window        = layout.window;
	this->metrics       = layout.metrics;
	this->clipping      = layout.clipping;
	this->alignment     = layout.alignment;
	this->word_wrapping = layout.word_wrapping;

	window->Resource_Copy(&layout, this);

	return *this;
}
bool DXWindow::TextLayout::CalculateMetrics(TextMetrics& value, IDWriteTextLayout* layout)
{
	HRESULT             hResult;
	DWRITE_TEXT_METRICS metrics;

	if (FAILED((hResult = layout->GetMetrics(&metrics))))
		return false;

	value.Left                             = metrics.left;
	value.Top                              = metrics.top;
	value.Width                            = metrics.width;
	value.WidthIncludingTrailingWhitespace = metrics.widthIncludingTrailingWhitespace;
	value.Height                           = metrics.height;
	value.LayoutWidth                      = metrics.layoutWidth;
	value.LayoutHeight                     = metrics.layoutHeight;
	value.LineCount                        = metrics.lineCount;

	return true;
}

DXWindow::PathGeometry::PathGeometry()
	: window(nullptr),
	geometry(nullptr),
	geometry_sink(nullptr),
	geometry_fill_mode(FillModes::Winding)
{
}
DXWindow::PathGeometry::PathGeometry(PathGeometry&& geometry)
	: window(geometry.window),
	geometry(geometry.geometry),
	geometry_sink(geometry.geometry_sink),
	geometry_fill_mode(geometry.geometry_fill_mode)
{
	geometry.window        = nullptr;
	geometry.geometry      = nullptr;
	geometry.geometry_sink = nullptr;

	window->Resource_Move(&geometry, this);
}
DXWindow::PathGeometry::PathGeometry(DXWindow& window, FillModes fill_mode)
	: window(&window),
	geometry(nullptr),
	geometry_sink(nullptr),
	geometry_fill_mode(fill_mode)
{
	window.Resource_Add(this);
}
DXWindow::PathGeometry::~PathGeometry()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::PathGeometry::AddArc(float x, float y, float width, float height, float angle, SweepDirections direction, ArcSizes size)
{
	if (!IsLoaded() || IsFinalized())
		return false;

	geometry_sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x, y),
		D2D1::SizeF(width, height),
		angle,
		(D2D1_SWEEP_DIRECTION)direction,
		(D2D1_ARC_SIZE)size
	));

	return true;
}
bool DXWindow::PathGeometry::AddLine(float x, float y)
{
	if (!IsLoaded() || IsFinalized())
		return false;

	geometry_sink->AddLine(D2D1::Point2F(x, y));

	return true;
}
bool DXWindow::PathGeometry::AddLines(const Vector2F* points, size_t count)
{
	if (!IsLoaded() || IsFinalized())
		return false;

	for (size_t i = 0; i < count; ++i, ++points)
		geometry_sink->AddLine(D2D1::Point2F(points->X, points->X));

	return true;
}
bool DXWindow::PathGeometry::AddBezier(const Vector2F& point1, const Vector2F& point2, const Vector2F& point3)
{
	if (!IsLoaded() || IsFinalized())
		return false;

	geometry_sink->AddBezier(D2D1::BezierSegment(
		D2D1::Point2F(point1.X, point1.Y),
		D2D1::Point2F(point2.X, point2.Y),
		D2D1::Point2F(point3.X, point3.Y)
	));

	return true;
}
bool DXWindow::PathGeometry::AddQBezier(const Vector2F& point1, const Vector2F& point2)
{
	if (!IsLoaded() || IsFinalized())
		return false;

	geometry_sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(
		D2D1::Point2F(point1.X, point1.Y),
		D2D1::Point2F(point2.X, point2.Y)
	));

	return true;
}
bool DXWindow::PathGeometry::Finalize()
{
	if (!IsLoaded())
		return false;

	if (IsFinalized())
		return true;

	geometry_sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	HRESULT hResult;

	if (FAILED((hResult = geometry_sink->Close())))
		return false;

	geometry_sink->Release();
	geometry_sink = nullptr;

	return true;
}
bool DXWindow::PathGeometry::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	HRESULT hResult;

	if (FAILED((hResult = window->d2d1_factory->CreatePathGeometry(&geometry))))
		return false;

	if (FAILED((hResult = geometry->Open(&geometry_sink))))
	{
		geometry->Release();
		geometry = nullptr;

		return false;
	}

	geometry_sink->SetFillMode((D2D1_FILL_MODE)GetFillMode());
	geometry_sink->BeginFigure(D2D1::Point2F(), D2D1_FIGURE_BEGIN_FILLED);

	return true;
}
void DXWindow::PathGeometry::Unload()
{
	if (geometry)
	{
		if (geometry_sink)
		{
			geometry_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			geometry_sink->Close();
			geometry_sink->Release();
			geometry_sink = nullptr;
		}

		geometry->Release();
		geometry = nullptr;
	}
}
DXWindow::PathGeometry& DXWindow::PathGeometry::operator = (PathGeometry&& geometry)
{
	Unload();

	this->window             = geometry.window;
	this->geometry           = geometry.geometry;
	this->geometry_sink      = geometry.geometry_sink;
	this->geometry_fill_mode = geometry.geometry_fill_mode;

	geometry.window        = nullptr;
	geometry.geometry      = nullptr;
	geometry.geometry_sink = nullptr;

	window->Resource_Move(&geometry, this);

	return *this;
}

DXWindow::Texture2D::Texture2D()
	: bitmap(nullptr),
	window(nullptr),
	texture(nullptr)
{
}
DXWindow::Texture2D::Texture2D(Texture2D&& texture)
	: bitmap(texture.bitmap),
	window(texture.window),
	texture(texture.texture)
{
	texture.bitmap  = nullptr;
	texture.window  = nullptr;
	texture.texture = nullptr;

	window->Resource_Move(&texture, this);
}
DXWindow::Texture2D::Texture2D(const Texture2D& texture)
	: bitmap(texture.bitmap),
	window(texture.window),
	texture(nullptr)
{
	window->Resource_Copy(&texture, this);
}
DXWindow::Texture2D::Texture2D(DXWindow& window, const Bitmap& bitmap)
	: bitmap(&bitmap),
	window(&window),
	texture(nullptr)
{
	window.Resource_Add(this);
}
DXWindow::Texture2D::~Texture2D()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::Texture2D::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	HRESULT       hResult;
	IDXGISurface* dxgi_surface;

	if (FAILED((hResult = bitmap->GetHandle()->GetSurface(&dxgi_surface))))
		return false;

	if (FAILED((hResult = dxgi_surface->QueryInterface(IID_PPV_ARGS(&texture)))))
	{
		dxgi_surface->Release();

		return false;
	}

	dxgi_surface->Release();

	return true;
}
void DXWindow::Texture2D::Unload()
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}
DXWindow::Texture2D& DXWindow::Texture2D::operator = (Texture2D&& texture)
{
	Unload();

	this->bitmap  = texture.bitmap;
	this->window  = texture.window;
	this->texture = texture.texture;

	texture.bitmap  = nullptr;
	texture.window  = nullptr;
	texture.texture = nullptr;

	window->Resource_Move(&texture, this);

	return *this;
}
DXWindow::Texture2D& DXWindow::Texture2D::operator = (const Texture2D& texture)
{
	Unload();

	this->bitmap  = texture.bitmap;
	this->window  = texture.window;
	this->texture = nullptr;

	window->Resource_Copy(&texture, this);

	return *this;
}

DXWindow::ShaderResourceView::ShaderResourceView()
	: view(nullptr),
	window(nullptr),
	texture(nullptr)
{
}
DXWindow::ShaderResourceView::ShaderResourceView(ShaderResourceView&& view)
	: view(view.view),
	window(view.window),
	texture(view.texture)
{
	view.view    = nullptr;
	view.window  = nullptr;
	view.texture = nullptr;

	window->Resource_Move(&view, this);
}
DXWindow::ShaderResourceView::ShaderResourceView(const ShaderResourceView& view)
	: view(nullptr),
	window(view.window),
	texture(view.texture)
{
	window->Resource_Copy(&view, this);
}
DXWindow::ShaderResourceView::ShaderResourceView(DXWindow& window, const Texture2D& texture)
	: view(nullptr),
	window(&window),
	texture(&texture)
{
	window.Resource_Add(this);
}
DXWindow::ShaderResourceView::~ShaderResourceView()
{
	if (window)
		window->Resource_Remove(this);
}
bool DXWindow::ShaderResourceView::Load()
{
	if (!window->IsOpen())
		return false;

	if (IsLoaded())
		return true;

	D3D11_TEXTURE2D_DESC texture_description;
	texture->GetHandle()->GetDesc(&texture_description);

	HRESULT                         hResult;
	D3D11_SHADER_RESOURCE_VIEW_DESC description =
	{
		.Format        = texture_description.Format,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D     =
		{
			.MostDetailedMip = 0,
			.MipLevels       = texture_description.MipLevels
		}
	};

	if (FAILED((hResult = window->d3d11_device->CreateShaderResourceView(texture->GetHandle(), &description, &view))))
		return false;

	return true;
}
void DXWindow::ShaderResourceView::Unload()
{
	if (view)
	{
		view->Release();
		view = nullptr;
	}
}
DXWindow::ShaderResourceView& DXWindow::ShaderResourceView::operator = (ShaderResourceView&& view)
{
	Unload();

	this->view    = view.view;
	this->window  = view.window;
	this->texture = view.texture;

	view.view    = nullptr;
	view.window  = nullptr;
	view.texture = nullptr;

	window->Resource_Move(&view, this);

	return *this;
}
DXWindow::ShaderResourceView& DXWindow::ShaderResourceView::operator = (const ShaderResourceView& view)
{
	Unload();

	this->view    = nullptr;
	this->window  = view.window;
	this->texture = view.texture;

	window->Resource_Copy(&view, this);

	return *this;
}

DXWindow::Transform DXWindow::Transform::Skew(float x, float y, const Vector2F& center)
{
	return D2D1::Matrix3x2F::Skew(x, y, { .x = center.X, .y = center.Y });
}
DXWindow::Transform DXWindow::Transform::Scale(float x, float y, const Vector2F& center)
{
	return D2D1::Matrix3x2F::Scale(x, y, { .x = center.X, .y = center.Y });
}
DXWindow::Transform DXWindow::Transform::Scale(const SizeF& size, const Vector2F& center)
{
	return D2D1::Matrix3x2F::Scale({ .width = size.Width, .height = size.Height }, { .x = center.X, .y = center.Y });
}
DXWindow::Transform DXWindow::Transform::Identity()
{
	return D2D1::Matrix3x2F::Identity();
}
DXWindow::Transform DXWindow::Transform::Rotation(float angle, const Vector2F& center)
{
	return D2D1::Matrix3x2F::Rotation(angle, { .x = center.X, .y = center.Y });
}
DXWindow::Transform DXWindow::Transform::Translation(float x, float y)
{
	return D2D1::Matrix3x2F::Translation(x, y);
}
DXWindow::Transform DXWindow::Transform::Translation(const SizeF& size)
{
	return D2D1::Matrix3x2F::Translation({ .width = size.Width, .height = size.Height });
}
DXWindow::Transform::Transform()
{
}
DXWindow::Transform::Transform(Transform&& transform)
	: matrix(std::move(transform.matrix))
{
}
DXWindow::Transform::Transform(const Transform& transform)
	: matrix(transform.matrix)
{
}
DXWindow::Transform::Transform(const D2D1::Matrix3x2F& matrix)
	: matrix(matrix)
{
}
DXWindow::Transform::Transform(const D2D1_MATRIX_3X2_F& matrix)
	: matrix(matrix._11, matrix._12, matrix._21, matrix._22, matrix._31, matrix._32)
{
}
DXWindow::Transform::~Transform()
{
}
bool DXWindow::Transform::IsIdentity() const
{
	return matrix.IsIdentity();
}
bool DXWindow::Transform::IsInvertible() const
{
	return matrix.IsInvertible();
}
float DXWindow::Transform::Determinant() const
{
	return matrix.Determinant();
}
bool DXWindow::Transform::Invert()
{
	return matrix.Invert();
}
void DXWindow::Transform::SetProduct(const Transform& a, const Transform& b)
{
	matrix.SetProduct(a.matrix, b.matrix);
}
DXWindow::Vector2F DXWindow::Transform::TransformPoint(const Vector2F& value) const
{
	auto point = matrix.TransformPoint({ .x = value.X, .y = value.Y });

	return { .X = point.x, .Y = point.y };
}
DXWindow::Transform::operator const D2D1::Matrix3x2F& () const
{
	return matrix;
}
DXWindow::Transform DXWindow::Transform::operator * (const Transform& value) const
{
	return matrix * value.matrix;
}
DXWindow::Transform& DXWindow::Transform::operator = (Transform&& transform)
{
	matrix = std::move(transform.matrix);

	return *this;
}
DXWindow::Transform& DXWindow::Transform::operator = (const Transform& transform)
{
	matrix = transform.matrix;

	return *this;
}
DXWindow::Transform& DXWindow::Transform::operator = (const D2D1::Matrix3x2F& matrix)
{
	this->matrix = matrix;

	return *this;
}
DXWindow::Transform& DXWindow::Transform::operator = (const D2D1_MATRIX_3X2_F& matrix)
{
	this->matrix = D2D1::Matrix3x2F(matrix._11, matrix._12, matrix._21, matrix._22, matrix._31, matrix._32);

	return *this;
}
bool DXWindow::Transform::operator == (const Transform& transform) const
{
	if (matrix._11 != transform.matrix._11) return false;
	if (matrix._12 != transform.matrix._12) return false;
	if (matrix._21 != transform.matrix._21) return false;
	if (matrix._22 != transform.matrix._22) return false;
	if (matrix._31 != transform.matrix._31) return false;
	if (matrix._32 != transform.matrix._32) return false;

	return true;
}
bool DXWindow::Transform::operator != (const Transform& transform) const
{
	return !operator==(transform);
}

DXWindow::DXWindow::DXWindow(std::wstring_view name, std::wstring_view title, uint32_t width, uint32_t height)
	: is_open(false),
	is_focus(false),
	is_closing(false),
	is_resizable(false),
	is_minimized(false),
	is_maximized(false),
	is_maximizable(false),
	is_vsync_enabled(true),
	is_resize_pending(false),
	is_content_loaded(false),
	is_mouse_in_client(false),
	is_mouse_track_enabled(false),
	clazz{
		.cbSize      = sizeof(WNDCLASSEXW),
		.style       = CS_CLASSDC,
		.lpfnWndProc = &DXWindow::WndProc,
		.hInstance   = GetModuleHandleW(NULL),
		.hCursor     = LoadCursorA(NULL, IDC_ARROW)
	},
	size{
		.Width  = width,
		.Height = height
	},
	name(name),
	title(title),
	position{
		.X = 100,
		.Y = 100
	},
	background_color(Color::CornflowerBlue),
	icon{
		.Type   = Icons::Application,
		.Handle = LoadIconA(NULL, IDI_APPLICATION)
	},
	cursor(nullptr),
	clipboard{
		.IsSet = false
	},
	wic_factory(nullptr),
	d2d1_factory(nullptr),
	d2d1_render_target(nullptr),
	d2d1_device_context(nullptr),
	d3d11_device(nullptr),
	d3d11_device_context(nullptr),
	d3d11_render_target_view(nullptr),
	dwrite_factory(nullptr),
	dsound_factory(nullptr),
	dxgi_swap_chain(nullptr)
{
	clazz.hIcon         = this->icon.Handle;
	clazz.hIconSm       = this->icon.Handle;
	clazz.lpszClassName = this->name.c_str();

	cursors.push({ .Type = Cursors::Arrow, .Handle = LoadCursorA(NULL, IDC_ARROW) });
	cursor = &cursors.top();

	Create();
}

DXWindow::DXWindow::~DXWindow()
{
	Destroy();
}

bool DXWindow::DXWindow::SetIcon(Icons value)
{
	if ((value == Icons::UserDefined) || (value >= Icons::COUNT))
		return false;

	icon.Type   = value;
	icon.Handle = WINDOW_ICONS[(size_t)value].LoadIconA();

	if (IsOpen())
	{
		if (!PostMessageW(handle, WM_SETICON, ICON_BIG, (LPARAM)icon.Handle))
			return false;

		if (!PostMessageW(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon.Handle))
			return false;
	}

	return true;
}
bool DXWindow::DXWindow::SetIcon(HICON value)
{
	icon.Type   = Icons::UserDefined;
	icon.Handle = value;

	if (IsOpen())
	{
		if (!PostMessageW(handle, WM_SETICON, ICON_BIG, (LPARAM)value))
			return false;

		if (!PostMessageW(handle, WM_SETICON, ICON_SMALL, (LPARAM)value))
			return false;
	}

	return true;
}

bool DXWindow::DXWindow::SetSize(uint32_t width, uint32_t height)
{
	if (IsOpen())
	{
		if (!MoveWindow(handle, position.X, position.Y, width, height, FALSE))
			return false;

		return true;
	}

	size.Width  = width;
	size.Height = height;

	return true;
}

void DXWindow::DXWindow::SetVSync(bool value)
{
	is_vsync_enabled = value;
}

void DXWindow::DXWindow::SetBackground(const Color& value)
{
	background_color = value;
}

bool DXWindow::DXWindow::PopClip()
{
	bool       aa;
	RectangleF bounds;

	return PopClip(bounds, aa);
}
bool DXWindow::DXWindow::PopClip(RectangleF& bounds, bool& aa)
{
	if (clips.empty())
		return false;

	auto& clip = clips.front();
	aa         = clip.AA;
	bounds     = clip.Bounds;

	clips.pop_front();

	if (d2d1_render_target)
		d2d1_render_target->PopAxisAlignedClip();

	return true;
}
void DXWindow::DXWindow::PushClip(const RectangleF& bounds, bool aa)
{
	clips.push_front({ .AA = aa, .Bounds = bounds });

	if (d2d1_render_target)
		d2d1_render_target->PushAxisAlignedClip(D2D1::RectF(bounds.Left, bounds.Top, bounds.Right, bounds.Bottom), aa ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
}

bool DXWindow::DXWindow::PopCursor()
{
	Cursors value;

	return PopCursor(value);
}
bool DXWindow::DXWindow::PopCursor(Cursors& value)
{
	if (cursors.size() == 1)
		return false;

	value = cursors.top().Type;
	cursors.pop();
	cursor = &cursors.top();

	if (IsOpen())
		SetCursor(cursor->Handle);

	return true;
}
bool DXWindow::DXWindow::PushCursor(Cursors value)
{
	if ((value == Cursors::UserDefined) || (value >= Cursors::COUNT))
		return false;

	cursors.push({
		.Type   = value,
		.Handle = WINDOW_CURSORS[(size_t)value].LoadCursorA()
	});

	cursor = &cursors.top();

	if (IsOpen())
		SetCursor(cursor->Handle);

	return true;
}
void DXWindow::DXWindow::PushCursor(HCURSOR value)
{
	cursors.push({
		.Type   = Cursors::UserDefined,
		.Handle = value
	});

	cursor = &cursors.top();

	if (IsOpen())
		SetCursor(value);
}

bool DXWindow::DXWindow::PopTransform()
{
	Transform value;

	return PopTransform(value);
}
bool DXWindow::DXWindow::PopTransform(Transform& value)
{
	if (transforms.size() == 1)
		return false;

	value = transforms.top().Original;
	transforms.pop();
	transform = &transforms.top();

	if (d2d1_render_target)
		d2d1_render_target->SetTransform(transform->Modified);

	return true;
}
void DXWindow::DXWindow::PushTransform(const Transform& value)
{
	transforms.push({
		.Original = value,
		.Modified = transform->Modified * value
	});
	transform = &transforms.top();

	if (d2d1_render_target)
		d2d1_render_target->SetTransform(transform->Modified);
}

bool DXWindow::DXWindow::DrawLine(const Vector2F* points, size_t count, const Brush& brush, float stroke_width)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!points)
		return false;

	if (!brush.IsLoaded())
		return false;

	for (size_t i = 1; i < count; ++i)
		d2d1_render_target->DrawLine(D2D1::Point2F(points[i - 1].X, points[i - 1].Y), D2D1::Point2F(points[i].X, points[i].Y), brush.GetHandle(), stroke_width);

	return true;
}
bool DXWindow::DXWindow::DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!bitmap.IsLoaded())
		return false;

	auto dst = D2D1::RectF(x, y, x + width, y + height);

	d2d1_render_target->DrawBitmap(bitmap.GetHandle(), dst, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	return true;
}
bool DXWindow::DXWindow::DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity, const RectangleF& source)
{
	return DrawBitmap(x, y, bitmap, width, height, opacity, source.Left, source.Top, source.GetWidth(), source.GetHeight());
}
bool DXWindow::DXWindow::DrawBitmap(float x, float y, const Bitmap& bitmap, float width, float height, float opacity, float source_x, float source_y, float source_width, float source_height)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!bitmap.IsLoaded())
		return false;

	auto dst = D2D1::RectF(x, y, x + width, y + height);
	auto src = D2D1::RectF(source_x, source_y, source_x + source_width, source_y + source_height);

	d2d1_render_target->DrawBitmap(bitmap.GetHandle(), dst, opacity, (D2D1_BITMAP_INTERPOLATION_MODE)bitmap.GetInterpolationMode(), src);

	return true;
}
bool DXWindow::DXWindow::DrawCircle(float x, float y, const Brush& brush, float radius, float stroke_width)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	d2d1_render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush.GetHandle(), stroke_width);

	return true;
}
bool DXWindow::DXWindow::DrawString(float x, float y, const Brush& brush, const TextFormat& format, std::wstring_view string)
{
	return DrawString(x, y, brush, format, string, resolution.Width, resolution.Height);
}
bool DXWindow::DXWindow::DrawString(float x, float y, const Brush& brush, const TextFormat& format, std::wstring_view string, float width, float height)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (!format.IsLoaded())
		return false;

	d2d1_render_target->DrawText(string.data(), string.length(), format.GetHandle(), D2D1::RectF(x, y, x + width, y + height), brush.GetHandle());

	return true;
}
bool DXWindow::DXWindow::DrawEllipse(float x, float y, const Brush& brush, float width, float height, float stroke_width)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	width  /= 2;
	height /= 2;

	d2d1_render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x + width, y + height), width, height), brush.GetHandle(), stroke_width);

	return true;
}
bool DXWindow::DXWindow::DrawGeometry(float x, float y, const Brush& brush, const PathGeometry& geometry, float stroke_width)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (!geometry.IsLoaded())
		return false;

	if (!geometry.IsFinalized())
		return false;

	D2D1::Matrix3x2F transform[2];
	d2d1_render_target->GetTransform(&transform[0]);
	transform[1] = transform[0] * D2D1::Matrix3x2F::Translation(x, y);

	d2d1_render_target->SetTransform(transform[1]);
	d2d1_render_target->DrawGeometry(geometry.GetHandle(), brush.GetHandle());
	d2d1_render_target->SetTransform(transform[0]);

	return true;
}
bool DXWindow::DXWindow::DrawRectangle(float x, float y, const Brush& brush, float width, float height, float stroke_width)
{
	return DrawRectangle(x, y, brush, width, height, stroke_width, 0);
}
bool DXWindow::DXWindow::DrawRectangle(float x, float y, const Brush& brush, float width, float height, float stroke_width, float corner_radius)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (corner_radius == 0)
		d2d1_render_target->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush.GetHandle(), stroke_width);
	else
		d2d1_render_target->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + width, y + height), corner_radius, corner_radius), brush.GetHandle(), stroke_width);

	return true;
}
bool DXWindow::DXWindow::DrawTextLayout(float x, float y, const Brush& brush, const TextLayout& layout)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (!layout.IsLoaded())
		return false;

	d2d1_render_target->DrawTextLayout(D2D1::Point2F(x, y), layout.GetHandle(), brush.GetHandle());

	return true;
}
bool DXWindow::DXWindow::DrawSolidCircle(float x, float y, const Brush& brush, float radius)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	d2d1_render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush.GetHandle());

	return true;
}
bool DXWindow::DXWindow::DrawSolidEllipse(float x, float y, const Brush& brush, float width, float height)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	width  /= 2;
	height /= 2;

	d2d1_render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x + width, y + height), width, height), brush.GetHandle());

	return true;
}
bool DXWindow::DXWindow::DrawSolidGeometry(float x, float y, const Brush& brush, const PathGeometry& geometry)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (!geometry.IsLoaded())
		return false;

	if (!geometry.IsFinalized())
		return false;

	D2D1::Matrix3x2F transform[2];
	d2d1_render_target->GetTransform(&transform[0]);
	transform[1] = transform[0] * D2D1::Matrix3x2F::Translation(x, y);

	d2d1_render_target->SetTransform(transform[1]);
	d2d1_render_target->FillGeometry(geometry.GetHandle(), brush.GetHandle());
	d2d1_render_target->SetTransform(transform[0]);

	return true;
}
bool DXWindow::DXWindow::DrawSolidRectangle(float x, float y, const Brush& brush, float width, float height)
{
	return DrawSolidRectangle(x, y, brush, width, height, 0);
}
bool DXWindow::DXWindow::DrawSolidRectangle(float x, float y, const Brush& brush, float width, float height, float corner_radius)
{
	if (!IsOpen())
		return false;

	if (!d2d1_render_target)
		return false;

	if (!brush.IsLoaded())
		return false;

	if (corner_radius == 0)
		d2d1_render_target->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush.GetHandle());
	else
		d2d1_render_target->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + width, y + height), corner_radius, corner_radius), brush.GetHandle());

	return true;
}

DXWindow::MessageBoxResults DXWindow::MessageBox(std::wstring_view title, std::wstring_view message, MessageBoxIcons icon, MessageBoxButtons buttons)
{
	UINT type = MB_TOPMOST | (UINT)icon | (UINT)buttons;

	return (MessageBoxResults)MessageBoxW(handle, message.data(), title.data(), type);
}
DXWindow::MessageBoxResults DXWindow::MessageBox(std::wstring_view title, std::wstring_view message, MessageBoxIcons icon, MessageBoxButtons buttons, MessageBoxDefaultButtons default_button)
{
	UINT type = MB_TOPMOST | (UINT)icon | (UINT)buttons;

	switch (buttons)
	{
		case MessageBoxButtons::Ok:
			type |= MB_DEFBUTTON1;
			break;

		case MessageBoxButtons::OkCancel:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Ok:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::Cancel:
					type |= MB_DEFBUTTON2;
					break;
			}
			break;

		case MessageBoxButtons::Help:
			type |= MB_DEFBUTTON1;
			break;

		case MessageBoxButtons::YesNo:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Yes:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::No:
					type |= MB_DEFBUTTON2;
					break;
			}
			break;

		case MessageBoxButtons::YesNoCancel:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Yes:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::No:
					type |= MB_DEFBUTTON2;
					break;

				case MessageBoxDefaultButtons::Cancel:
					type |= MB_DEFBUTTON3;
					break;
			}
			break;

		case MessageBoxButtons::RetryCancel:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Retry:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::Cancel:
					type |= MB_DEFBUTTON2;
					break;
			}
			break;

		case MessageBoxButtons::AbortRetryIgnore:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Abort:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::Retry:
					type |= MB_DEFBUTTON2;
					break;

				case MessageBoxDefaultButtons::Ignore:
					type |= MB_DEFBUTTON3;
					break;
			}
			break;

		case MessageBoxButtons::CancelTryContinue:
			switch (default_button)
			{
				case MessageBoxDefaultButtons::Cancel:
					type |= MB_DEFBUTTON1;
					break;

				case MessageBoxDefaultButtons::Try:
					type |= MB_DEFBUTTON2;
					break;

				case MessageBoxDefaultButtons::Continue:
					type |= MB_DEFBUTTON3;
					break;
			}
			break;
	}

	return (MessageBoxResults)MessageBoxW(handle, message.data(), title.data(), type);
}

DXWindow::FileDialogResult  DXWindow::OpenFileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags)
{
	return FileDialog(title, directory, filter, filter_count, flags, &GetOpenFileNameW);
}
DXWindow::FileDialogResult  DXWindow::SaveFileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags)
{
	return FileDialog(title, directory, filter, filter_count, flags, &GetSaveFileNameW);
}

int  DXWindow::DXWindow::Poll()
{
	if (!IsOpen())
		return -1;

	bool close = false;

	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.message == WM_QUIT)
			close = true;
	}

	is_focus = GetForegroundWindow() == handle;

	if (close)
	{
		Close();

		return -1;
	}

	if (!OnPoll())
		return 0;

	memcpy(mouse.buttons_down_prev, mouse.buttons_down, sizeof(mouse.buttons_down));
	memcpy(keyboard.keys_down_prev, keyboard.keys_down, sizeof(keyboard.keys_down));

	return 1;
}
bool DXWindow::DXWindow::Draw()
{
	if (!IsOpen())
		return false;

	if (!IsMinimized())
	{
		if (!Graphics_Target_Clear() || !OnDraw())
			return false;

#ifdef DXWINDOW_IMGUI
		ImGui::NewFrame();
#endif

		if (!OnDrawGUI())
			return false;

#ifdef DXWINDOW_IMGUI
		ImGui::Render();
#endif

		switch (Graphics_Target_Present())
		{
			case 0:
				return false;

			case -1:
				Resources_Unload(true);
				Graphics_Target_Destroy();
				if (!Graphics_Target_Create() || !Resources_Load(true))
					return false;
				break;
		}
	}

	return true;
}

void DXWindow::DXWindow::Close()
{
	if (IsOpen())
	{
		Resources_Unload(false);

		Audio_Destroy();
		Graphics_Destroy();

		is_open = false;
	}
}

bool DXWindow::DXWindow::Create()
{
	if (!(atom = RegisterClassExW(&clazz)))
		return false;

	if (!(handle = CreateWindowW(clazz.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, position.X, position.Y, size.Width, size.Height, nullptr, nullptr, clazz.hInstance, this)))
	{
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	HDC hDC;

	if (!(hDC = GetDC(handle)))
	{
		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	transforms.push({
		.Original = Transform::Identity(),
		.Modified = Transform::Identity()
	});
	transform = &transforms.top();

	RECT rect;
	GetWindowRect(handle, &rect);
	size.Width  = rect.right - rect.left;
	size.Height = rect.bottom - rect.top;
	position.X  = rect.left;
	position.Y  = rect.top;
	GetClientRect(handle, &rect);
	resolution.Width  = rect.right - rect.left;
	resolution.Height = rect.bottom - rect.top;

	if (!Audio_Create())
	{
		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	if (!Graphics_Create())
	{
		Audio_Destroy();

		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	if (!Graphics_Target_Create())
	{
		Graphics_Destroy();

		Audio_Destroy();

		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	SetCursor(cursor->Handle);
	ShowWindow(handle, SW_SHOWDEFAULT);
	DragAcceptFiles(handle, TRUE);

	if (!UpdateWindow(handle))
	{
		Graphics_Destroy();
		Audio_Destroy();

		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	if (!Resources_Load(false))
	{
		Graphics_Destroy();
		Audio_Destroy();

		DestroyWindow(handle);
		UnregisterClassW(clazz.lpszClassName, clazz.hInstance);

		return false;
	}

	is_open = true;

	return true;
}
void DXWindow::DXWindow::Destroy()
{
	if (IsOpen())
		Close();

	DestroyWindow(handle);
	UnregisterClassW(clazz.lpszClassName, clazz.hInstance);
}

bool DXWindow::DXWindow::Audio_Create()
{
	HRESULT hResult;

	if ((hResult = DirectSoundCreate8(nullptr, &dsound_factory, nullptr)) != DS_OK)
		return false;

	if ((hResult = dsound_factory->SetCooperativeLevel(handle, DSSCL_PRIORITY)) != DS_OK)
	{
		dsound_factory->Release();
		dsound_factory = nullptr;

		return false;
	}

	return true;
}
void DXWindow::DXWindow::Audio_Destroy()
{
	if (dsound_factory)
	{
		dsound_factory->Release();
		dsound_factory = nullptr;
	}
}

bool DXWindow::DXWindow::Graphics_Create()
{
	// Direct3D
	{
		const DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_description =
		{
			.BufferDesc =
			{
				.RefreshRate =
				{
					.Numerator   = 0,
					.Denominator = 1
				},
				.Format           = DXGI_FORMAT_B8G8R8A8_UNORM,
				.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED
			},
			.SampleDesc =
			{
				.Count   = 1,
				.Quality = 0
			},
			.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount  = 1,
			.OutputWindow = handle,
			.Windowed     = TRUE,
			.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD
		};

		static constexpr UINT              flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
		HRESULT                            hResult;
		static constexpr D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;

		if (FAILED((hResult = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, &feature_level, 1, D3D11_SDK_VERSION, &dxgi_swap_chain_description, &dxgi_swap_chain, &d3d11_device, nullptr, &d3d11_device_context))))
			return false;

		IDXGIDevice* dxgi_device;

		if (FAILED((hResult = d3d11_device->QueryInterface(&dxgi_device))))
		{
			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		IDXGIAdapter* dxgi_adapter;

		if (FAILED((hResult = dxgi_device->GetAdapter(&dxgi_adapter))))
		{
			dxgi_device->Release();

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		IDXGIFactory* dxgi_factory;

		if (FAILED((hResult = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)))))
		{
			dxgi_adapter->Release();
			dxgi_device->Release();

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		if (FAILED((hResult = dxgi_factory->MakeWindowAssociation(handle, DXGI_MWA_NO_WINDOW_CHANGES))))
		{
			dxgi_factory->Release();
			dxgi_adapter->Release();
			dxgi_device->Release();

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		dxgi_factory->Release();
		dxgi_adapter->Release();
		dxgi_device->Release();
	}

	// Direct2D
	{
		HRESULT hResult;

		if (FAILED((hResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d1_factory))))
		{
			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		if (FAILED((hResult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory1), (IUnknown**)&dwrite_factory))))
		{
			d2d1_factory->Release();
			d2d1_factory = nullptr;

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		if (FAILED((hResult = CoInitialize(nullptr))))
		{
			dwrite_factory->Release();
			dwrite_factory = nullptr;

			d2d1_factory->Release();
			d2d1_factory = nullptr;

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}

		if (FAILED((hResult = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory)))))
		{
			CoUninitialize();

			dwrite_factory->Release();
			dwrite_factory = nullptr;

			d2d1_factory->Release();
			d2d1_factory = nullptr;

			d3d11_device->Release();
			d3d11_device = nullptr;
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;

			return false;
		}
	}

#ifdef DXWINDOW_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(handle);
	// ImGui_ImplWin32_EnableDpiAwareness();
	ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);
	ImGui::GetIO().IniFilename = nullptr;
	// ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();
	// ImGui::StyleColorsClassic();
#endif

	return true;
}
void DXWindow::DXWindow::Graphics_Destroy()
{
	Graphics_Target_Destroy();

#ifdef DXWINDOW_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

	// Direct2D
	{
		if (wic_factory)
		{
			wic_factory->Release();
			wic_factory = nullptr;

			CoUninitialize();
		}

		if (dwrite_factory)
		{
			dwrite_factory->Release();
			dwrite_factory = nullptr;
		}

		if (d2d1_factory)
		{
			d2d1_factory->Release();
			d2d1_factory = nullptr;
		}
	}

	// Direct3D
	{
		if (d3d11_device)
		{
			d3d11_device->Release();
			d3d11_device = nullptr;
		}

		if (d3d11_device_context)
		{
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
		}

		if (dxgi_swap_chain)
		{
			dxgi_swap_chain->Release();
			dxgi_swap_chain = nullptr;
		}
	}
}
bool DXWindow::DXWindow::Graphics_Target_Create()
{
	if (!d2d1_factory || !d3d11_device || !d3d11_device_context || !dxgi_swap_chain)
		return false;

	// Direct3D
	{
		HRESULT          hResult;
		ID3D11Texture2D* d3d11_texture;

		if (FAILED((hResult = dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&d3d11_texture)))))
			return false;

		if (FAILED((hResult = d3d11_device->CreateRenderTargetView(d3d11_texture, nullptr, &d3d11_render_target_view))))
		{
			d3d11_texture->Release();

			return false;
		}

		d3d11_texture->Release();

		d3d11_device_context->OMSetRenderTargets(1, &d3d11_render_target_view, nullptr);
	}

	// Direct2D
	{
		HRESULT       hResult;
		IDXGISurface* dxgi_surface;

		if (FAILED((hResult = dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&dxgi_surface)))))
		{
			d3d11_render_target_view->Release();
			d3d11_render_target_view = nullptr;

			return false;
		}

		auto d2d1_render_target_properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		if (FAILED((hResult = d2d1_factory->CreateDxgiSurfaceRenderTarget(dxgi_surface, &d2d1_render_target_properties, &d2d1_render_target))))
		{
			dxgi_surface->Release();

			d3d11_render_target_view->Release();
			d3d11_render_target_view = nullptr;

			return false;
		}

		dxgi_surface->Release();

		if (FAILED((hResult = d2d1_render_target->QueryInterface(IID_PPV_ARGS(&d2d1_device_context)))))
		{
			d2d1_render_target->Release();
			d2d1_render_target = nullptr;

			d3d11_render_target_view->Release();
			d3d11_render_target_view = nullptr;

			return false;
		}

		for (auto& clip : clips)
			d2d1_render_target->PushAxisAlignedClip(D2D1::RectF(clip.Bounds.Left, clip.Bounds.Top, clip.Bounds.Right, clip.Bounds.Bottom), clip.AA ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
	}

	return true;
}
void DXWindow::DXWindow::Graphics_Target_Destroy()
{
	// Direct2D
	{
		if (d2d1_render_target)
		{
			d2d1_render_target->Release();
			d2d1_render_target = nullptr;
		}

		if (d2d1_device_context)
		{
			d2d1_device_context->Release();
			d2d1_device_context = nullptr;
		}
	}

	// Direct3D
	{
		if (d3d11_device_context)
			d3d11_device_context->OMSetRenderTargets(0, nullptr, nullptr);

		if (d3d11_render_target_view)
		{
			d3d11_render_target_view->Release();
			d3d11_render_target_view = nullptr;
		}
	}
}
bool DXWindow::DXWindow::Graphics_Target_Clear()
{
	if (!d2d1_render_target || !dxgi_swap_chain || !d3d11_render_target_view)
		return false;

	if (is_resize_pending)
	{
		is_resize_pending = false;

		Resources_Unload(true);
		Graphics_Target_Destroy();

		HRESULT hResult;

		if (FAILED((hResult = dxgi_swap_chain->ResizeBuffers(0, (UINT)resolution.Width, (UINT)resolution.Height, DXGI_FORMAT_UNKNOWN, 0))))
			return false;

		if (!Graphics_Target_Create())
			return false;

		if (!Resources_Load(true))
			return false;

		if (!OnResolutionChanged(resolution.Width, resolution.Height))
			return false;
	}

	const FLOAT color[] = { background_color.R, background_color.G, background_color.B, background_color.A };

	d3d11_device_context->ClearRenderTargetView(d3d11_render_target_view, color);
	d2d1_render_target->BeginDraw();
	d2d1_render_target->SetTransform(transform->Modified);

#ifdef DXWINDOW_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
#endif

	return true;
}
int  DXWindow::DXWindow::Graphics_Target_Present()
{
	if (!d2d1_render_target || !dxgi_swap_chain)
		return false;

	HRESULT hResult;

	if (FAILED((hResult = d2d1_render_target->EndDraw())))
	{
		if (hResult == D2DERR_RECREATE_TARGET)
			return -1;

		return 0;
	}

#ifdef DXWINDOW_IMGUI
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	if (FAILED((hResult = dxgi_swap_chain->Present(IsVSync() ? 1 : 0, 0))))
	{
		switch (hResult)
		{
			case DXGI_STATUS_OCCLUDED:
			case DXGI_ERROR_WAS_STILL_DRAWING:
				return 1;
		}

		return 0;
	}

	return 1;
}

bool DXWindow::DXWindow::Resources_Load(bool is_reload)
{
	is_content_loaded = true;

	for (auto it = resources.begin(); it != resources.end(); ++it)
	{
		auto resource = *it;

		if (is_reload && !resource->IsReloadable())
			;
		else if (!resource->Load())
		{
			is_content_loaded = false;

			while (it-- != resources.begin())
			{
				resource = *it;

				if (!is_reload || resource->IsReloadable())
					resource->Unload();
			}

			return false;
		}
	}

	return true;
}
void DXWindow::DXWindow::Resources_Unload(bool is_reload)
{
	for (auto it = resources.rbegin(); it != resources.rend(); ++it)
	{
		auto resource = *it;

		if (!is_reload || resource->IsReloadable())
			resource->Unload();
	}

	is_content_loaded = false;
}

bool DXWindow::DXWindow::Resource_Add(IResource* resource)
{
	if (d2d1_render_target && d3d11_render_target_view && !resource->Load())
		return false;

	resources.emplace(resource);

	return true;
}
bool DXWindow::DXWindow::Resource_Remove(IResource* resource)
{
	if (auto it = resources.find(resource); it != resources.end())
	{
		if (resource->IsLoaded())
			resource->Unload();

		resources.erase(it);

		return true;
	}

	return false;
}
void DXWindow::DXWindow::Resource_Move(IResource* source, IResource* destination)
{
	if (auto it = resources.find(source); it != resources.end())
		resources.erase(it);

	resources.emplace(destination);
}
bool DXWindow::DXWindow::Resource_Copy(const IResource* source, const IResource* destination)
{
	return Resource_Add((IResource*)destination);
}

DXWindow::FileDialogResult DXWindow::DXWindow::FileDialog(std::wstring_view title, std::wstring_view directory, const FileDialogFilter* filter, size_t filter_count, FileDialogFlags flags, BOOL(*function)(LPOPENFILENAMEW))
{
	size_t filter_size = 0;

	for (size_t i = 0; i < filter_count; ++i)
	{
		filter_size += filter[i].Key.length() + 1;
		filter_size += filter[i].Value.length() + 1;
	}

	size_t               filter_index = 0;
	std::vector<wchar_t> filter_buffer(filter_size + 2, L'\0');

	for (size_t i = 0; i < filter_count; ++i, ++filter)
	{
		memcpy(&filter_buffer[filter_index], filter->Key.data(), filter->Key.length() * sizeof(wchar_t));
		filter_index += filter->Key.length() + 1;

		memcpy(&filter_buffer[filter_index], filter->Value.data(), filter->Value.length() * sizeof(wchar_t));
		filter_index += filter->Value.length() + 1;
	}

	OPENFILENAMEW ofn =
	{
		.lStructSize     = sizeof(OPENFILENAMEW),
		.hwndOwner       = handle,
		.lpstrFilter     = filter_buffer.data(),
		.nFilterIndex    = (DWORD)(filter_count ? 1 : 0),
		.lpstrInitialDir = directory.data(),
		.lpstrTitle      = title.data(),
		.Flags           = (DWORD)(OFN_EXPLORER | OFN_ENABLEHOOK | (DWORD)flags),
		.lpfnHook        = &FileDialogProc
	};

	std::vector<wchar_t> ofn_buffer(MAX_PATH, L'\0');
	FileDialogResult     ofn_result = {};

	ofn.nMaxFile  = MAX_PATH;
	ofn.lpstrFile = ofn_buffer.data();
	ofn.lCustData = (LPARAM)&ofn_buffer;

	if (function(&ofn))
	{
		std::wstring_view path(ofn_buffer.data());

		for (size_t i = path.length() + 1, j = 0; i < ofn.nMaxFile; ++j)
		{
			if (!ofn_buffer[i])
			{
				if (!j)
					ofn_result.Paths.emplace_back(path);

				break;
			}

			std::wstring_view file_name(&ofn_buffer[i]);
			std::wstring      file_path(path.length() + 1 + file_name.length(), L'\\');
			memcpy(file_path.data(), path.data(), path.length() * sizeof(wchar_t));
			memcpy(&file_path[path.length() + 1], file_name.data(), file_name.length() * sizeof(wchar_t));

			i += file_name.length() + 1;
			ofn_result.Paths.push_back(std::move(file_path));
		}
	}
	else if (ofn_result.ErrorCode = CommDlgExtendedError())
		;

	return ofn_result;
}

LRESULT  CALLBACK DXWindow::DXWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef DXWINDOW_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return TRUE;
#endif

	static auto get_mouse_position = [](LPARAM lParam)
	{
		auto points = MAKEPOINTS(lParam);

		return Vector2F { .X = (float)points.x, .Y = (float)points.y };
	};

	static auto get_keyboard_key = [](WPARAM wParam)
	{
		switch (wParam)
		{
			case 0x41:        return Keys::A;
			case 0x42:        return Keys::B;
			case 0x43:        return Keys::C;
			case 0x44:        return Keys::D;
			case 0x45:        return Keys::E;
			case 0x46:        return Keys::F;
			case 0x47:        return Keys::G;
			case 0x48:        return Keys::H;
			case 0x49:        return Keys::I;
			case 0x4A:        return Keys::J;
			case 0x4B:        return Keys::K;
			case 0x4C:        return Keys::L;
			case 0x4D:        return Keys::M;
			case 0x4E:        return Keys::N;
			case 0x4F:        return Keys::O;
			case 0x50:        return Keys::P;
			case 0x51:        return Keys::Q;
			case 0x52:        return Keys::R;
			case 0x53:        return Keys::S;
			case 0x54:        return Keys::T;
			case 0x55:        return Keys::U;
			case 0x56:        return Keys::V;
			case 0x57:        return Keys::W;
			case 0x58:        return Keys::X;
			case 0x59:        return Keys::Y;
			case 0x5A:        return Keys::Z;
			case 0x30:        return Keys::Num0;
			case 0x31:        return Keys::Num1;
			case 0x32:        return Keys::Num2;
			case 0x33:        return Keys::Num3;
			case 0x34:        return Keys::Num4;
			case 0x35:        return Keys::Num5;
			case 0x36:        return Keys::Num6;
			case 0x37:        return Keys::Num7;
			case 0x38:        return Keys::Num8;
			case 0x39:        return Keys::Num9;
			case VK_NUMPAD0:  return Keys::NumPad0;
			case VK_NUMPAD1:  return Keys::NumPad1;
			case VK_NUMPAD2:  return Keys::NumPad2;
			case VK_NUMPAD3:  return Keys::NumPad3;
			case VK_NUMPAD4:  return Keys::NumPad4;
			case VK_NUMPAD5:  return Keys::NumPad5;
			case VK_NUMPAD6:  return Keys::NumPad6;
			case VK_NUMPAD7:  return Keys::NumPad7;
			case VK_NUMPAD8:  return Keys::NumPad8;
			case VK_NUMPAD9:  return Keys::NumPad9;
			case VK_F1:       return Keys::F1;
			case VK_F2:       return Keys::F2;
			case VK_F3:       return Keys::F3;
			case VK_F4:       return Keys::F4;
			case VK_F5:       return Keys::F5;
			case VK_F6:       return Keys::F6;
			case VK_F7:       return Keys::F7;
			case VK_F8:       return Keys::F8;
			case VK_F9:       return Keys::F9;
			case VK_F10:      return Keys::F10;
			case VK_F11:      return Keys::F11;
			case VK_F12:      return Keys::F12;
			case VK_OEM_3:    return Keys::Tilde;
			case VK_MENU:     return Keys::Alt;
			case VK_LMENU:    return Keys::LAlt;
			case VK_RMENU:    return Keys::RAlt;
			case VK_CLEAR:    return Keys::Clear;
			case VK_DELETE:   return Keys::Delete;
			case VK_RETURN:   return Keys::Enter;
			case VK_ESCAPE:   return Keys::Escape;
			case VK_BACK:     return Keys::Backspace;
			case VK_CONTROL:  return Keys::Control;
			case VK_LCONTROL: return Keys::LControl;
			case VK_RCONTROL: return Keys::RControl;
			case VK_SHIFT:    return Keys::Shift;
			case VK_LSHIFT:   return Keys::LShift;
			case VK_RSHIFT:   return Keys::RShift;
			case VK_SPACE:    return Keys::Spacebar;
			case VK_TAB:      return Keys::Tab;
			case VK_UP:       return Keys::Up;
			case VK_DOWN:     return Keys::Down;
			case VK_LEFT:     return Keys::Left;
			case VK_RIGHT:    return Keys::Right;
		}

		return Keys::COUNT;
	};

	switch (msg)
	{
		case WM_CUT:
			// TODO: implement
			// https://learn.microsoft.com/en-us/windows/win32/dataxchg/wm-cut
			break;

		case WM_CLEAR:
			// TODO: implement
			// https://learn.microsoft.com/en-us/windows/win32/dataxchg/wm-clear
			break;

		case WM_COPY:
			// TODO: implement
			// https://learn.microsoft.com/en-us/windows/win32/dataxchg/wm-copy
			break;

		case WM_PASTE:
			// TODO: implement
			// https://learn.microsoft.com/en-us/windows/win32/dataxchg/wm-paste
			break;

		case WM_SETICON:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
				switch (wParam)
				{
					case ICON_BIG:   window->clazz.hIcon   = (HICON)lParam; break;
					case ICON_SMALL: window->clazz.hIconSm = (HICON)lParam; break;
				}
			break;

		case WM_SETCURSOR:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
				if (window->is_mouse_in_client)
					return TRUE;
			break;

		case WM_DROPFILES:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				UINT file_count = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, NULL, 0);

				for (UINT i = 0; i < file_count; ++i)
				{
					std::wstring file_path(DragQueryFileW((HDROP)wParam, i, NULL, 0) + 1, L'\0');
					POINT        file_drop_position;

					DragQueryFileW((HDROP)wParam, i, file_path.data(), (UINT)file_path.length());
					DragQueryPoint((HDROP)wParam, &file_drop_position);

					if (!window->OnDropFile({ .X = (float)file_drop_position.x, .Y = (float)file_drop_position.y }, file_path))
					{
						DragFinish((HDROP)wParam);
						PostQuitMessage(0);

						return 0;
					}
				}

				DragFinish((HDROP)wParam);

				return 0;
			}
			break;

		case WM_CLIPBOARDUPDATE:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				int  format;
				UINT formats[] = { CF_TEXT, CF_UNICODETEXT };

				window->clipboard.IsSet = false;

				if ((format = GetPriorityClipboardFormat(formats, sizeof(formats) / sizeof(UINT))) > 0)
					if (OpenClipboard(hWnd))
					{
						if (auto hValue = GetClipboardData((UINT)format))
							if (auto value = GlobalLock(hValue))
							{
								switch (format)
								{
									case CF_TEXT:
										if (int length; (length = MultiByteToWideChar(CP_ACP, 0, (const char*)value, -1, NULL, 0)) > 0)
										{
											window->clipboard.IsSet = true;
											window->clipboard.String.resize(length - 1);

											MultiByteToWideChar(CP_ACP, 0, (const char*)value, -1, window->clipboard.String.data(), length);
										}
										break;

									case CF_UNICODETEXT:
										window->clipboard.IsSet = true;
										window->clipboard.String.assign((const wchar_t*)value);
										break;
								}

								GlobalUnlock(hValue);
							}

						CloseClipboard();
					}
			}
			break;

		case WM_DESTROYCLIPBOARD:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->clipboard.IsSet = false;
				window->clipboard.String.clear();
			}
			break;

		case WM_CHAR:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
				if (std::iswprint(wParam) && !window->OnKeyboardChar((wchar_t)wParam))
					window->Close();
			break;

		case WM_KEYUP:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				Keys key;

				if ((key = get_keyboard_key(wParam)) != Keys::COUNT)
				{
					window->keyboard.keys_down_prev[(int)key] = window->keyboard.keys_down[(int)key];
					window->keyboard.keys_down[(int)key]      = false;

					if (!window->OnKeyboardKeyUp(key))
						window->Close();
				}
			}
			break;

		case WM_KEYDOWN:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				Keys key;

				if ((key = get_keyboard_key(wParam)) != Keys::COUNT)
				{
					window->keyboard.keys_down_prev[(int)key] = window->keyboard.keys_down[(int)key];
					window->keyboard.keys_down[(int)key]      = true;

					if (!window->OnKeyboardKeyDown(key))
						window->Close();
				}
			}
			break;

		case WM_MOUSEMOVE:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->is_mouse_in_client = true;

				if (!window->is_mouse_track_enabled)
				{
					TRACKMOUSEEVENT event =
					{
						.cbSize    = sizeof(TRACKMOUSEEVENT),
						.dwFlags   = TME_LEAVE,
						.hwndTrack = hWnd
					};

					TrackMouseEvent(&event);

					window->is_mouse_track_enabled = true;

					SetCursor(window->cursor->Handle);
				}

				window->mouse.position = get_mouse_position(lParam);

				if (!window->OnMouseMove(window->mouse.position))
					window->Close();
			}
			break;

		case WM_MOUSELEAVE:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->is_mouse_in_client     = false;
				window->is_mouse_track_enabled = false;
			}
			break;

		case WM_MOUSEWHEEL:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				POINT point = { LOWORD(lParam), HIWORD(lParam) };

				if (ScreenToClient(hWnd, &point))
				{
					window->mouse.position = { .X = (float)point.x, .Y = (float)point.y };

					if (!window->OnMouseScroll(window->mouse.position, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA))
						window->Close();
				}
			}
			break;

		case WM_LBUTTONUP:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                   = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Left] = window->mouse.buttons_down[(int)MouseButtons::Left];
				window->mouse.buttons_down[(int)MouseButtons::Left]      = false;

				if (!window->OnMouseButtonUp(window->mouse.position, MouseButtons::Left))
					window->Close();
			}
			break;
		case WM_LBUTTONDOWN:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                   = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Left] = window->mouse.buttons_down[(int)MouseButtons::Left];
				window->mouse.buttons_down[(int)MouseButtons::Left]      = true;

				if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::Left))
					window->Close();
			}
			break;

		case WM_RBUTTONUP:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                    = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Right] = window->mouse.buttons_down[(int)MouseButtons::Right];
				window->mouse.buttons_down[(int)MouseButtons::Right]      = false;

				if (!window->OnMouseButtonUp(window->mouse.position, MouseButtons::Right))
					window->Close();
			}
			break;
		case WM_RBUTTONDOWN:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                    = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Right] = window->mouse.buttons_down[(int)MouseButtons::Right];
				window->mouse.buttons_down[(int)MouseButtons::Right]      = true;

				if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::Right))
					window->Close();
			}
			break;

		case WM_MBUTTONUP:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                     = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Middle] = window->mouse.buttons_down[(int)MouseButtons::Middle];
				window->mouse.buttons_down[(int)MouseButtons::Middle]      = false;

				if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::Middle))
					window->Close();
			}
			break;
		case WM_MBUTTONDOWN:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position                                     = get_mouse_position(lParam);
				window->mouse.buttons_down_prev[(int)MouseButtons::Middle] = window->mouse.buttons_down[(int)MouseButtons::Middle];
				window->mouse.buttons_down[(int)MouseButtons::Middle]      = true;

				if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::Middle))
					window->Close();
			}
			break;

		case WM_XBUTTONUP:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position = get_mouse_position(lParam);

				switch (GET_XBUTTON_WPARAM(wParam))
				{
					case XBUTTON1:
						window->mouse.buttons_down_prev[(int)MouseButtons::X1] = window->mouse.buttons_down[(int)MouseButtons::X1];
						window->mouse.buttons_down[(int)MouseButtons::X1]      = false;

						if (!window->OnMouseButtonUp(window->mouse.position, MouseButtons::X1))
							window->Close();
						break;

					case XBUTTON2:
						window->mouse.buttons_down_prev[(int)MouseButtons::X2] = window->mouse.buttons_down[(int)MouseButtons::X2];
						window->mouse.buttons_down[(int)MouseButtons::X2]      = false;

						if (!window->OnMouseButtonUp(window->mouse.position, MouseButtons::X2))
							window->Close();
						break;
				}
			}
			break;
		case WM_XBUTTONDOWN:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->mouse.position = get_mouse_position(lParam);

				switch (GET_XBUTTON_WPARAM(wParam))
				{
					case XBUTTON1:
						window->mouse.buttons_down_prev[(int)MouseButtons::X1] = window->mouse.buttons_down[(int)MouseButtons::X1];
						window->mouse.buttons_down[(int)MouseButtons::X1]      = true;

						if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::X1))
							window->Close();
						break;

					case XBUTTON2:
						window->mouse.buttons_down_prev[(int)MouseButtons::X2] = window->mouse.buttons_down[(int)MouseButtons::X2];
						window->mouse.buttons_down[(int)MouseButtons::X2]      = true;

						if (!window->OnMouseButtonDown(window->mouse.position, MouseButtons::X2))
							window->Close();
						break;
				}
			}
			break;

		case WM_MOVE:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				window->position.X = LOWORD(lParam);
				window->position.Y = HIWORD(lParam);
			}
			break;

		case WM_SIZE:
			if (auto window = (DXWindow*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			{
				auto window_resize = [](DXWindow* window, HWND hWnd, LPARAM lParam)
				{
					RECT rect = {};
					GetWindowRect(hWnd, &rect);

					if (((rect.right - rect.left) != window->size.Width) ||
						(rect.bottom - rect.top) != window->size.Height)
					{
						window->size.Width        = rect.right - rect.left;
						window->size.Height       = rect.bottom - rect.top;
						window->resolution.Width  = LOWORD(lParam);
						window->resolution.Height = HIWORD(lParam);
						window->is_resize_pending = true;
					}
				};

				switch (wParam)
				{
					case SIZE_MAXIMIZED:
						window->is_minimized = false;
						window->is_maximized = true;
						window_resize(window, hWnd, lParam);
						break;

					case SIZE_RESTORED:
						window->is_minimized = false;
						window->is_maximized = false;
						window_resize(window, hWnd, lParam);
						break;

					case SIZE_MINIMIZED:
						window->is_minimized = true;
						window->is_maximized = false;
						break;
				}
			}
			break;

		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_KEYMENU)
				return 0;
			break;

		case WM_CREATE:
			if (auto create = (CREATESTRUCTA*)lParam)
				if (auto window = (DXWindow*)create->lpCreateParams)
				{
					SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)window);
					AddClipboardFormatListener(hWnd);
				}
			break;

		case WM_DESTROY:
			RemoveClipboardFormatListener(hWnd);
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}
UINT_PTR CALLBACK DXWindow::DXWindow::FileDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NOTIFY)
		if (auto notify = (LPOFNOTIFYW)lParam; notify->hdr.code == CDN_SELCHANGE)
		{
			auto  ofn    = notify->lpOFN;
			auto  buffer = (std::vector<wchar_t>*)ofn->lCustData;
			DWORD length = MAX_PATH + CommDlg_OpenSave_GetSpecW(GetParent(hWnd), NULL, 0);

			if (length > ofn->nMaxFile)
			{
				buffer->resize(length);

				ofn->nMaxFile  = length;
				ofn->lpstrFile = buffer->data();
			}
		}

	return 0;
}
