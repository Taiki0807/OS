typedef unsigned short CHAR16;
typedef unsigned long long EFI_STATUS;
typedef void *EFI_HANDLE;

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
struct EFI_BOOT_SERVICES;

struct  EFI_INPUT_KEY
{
  unsigned short ScanCode;
  unsigned short UnicodeChar;
};
struct EFI_GUID {
	unsigned int Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
};
typedef EFI_STATUS (*EFI_TEXT_STRING)(
  struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This,
  CHAR16                                   *String);
typedef EFI_STATUS (*ReadKeyStroke)(
  struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  struct EFI_INPUT_KEY *key);

typedef EFI_STATUS (*ClearScreen) (
      struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This);

typedef EFI_STATUS (*SetWatchdog_Timer)(
  EFI_STATUS Timeout,
  EFI_STATUS WatchdogCode,
  EFI_STATUS DataSize,
  CHAR16 *WatchdogData);
typedef EFI_STATUS (*Locate_Protocol)(
  struct EFI_GUID *Protocol,
  void *Registration,
  void **Interface);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  void             *dummy;
    EFI_TEXT_STRING  OutputString;
    EFI_STATUS dummy2[4];
    ClearScreen Clear_Screen;
    
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL
{
  EFI_STATUS *dummy;
  ReadKeyStroke Inputkey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct _EFI_BOOT_SERVICES
{
  char _buf1[24];
  unsigned long long _buf2[2];
  unsigned long long _buf3[5];
  unsigned long long _buf4[2];
  unsigned long long _buf4_2[4];
  unsigned long long _buf5[9];
  unsigned long long _buf6[5];
  unsigned long long _buf7[2];
  SetWatchdog_Timer SetWatchdogTimer;
  unsigned long long _buf8[2];
  unsigned long long _buf9[3];
  unsigned long long _buf10[2];
  Locate_Protocol LocateProtocol;
  unsigned long long _buf10_2[2];
  unsigned long long _buf11;
  unsigned long long _buf12[3];
} EFI_BOOT_SERVICES;
struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
};

struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
	unsigned long long _buf[3];
	struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
		unsigned int MaxMode;
		unsigned int Mode;
		struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
			unsigned int Version;
			unsigned int HorizontalResolution;
			unsigned int VerticalResolution;
			enum EFI_GRAPHICS_PIXEL_FORMAT {
				PixelRedGreenBlueReserved8BitPerColor,
				PixelBlueGreenRedReserved8BitPerColor,
				PixelBitMask,
				PixelBltOnly,
				PixelFormatMax
			} PixelFormat;
		} *Info;
		unsigned long long SizeOfInfo;
		unsigned long long FrameBufferBase;
	} *Mode;
};

typedef struct {
    char                            dummy[44];
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_STATUS dummy_2;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
    EFI_STATUS dummy_3[3];
    EFI_BOOT_SERVICES *BootServices;
}EFI_SYSTEM_TABLE;

const struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL white = {0xff, 0x00, 0x00, 0xff};
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
void draw_pixel(unsigned int x, unsigned int y,
		struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	unsigned int hr = GOP->Mode->Info->HorizontalResolution;
	struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL *base =
		(struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)GOP->Mode->FrameBufferBase;
	struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL *p = base + (hr * y) + x;

	p->Blue = color.Blue;
	p->Green = color.Green;
	p->Red = color.Red;
	p->Reserved = color.Reserved;
}
struct RECT {
	unsigned int x, y;
	unsigned int w, h;
};

void draw_rect(struct RECT r, struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL c)
{
	unsigned int i;

	for (i = r.x; i < (r.x + r.w); i++)
		draw_pixel(i, r.y, c);
	for (i = r.x; i < (r.x + r.w); i++)
		draw_pixel(i, r.y + r.h - 1, c);

	for (i = r.y; i < (r.y + r.h); i++)
		draw_pixel(r.x, i, c);
	for (i = r.y; i < (r.y + r.h); i++)
		draw_pixel(r.x + r.w - 1, i, c);
}

EFI_STATUS EfiMain(EFI_HANDLE        ImageHandle,
                   EFI_SYSTEM_TABLE  *SystemTable) {
    struct EFI_INPUT_KEY key;
    CHAR16 str[3];
    SystemTable->ConOut->Clear_Screen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"hello world \r\n");
    struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38,{0x96, 0xfb, 0x7a, 0xde,0xd0, 0x80, 0x51, 0x6a}};
    SystemTable->BootServices->SetWatchdogTimer(0,0,0,(void *)0);
    SystemTable->BootServices->LocateProtocol(&gop_guid, (void *)0, (void **)&GOP);
    struct RECT r = {10, 10, 100, 200};
    draw_rect(r, white);
    while (1){
      if(!SystemTable->ConIn->Inputkey(SystemTable->ConIn,&key)) {
        if(key.UnicodeChar != L'\r') {
          str[0] = key.UnicodeChar;
          str[1] = L'\0';
        }else {
          str[0] = L'\r';
          str[1] = L'\n';
          str[2] = L'\0';
        }
        SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
      }
    }
    return 0;
}