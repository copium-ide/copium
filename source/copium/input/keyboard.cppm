module;
#include <cstdint>
#include <SDL3/SDL.h>
export module copium.input.keyboard.layout;

export class KeyboardListener
{

};

export class KeySymbol
{
public:
    using SDL_Keycode = uint32_t;
    
    static constexpr SDL_Keycode EXTENDED_MASK = (1u << 29);
    static constexpr SDL_Keycode SCANCODE_MASK = (1u << 30);
    
    static constexpr SDL_Keycode SCANCODE_TO_KEYCODE(SDL_Keycode X) {
        return X | SCANCODE_MASK;
    }
    
    static constexpr SDL_Keycode UNKNOWN = 0x00000000u; /**< 0 */
    static constexpr SDL_Keycode RETURN = 0x0000000du; /**< '\r' */
    static constexpr SDL_Keycode ESCAPE = 0x0000001bu; /**< '\x1B' */
    static constexpr SDL_Keycode BACKSPACE = 0x00000008u; /**< '\b' */
    static constexpr SDL_Keycode TAB = 0x00000009u; /**< '\t' */
    static constexpr SDL_Keycode SPACE = 0x00000020u; /**< ' ' */
    static constexpr SDL_Keycode EXCLAIM = 0x00000021u; /**< '!' */
    static constexpr SDL_Keycode DBLAPOSTROPHE = 0x00000022u; /**< '"' */
    static constexpr SDL_Keycode HASH = 0x00000023u; /**< '#' */
    static constexpr SDL_Keycode DOLLAR = 0x00000024u; /**< '$' */
    static constexpr SDL_Keycode PERCENT = 0x00000025u; /**< '%' */
    static constexpr SDL_Keycode AMPERSAND = 0x00000026u; /**< '&' */
    static constexpr SDL_Keycode APOSTROPHE = 0x00000027u; /**< '\'' */
    static constexpr SDL_Keycode LEFTPAREN = 0x00000028u; /**< '(' */
    static constexpr SDL_Keycode RIGHTPAREN = 0x00000029u; /**< ')' */
    static constexpr SDL_Keycode ASTERISK = 0x0000002au; /**< '*' */
    static constexpr SDL_Keycode PLUS = 0x0000002bu; /**< '+' */
    static constexpr SDL_Keycode COMMA = 0x0000002cu; /**< ',' */
    static constexpr SDL_Keycode MINUS = 0x0000002du; /**< '-' */
    static constexpr SDL_Keycode PERIOD = 0x0000002eu; /**< '.' */
    static constexpr SDL_Keycode SLASH = 0x0000002fu; /**< '/' */
    static constexpr SDL_Keycode N0 = 0x00000030u; /**< '0' */
    static constexpr SDL_Keycode N1 = 0x00000031u; /**< '1' */
    static constexpr SDL_Keycode N2 = 0x00000032u; /**< '2' */
    static constexpr SDL_Keycode N3 = 0x00000033u; /**< '3' */
    static constexpr SDL_Keycode N4 = 0x00000034u; /**< '4' */
    static constexpr SDL_Keycode N5 = 0x00000035u; /**< '5' */
    static constexpr SDL_Keycode N6 = 0x00000036u; /**< '6' */
    static constexpr SDL_Keycode N7 = 0x00000037u; /**< '7' */
    static constexpr SDL_Keycode N8 = 0x00000038u; /**< '8' */
    static constexpr SDL_Keycode N9 = 0x00000039u; /**< '9' */
    static constexpr SDL_Keycode COLON = 0x0000003au; /**< ':' */
    static constexpr SDL_Keycode SEMICOLON = 0x0000003bu; /**< ';' */
    static constexpr SDL_Keycode LESS = 0x0000003cu; /**< '<' */
    static constexpr SDL_Keycode EQUALS = 0x0000003du; /**< '=' */
    static constexpr SDL_Keycode GREATER = 0x0000003eu; /**< '>' */
    static constexpr SDL_Keycode QUESTION = 0x0000003fu; /**< '?' */
    static constexpr SDL_Keycode AT = 0x00000040u; /**< '@' */
    static constexpr SDL_Keycode LEFTBRACKET = 0x0000005bu; /**< '[' */
    static constexpr SDL_Keycode BACKSLASH = 0x0000005cu; /**< '\\' */
    static constexpr SDL_Keycode RIGHTBRACKET = 0x0000005du; /**< ']' */
    static constexpr SDL_Keycode CARET = 0x0000005eu; /**< '^' */
    static constexpr SDL_Keycode UNDERSCORE = 0x0000005fu; /**< '_' */
    static constexpr SDL_Keycode GRAVE = 0x00000060u; /**< '`' */
    static constexpr SDL_Keycode A = 0x00000061u; /**< 'a' */
    static constexpr SDL_Keycode B = 0x00000062u; /**< 'b' */
    static constexpr SDL_Keycode C = 0x00000063u; /**< 'c' */
    static constexpr SDL_Keycode D = 0x00000064u; /**< 'd' */
    static constexpr SDL_Keycode E = 0x00000065u; /**< 'e' */
    static constexpr SDL_Keycode F = 0x00000066u; /**< 'f' */
    static constexpr SDL_Keycode G = 0x00000067u; /**< 'g' */
    static constexpr SDL_Keycode H = 0x00000068u; /**< 'h' */
    static constexpr SDL_Keycode I = 0x00000069u; /**< 'i' */
    static constexpr SDL_Keycode J = 0x0000006au; /**< 'j' */
    static constexpr SDL_Keycode K = 0x0000006bu; /**< 'k' */
    static constexpr SDL_Keycode L = 0x0000006cu; /**< 'l' */
    static constexpr SDL_Keycode M = 0x0000006du; /**< 'm' */
    static constexpr SDL_Keycode N = 0x0000006eu; /**< 'n' */
    static constexpr SDL_Keycode O = 0x0000006fu; /**< 'o' */
    static constexpr SDL_Keycode P = 0x00000070u; /**< 'p' */
    static constexpr SDL_Keycode Q = 0x00000071u; /**< 'q' */
    static constexpr SDL_Keycode R = 0x00000072u; /**< 'r' */
    static constexpr SDL_Keycode S = 0x00000073u; /**< 's' */
    static constexpr SDL_Keycode T = 0x00000074u; /**< 't' */
    static constexpr SDL_Keycode U = 0x00000075u; /**< 'u' */
    static constexpr SDL_Keycode V = 0x00000076u; /**< 'v' */
    static constexpr SDL_Keycode W = 0x00000077u; /**< 'w' */
    static constexpr SDL_Keycode X = 0x00000078u; /**< 'x' */
    static constexpr SDL_Keycode Y = 0x00000079u; /**< 'y' */
    static constexpr SDL_Keycode Z = 0x0000007au; /**< 'z' */
    static constexpr SDL_Keycode LEFTBRACE = 0x0000007bu; /**< '{' */
    static constexpr SDL_Keycode PIPE = 0x0000007cu; /**< '|' */
    static constexpr SDL_Keycode RIGHTBRACE = 0x0000007du; /**< '}' */
    static constexpr SDL_Keycode TILDE = 0x0000007eu; /**< '~' */
    static constexpr SDL_Keycode DELETE = 0x0000007fu; /**< '\x7F' */
    static constexpr SDL_Keycode PLUSMINUS = 0x000000b1u; /**< '\xB1' */
    static constexpr SDL_Keycode CAPSLOCK = 0x40000039u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK) */
    static constexpr SDL_Keycode F1 = 0x4000003au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1) */
    static constexpr SDL_Keycode F2 = 0x4000003bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2) */
    static constexpr SDL_Keycode F3 = 0x4000003cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3) */
    static constexpr SDL_Keycode F4 = 0x4000003du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4) */
    static constexpr SDL_Keycode F5 = 0x4000003eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5) */
    static constexpr SDL_Keycode F6 = 0x4000003fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6) */
    static constexpr SDL_Keycode F7 = 0x40000040u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7) */
    static constexpr SDL_Keycode F8 = 0x40000041u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8) */
    static constexpr SDL_Keycode F9 = 0x40000042u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9) */
    static constexpr SDL_Keycode F10 = 0x40000043u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10) */
    static constexpr SDL_Keycode F11 = 0x40000044u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11) */
    static constexpr SDL_Keycode F12 = 0x40000045u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12) */
    static constexpr SDL_Keycode PRINTSCREEN = 0x40000046u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN) */
    static constexpr SDL_Keycode SCROLLLOCK = 0x40000047u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK) */
    static constexpr SDL_Keycode PAUSE = 0x40000048u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE) */
    static constexpr SDL_Keycode INSERT = 0x40000049u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT) */
    static constexpr SDL_Keycode HOME = 0x4000004au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME) */
    static constexpr SDL_Keycode PAGEUP = 0x4000004bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP) */
    static constexpr SDL_Keycode END = 0x4000004du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END) */
    static constexpr SDL_Keycode PAGEDOWN = 0x4000004eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN) */
    static constexpr SDL_Keycode RIGHT = 0x4000004fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT) */
    static constexpr SDL_Keycode LEFT = 0x40000050u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT) */
    static constexpr SDL_Keycode DOWN = 0x40000051u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN) */
    static constexpr SDL_Keycode UP = 0x40000052u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP) */
    static constexpr SDL_Keycode NUMLOCKCLEAR = 0x40000053u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR) */
    static constexpr SDL_Keycode KP_DIVIDE = 0x40000054u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE) */
    static constexpr SDL_Keycode KP_MULTIPLY = 0x40000055u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY) */
    static constexpr SDL_Keycode KP_MINUS = 0x40000056u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS) */
    static constexpr SDL_Keycode KP_PLUS = 0x40000057u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS) */
    static constexpr SDL_Keycode KP_ENTER = 0x40000058u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER) */
    static constexpr SDL_Keycode KP_1 = 0x40000059u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1) */
    static constexpr SDL_Keycode KP_2 = 0x4000005au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2) */
    static constexpr SDL_Keycode KP_3 = 0x4000005bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3) */
    static constexpr SDL_Keycode KP_4 = 0x4000005cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4) */
    static constexpr SDL_Keycode KP_5 = 0x4000005du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5) */
    static constexpr SDL_Keycode KP_6 = 0x4000005eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6) */
    static constexpr SDL_Keycode KP_7 = 0x4000005fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7) */
    static constexpr SDL_Keycode KP_8 = 0x40000060u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8) */
    static constexpr SDL_Keycode KP_9 = 0x40000061u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9) */
    static constexpr SDL_Keycode KP_0 = 0x40000062u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0) */
    static constexpr SDL_Keycode KP_PERIOD = 0x40000063u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD) */
    static constexpr SDL_Keycode APPLICATION = 0x40000065u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION) */
    static constexpr SDL_Keycode POWER = 0x40000066u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER) */
    static constexpr SDL_Keycode KP_EQUALS = 0x40000067u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS) */
    static constexpr SDL_Keycode F13 = 0x40000068u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13) */
    static constexpr SDL_Keycode F14 = 0x40000069u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14) */
    static constexpr SDL_Keycode F15 = 0x4000006au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15) */
    static constexpr SDL_Keycode F16 = 0x4000006bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16) */
    static constexpr SDL_Keycode F17 = 0x4000006cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17) */
    static constexpr SDL_Keycode F18 = 0x4000006du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18) */
    static constexpr SDL_Keycode F19 = 0x4000006eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19) */
    static constexpr SDL_Keycode F20 = 0x4000006fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20) */
    static constexpr SDL_Keycode F21 = 0x40000070u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21) */
    static constexpr SDL_Keycode F22 = 0x40000071u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22) */
    static constexpr SDL_Keycode F23 = 0x40000072u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23) */
    static constexpr SDL_Keycode F24 = 0x40000073u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24) */
    static constexpr SDL_Keycode EXECUTE = 0x40000074u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE) */
    static constexpr SDL_Keycode HELP = 0x40000075u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP) */
    static constexpr SDL_Keycode MENU = 0x40000076u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU) */
    static constexpr SDL_Keycode SELECT = 0x40000077u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT) */
    static constexpr SDL_Keycode STOP = 0x40000078u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP) */
    static constexpr SDL_Keycode AGAIN = 0x40000079u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN) */
    static constexpr SDL_Keycode UNDO = 0x4000007au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO) */
    static constexpr SDL_Keycode CUT = 0x4000007bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT) */
    static constexpr SDL_Keycode COPY = 0x4000007cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY) */
    static constexpr SDL_Keycode PASTE = 0x4000007du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE) */
    static constexpr SDL_Keycode FIND = 0x4000007eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND) */
    static constexpr SDL_Keycode MUTE = 0x4000007fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE) */
    static constexpr SDL_Keycode VOLUMEUP = 0x40000080u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP) */
    static constexpr SDL_Keycode VOLUMEDOWN = 0x40000081u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN) */
    static constexpr SDL_Keycode KP_COMMA = 0x40000085u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA) */
    static constexpr SDL_Keycode KP_EQUALSAS400 = 0x40000086u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400) */
    static constexpr SDL_Keycode ALTERASE = 0x40000099u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE) */
    static constexpr SDL_Keycode SYSREQ = 0x4000009au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ) */
    static constexpr SDL_Keycode CANCEL = 0x4000009bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL) */
    static constexpr SDL_Keycode CLEAR = 0x4000009cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR) */
    static constexpr SDL_Keycode PRIOR = 0x4000009du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR) */
    static constexpr SDL_Keycode RETURN2 = 0x4000009eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2) */
    static constexpr SDL_Keycode SEPARATOR = 0x4000009fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR) */
    static constexpr SDL_Keycode OUT = 0x400000a0u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT) */
    static constexpr SDL_Keycode OPER = 0x400000a1u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER) */
    static constexpr SDL_Keycode CLEARAGAIN = 0x400000a2u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN) */
    static constexpr SDL_Keycode CRSEL = 0x400000a3u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL) */
    static constexpr SDL_Keycode EXSEL = 0x400000a4u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL) */
    static constexpr SDL_Keycode KP_00 = 0x400000b0u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00) */
    static constexpr SDL_Keycode KP_000 = 0x400000b1u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000) */
    static constexpr SDL_Keycode THOUSANDSSEPARATOR = 0x400000b2u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR) */
    static constexpr SDL_Keycode DECIMALSEPARATOR = 0x400000b3u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR) */
    static constexpr SDL_Keycode CURRENCYUNIT = 0x400000b4u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT) */
    static constexpr SDL_Keycode CURRENCYSUBUNIT = 0x400000b5u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT) */
    static constexpr SDL_Keycode KP_LEFTPAREN = 0x400000b6u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN) */
    static constexpr SDL_Keycode KP_RIGHTPAREN = 0x400000b7u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN) */
    static constexpr SDL_Keycode KP_LEFTBRACE = 0x400000b8u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE) */
    static constexpr SDL_Keycode KP_RIGHTBRACE = 0x400000b9u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE) */
    static constexpr SDL_Keycode KP_TAB = 0x400000bau; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB) */
    static constexpr SDL_Keycode KP_BACKSPACE = 0x400000bbu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE) */
    static constexpr SDL_Keycode KP_A = 0x400000bcu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A) */
    static constexpr SDL_Keycode KP_B = 0x400000bdu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B) */
    static constexpr SDL_Keycode KP_C = 0x400000beu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C) */
    static constexpr SDL_Keycode KP_D = 0x400000bfu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D) */
    static constexpr SDL_Keycode KP_E = 0x400000c0u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E) */
    static constexpr SDL_Keycode KP_F = 0x400000c1u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F) */
    static constexpr SDL_Keycode KP_XOR = 0x400000c2u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR) */
    static constexpr SDL_Keycode KP_POWER = 0x400000c3u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER) */
    static constexpr SDL_Keycode KP_PERCENT = 0x400000c4u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT) */
    static constexpr SDL_Keycode KP_LESS = 0x400000c5u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS) */
    static constexpr SDL_Keycode KP_GREATER = 0x400000c6u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER) */
    static constexpr SDL_Keycode KP_AMPERSAND = 0x400000c7u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND) */
    static constexpr SDL_Keycode KP_DBLAMPERSAND = 0x400000c8u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND) */
    static constexpr SDL_Keycode KP_VERTICALBAR = 0x400000c9u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR) */
    static constexpr SDL_Keycode KP_DBLVERTICALBAR = 0x400000cau; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR) */
    static constexpr SDL_Keycode KP_COLON = 0x400000cbu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON) */
    static constexpr SDL_Keycode KP_HASH = 0x400000ccu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH) */
    static constexpr SDL_Keycode KP_SPACE = 0x400000cdu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE) */
    static constexpr SDL_Keycode KP_AT = 0x400000ceu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT) */
    static constexpr SDL_Keycode KP_EXCLAM = 0x400000cfu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM) */
    static constexpr SDL_Keycode KP_MEMSTORE = 0x400000d0u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE) */
    static constexpr SDL_Keycode KP_MEMRECALL = 0x400000d1u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL) */
    static constexpr SDL_Keycode KP_MEMCLEAR = 0x400000d2u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR) */
    static constexpr SDL_Keycode KP_MEMADD = 0x400000d3u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD) */
    static constexpr SDL_Keycode KP_MEMSUBTRACT = 0x400000d4u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT) */
    static constexpr SDL_Keycode KP_MEMMULTIPLY = 0x400000d5u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY) */
    static constexpr SDL_Keycode KP_MEMDIVIDE = 0x400000d6u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE) */
    static constexpr SDL_Keycode KP_PLUSMINUS = 0x400000d7u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS) */
    static constexpr SDL_Keycode KP_CLEAR = 0x400000d8u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR) */
    static constexpr SDL_Keycode KP_CLEARENTRY = 0x400000d9u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY) */
    static constexpr SDL_Keycode KP_BINARY = 0x400000dau; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY) */
    static constexpr SDL_Keycode KP_OCTAL = 0x400000dbu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL) */
    static constexpr SDL_Keycode KP_DECIMAL = 0x400000dcu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL) */
    static constexpr SDL_Keycode KP_HEXADECIMAL = 0x400000ddu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL) */
    static constexpr SDL_Keycode LCTRL = 0x400000e0u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL) */
    static constexpr SDL_Keycode LSHIFT = 0x400000e1u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT) */
    static constexpr SDL_Keycode LALT = 0x400000e2u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT) */
    static constexpr SDL_Keycode LGUI = 0x400000e3u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI) */
    static constexpr SDL_Keycode RCTRL = 0x400000e4u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL) */
    static constexpr SDL_Keycode RSHIFT = 0x400000e5u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT) */
    static constexpr SDL_Keycode RALT = 0x400000e6u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT) */
    static constexpr SDL_Keycode RGUI = 0x400000e7u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI) */
    static constexpr SDL_Keycode MODE = 0x40000101u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE) */
    static constexpr SDL_Keycode SLEEP = 0x40000102u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP) */
    static constexpr SDL_Keycode WAKE = 0x40000103u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WAKE) */
    static constexpr SDL_Keycode CHANNEL_INCREMENT = 0x40000104u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_INCREMENT) */
    static constexpr SDL_Keycode CHANNEL_DECREMENT = 0x40000105u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_DECREMENT) */
    static constexpr SDL_Keycode MEDIA_PLAY = 0x40000106u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY) */
    static constexpr SDL_Keycode MEDIA_PAUSE = 0x40000107u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PAUSE) */
    static constexpr SDL_Keycode MEDIA_RECORD = 0x40000108u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_RECORD) */
    static constexpr SDL_Keycode MEDIA_FAST_FORWARD = 0x40000109u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_FAST_FORWARD) */
    static constexpr SDL_Keycode MEDIA_REWIND = 0x4000010au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_REWIND) */
    static constexpr SDL_Keycode MEDIA_NEXT_TRACK = 0x4000010bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_NEXT_TRACK) */
    static constexpr SDL_Keycode MEDIA_PREVIOUS_TRACK = 0x4000010cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PREVIOUS_TRACK) */
    static constexpr SDL_Keycode MEDIA_STOP = 0x4000010du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_STOP) */
    static constexpr SDL_Keycode MEDIA_EJECT = 0x4000010eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_EJECT) */
    static constexpr SDL_Keycode MEDIA_PLAY_PAUSE = 0x4000010fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY_PAUSE) */
    static constexpr SDL_Keycode MEDIA_SELECT = 0x40000110u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_SELECT) */
    static constexpr SDL_Keycode AC_NEW = 0x40000111u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_NEW) */
    static constexpr SDL_Keycode AC_OPEN = 0x40000112u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_OPEN) */
    static constexpr SDL_Keycode AC_CLOSE = 0x40000113u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_CLOSE) */
    static constexpr SDL_Keycode AC_EXIT = 0x40000114u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_EXIT) */
    static constexpr SDL_Keycode AC_SAVE = 0x40000115u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SAVE) */
    static constexpr SDL_Keycode AC_PRINT = 0x40000116u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PRINT) */
    static constexpr SDL_Keycode AC_PROPERTIES = 0x40000117u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PROPERTIES) */
    static constexpr SDL_Keycode AC_SEARCH = 0x40000118u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH) */
    static constexpr SDL_Keycode AC_HOME = 0x40000119u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME) */
    static constexpr SDL_Keycode AC_BACK = 0x4000011au; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK) */
    static constexpr SDL_Keycode AC_FORWARD = 0x4000011bu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD) */
    static constexpr SDL_Keycode AC_STOP = 0x4000011cu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP) */
    static constexpr SDL_Keycode AC_REFRESH = 0x4000011du; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH) */
    static constexpr SDL_Keycode AC_BOOKMARKS = 0x4000011eu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS) */
    static constexpr SDL_Keycode SOFTLEFT = 0x4000011fu; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT) */
    static constexpr SDL_Keycode SOFTRIGHT = 0x40000120u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT) */
    static constexpr SDL_Keycode CALL = 0x40000121u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL) */
    static constexpr SDL_Keycode ENDCALL = 0x40000122u; /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL) */
    static constexpr SDL_Keycode LEFT_TAB = 0x20000001u; /**< Extended key Left Tab */
    static constexpr SDL_Keycode LEVEL5_SHIFT = 0x20000002u; /**< Extended key Level 5 Shift */
    static constexpr SDL_Keycode MULTI_KEY_COMPOSE = 0x20000003u; /**< Extended key Multi-key Compose */
    static constexpr SDL_Keycode LMETA = 0x20000004u; /**< Extended key Left Meta */
    static constexpr SDL_Keycode RMETA = 0x20000005u; /**< Extended key Right Meta */
    static constexpr SDL_Keycode LHYPER = 0x20000006u; /**< Extended key Left Hyper */
    static constexpr SDL_Keycode RHYPER = 0x20000007u; /**< Extended key Right Hyper */
};

export class KeyPosition
{
public:
    // Letters
    static constexpr int UNKNOWN = 0;
    static constexpr int A = 4;
    static constexpr int B = 5;
    static constexpr int C = 6;
    static constexpr int D = 7;
    static constexpr int E = 8;
    static constexpr int F = 9;
    static constexpr int G = 10;
    static constexpr int H = 11;
    static constexpr int I = 12;
    static constexpr int J = 13;
    static constexpr int K = 14;
    static constexpr int L = 15;
    static constexpr int M = 16;
    static constexpr int N = 17;
    static constexpr int O = 18;
    static constexpr int P = 19;
    static constexpr int Q = 20;
    static constexpr int R = 21;
    static constexpr int S = 22;
    static constexpr int T = 23;
    static constexpr int U = 24;
    static constexpr int V = 25;
    static constexpr int W = 26;
    static constexpr int X = 27;
    static constexpr int Y = 28;
    static constexpr int Z = 29;
    
    // Numbers
    static constexpr int N1 = 30;
    static constexpr int N2 = 31;
    static constexpr int N3 = 32;
    static constexpr int N4 = 33;
    static constexpr int N5 = 34;
    static constexpr int N6 = 35;
    static constexpr int N7 = 36;
    static constexpr int N8 = 37;
    static constexpr int N9 = 38;
    static constexpr int N0 = 39;
    
    // Control keys
    static constexpr int RETURN = 40;
    static constexpr int ESCAPE = 41;
    static constexpr int BACKSPACE = 42;
    static constexpr int TAB = 43;
    static constexpr int SPACE = 44;
    
    // Punctuation
    static constexpr int MINUS = 45;
    static constexpr int EQUALS = 46;
    static constexpr int LEFTBRACKET = 47;
    static constexpr int RIGHTBRACKET = 48;
    static constexpr int BACKSLASH = 49;
    static constexpr int NONUSHASH = 50;
    static constexpr int SEMICOLON = 51;
    static constexpr int APOSTROPHE = 52;
    static constexpr int GRAVE = 53;
    static constexpr int COMMA = 54;
    static constexpr int PERIOD = 55;
    static constexpr int SLASH = 56;
    
    // Special keys
    static constexpr int CAPSLOCK = 57;
    
    // Function keys
    static constexpr int F1 = 58;
    static constexpr int F2 = 59;
    static constexpr int F3 = 60;
    static constexpr int F4 = 61;
    static constexpr int F5 = 62;
    static constexpr int F6 = 63;
    static constexpr int F7 = 64;
    static constexpr int F8 = 65;
    static constexpr int F9 = 66;
    static constexpr int F10 = 67;
    static constexpr int F11 = 68;
    static constexpr int F12 = 69;
    
    // Navigation and editing
    static constexpr int PRINTSCREEN = 70;
    static constexpr int SCROLLLOCK = 71;
    static constexpr int PAUSE = 72;
    static constexpr int INSERT = 73;
    static constexpr int HOME = 74;
    static constexpr int PAGEUP = 75;
    static constexpr int DELETE = 76;
    static constexpr int END = 77;
    static constexpr int PAGEDOWN = 78;
    static constexpr int RIGHT = 79;
    static constexpr int LEFT = 80;
    static constexpr int DOWN = 81;
    static constexpr int UP = 82;
    
    // Keypad
    static constexpr int NUMLOCKCLEAR = 83;
    static constexpr int KP_DIVIDE = 84;
    static constexpr int KP_MULTIPLY = 85;
    static constexpr int KP_MINUS = 86;
    static constexpr int KP_PLUS = 87;
    static constexpr int KP_ENTER = 88;
    static constexpr int KP_1 = 89;
    static constexpr int KP_2 = 90;
    static constexpr int KP_3 = 91;
    static constexpr int KP_4 = 92;
    static constexpr int KP_5 = 93;
    static constexpr int KP_6 = 94;
    static constexpr int KP_7 = 95;
    static constexpr int KP_8 = 96;
    static constexpr int KP_9 = 97;
    static constexpr int KP_0 = 98;
    static constexpr int KP_PERIOD = 99;
    
    // Additional keys
    static constexpr int NONUSBACKSLASH = 100;
    static constexpr int APPLICATION = 101;
    static constexpr int POWER = 102;
    static constexpr int KP_EQUALS = 103;
    
    // Extended function keys
    static constexpr int F13 = 104;
    static constexpr int F14 = 105;
    static constexpr int F15 = 106;
    static constexpr int F16 = 107;
    static constexpr int F17 = 108;
    static constexpr int F18 = 109;
    static constexpr int F19 = 110;
    static constexpr int F20 = 111;
    static constexpr int F21 = 112;
    static constexpr int F22 = 113;
    static constexpr int F23 = 114;
    static constexpr int F24 = 115;
    
    // System keys
    static constexpr int EXECUTE = 116;
    static constexpr int HELP = 117;
    static constexpr int MENU = 118;
    static constexpr int SELECT = 119;
    static constexpr int STOP = 120;
    static constexpr int AGAIN = 121;
    static constexpr int UNDO = 122;
    static constexpr int CUT = 123;
    static constexpr int COPY = 124;
    static constexpr int PASTE = 125;
    static constexpr int FIND = 126;
    
    // Audio
    static constexpr int MUTE = 127;
    static constexpr int VOLUMEUP = 128;
    static constexpr int VOLUMEDOWN = 129;
    
    // Keypad extended
    static constexpr int KP_COMMA = 133;
    static constexpr int KP_EQUALSAS400 = 134;
    
    // International keys
    static constexpr int INTERNATIONAL1 = 135;
    static constexpr int INTERNATIONAL2 = 136;
    static constexpr int INTERNATIONAL3 = 137;
    static constexpr int INTERNATIONAL4 = 138;
    static constexpr int INTERNATIONAL5 = 139;
    static constexpr int INTERNATIONAL6 = 140;
    static constexpr int INTERNATIONAL7 = 141;
    static constexpr int INTERNATIONAL8 = 142;
    static constexpr int INTERNATIONAL9 = 143;
    
    // Language keys
    static constexpr int LANG1 = 144;
    static constexpr int LANG2 = 145;
    static constexpr int LANG3 = 146;
    static constexpr int LANG4 = 147;
    static constexpr int LANG5 = 148;
    static constexpr int LANG6 = 149;
    static constexpr int LANG7 = 150;
    static constexpr int LANG8 = 151;
    static constexpr int LANG9 = 152;
    
    // Additional system keys
    static constexpr int ALTERASE = 153;
    static constexpr int SYSREQ = 154;
    static constexpr int CANCEL = 155;
    static constexpr int CLEAR = 156;
    static constexpr int PRIOR = 157;
    static constexpr int RETURN2 = 158;
    static constexpr int SEPARATOR = 159;
    static constexpr int OUT = 160;
    static constexpr int OPER = 161;
    static constexpr int CLEARAGAIN = 162;
    static constexpr int CRSEL = 163;
    static constexpr int EXSEL = 164;
    
    // Keypad special
    static constexpr int KP_00 = 176;
    static constexpr int KP_000 = 177;
    static constexpr int THOUSANDSSEPARATOR = 178;
    static constexpr int DECIMALSEPARATOR = 179;
    static constexpr int CURRENCYUNIT = 180;
    static constexpr int CURRENCYSUBUNIT = 181;
    static constexpr int KP_LEFTPAREN = 182;
    static constexpr int KP_RIGHTPAREN = 183;
    static constexpr int KP_LEFTBRACE = 184;
    static constexpr int KP_RIGHTBRACE = 185;
    static constexpr int KP_TAB = 186;
    static constexpr int KP_BACKSPACE = 187;
    static constexpr int KP_A = 188;
    static constexpr int KP_B = 189;
    static constexpr int KP_C = 190;
    static constexpr int KP_D = 191;
    static constexpr int KP_E = 192;
    static constexpr int KP_F = 193;
    static constexpr int KP_XOR = 194;
    static constexpr int KP_POWER = 195;
    static constexpr int KP_PERCENT = 196;
    static constexpr int KP_LESS = 197;
    static constexpr int KP_GREATER = 198;
    static constexpr int KP_AMPERSAND = 199;
    static constexpr int KP_DBLAMPERSAND = 200;
    static constexpr int KP_VERTICALBAR = 201;
    static constexpr int KP_DBLVERTICALBAR = 202;
    static constexpr int KP_COLON = 203;
    static constexpr int KP_HASH = 204;
    static constexpr int KP_SPACE = 205;
    static constexpr int KP_AT = 206;
    static constexpr int KP_EXCLAM = 207;
    static constexpr int KP_MEMSTORE = 208;
    static constexpr int KP_MEMRECALL = 209;
    static constexpr int KP_MEMCLEAR = 210;
    static constexpr int KP_MEMADD = 211;
    static constexpr int KP_MEMSUBTRACT = 212;
    static constexpr int KP_MEMMULTIPLY = 213;
    static constexpr int KP_MEMDIVIDE = 214;
    static constexpr int KP_PLUSMINUS = 215;
    static constexpr int KP_CLEAR = 216;
    static constexpr int KP_CLEARENTRY = 217;
    static constexpr int KP_BINARY = 218;
    static constexpr int KP_OCTAL = 219;
    static constexpr int KP_DECIMAL = 220;
    static constexpr int KP_HEXADECIMAL = 221;
    
    // Modifier keys
    static constexpr int LCTRL = 224;
    static constexpr int LSHIFT = 225;
    static constexpr int LALT = 226;
    static constexpr int LGUI = 227;
    static constexpr int RCTRL = 228;
    static constexpr int RSHIFT = 229;
    static constexpr int RALT = 230;
    static constexpr int RGUI = 231;
    
    // Mode
    static constexpr int MODE = 257;
    
    // Media keys
    static constexpr int SLEEP = 258;
    static constexpr int WAKE = 259;
    static constexpr int CHANNEL_INCREMENT = 260;
    static constexpr int CHANNEL_DECREMENT = 261;
    static constexpr int MEDIA_PLAY = 262;
    static constexpr int MEDIA_PAUSE = 263;
    static constexpr int MEDIA_RECORD = 264;
    static constexpr int MEDIA_FAST_FORWARD = 265;
    static constexpr int MEDIA_REWIND = 266;
    static constexpr int MEDIA_NEXT_TRACK = 267;
    static constexpr int MEDIA_PREVIOUS_TRACK = 268;
    static constexpr int MEDIA_STOP = 269;
    static constexpr int MEDIA_EJECT = 270;
    static constexpr int MEDIA_PLAY_PAUSE = 271;
    static constexpr int MEDIA_SELECT = 272;
    
    // Application control
    static constexpr int AC_NEW = 273;
    static constexpr int AC_OPEN = 274;
    static constexpr int AC_CLOSE = 275;
    static constexpr int AC_EXIT = 276;
    static constexpr int AC_SAVE = 277;
    static constexpr int AC_PRINT = 278;
    static constexpr int AC_PROPERTIES = 279;
    static constexpr int AC_SEARCH = 280;
    static constexpr int AC_HOME = 281;
    static constexpr int AC_BACK = 282;
    static constexpr int AC_FORWARD = 283;
    static constexpr int AC_STOP = 284;
    static constexpr int AC_REFRESH = 285;
    static constexpr int AC_BOOKMARKS = 286;
    
    // Mobile keys
    static constexpr int SOFTLEFT = 287;
    static constexpr int SOFTRIGHT = 288;
    static constexpr int CALL = 289;
    static constexpr int ENDCALL = 290;
    
    // Special
    static constexpr int RESERVED = 400;
    static constexpr int COUNT = 512;
};