typedef unsigned short CHAR16;
typedef unsigned long long EFI_STATUS;
typedef void *EFI_HANDLE;

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

struct  EFI_INPUT_KEY
{
  unsigned short ScanCode;
  unsigned short UnicodeChar;
};
typedef EFI_STATUS (*EFI_TEXT_STRING)(
  struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This,
  CHAR16                                   *String);
typedef EFI_STATUS (*ReadKeyStroke)(
  struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  struct EFI_INPUT_KEY *key);

typedef EFI_STATUS (*ClearScreen) (
      struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This);

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


typedef struct {
    char                            dummy[44];
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_STATUS dummy_2;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
}EFI_SYSTEM_TABLE;

EFI_STATUS EfiMain(EFI_HANDLE        ImageHandle,
                   EFI_SYSTEM_TABLE  *SystemTable) {
    struct EFI_INPUT_KEY key;
    CHAR16 str[3];
    SystemTable->ConOut->Clear_Screen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"hello world \r\n");
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