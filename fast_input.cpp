#define cin _input_stream

#ifdef _WIN32
	#define nextChar getchar
#else
	#define nextChar getchar_unlocked
#endif

class _istream_t {} _input_stream;

template <typename T>
inline  _istream_t& operator >>(_istream_t& in, T& x) {
    char ch;
    bool negate = false;
    x = 0;
    do {ch = nextChar();} while (ch <= ' ');
    if (ch == '-') negate = true, ch = nextChar();
    while ('0' <= ch && ch <= '9') {
        x = (x<<3) + (x<<1) + ch - '0';
        ch = nextChar();
    }
    if (negate) x = -x;
    return in;
}

inline  _istream_t& operator >>(_istream_t& in, char& ch) {
    do {ch = nextChar();} while (ch <= ' ');
    return in;
}

inline  _istream_t& operator >>(_istream_t& in, std::string& s) {
    char ch; s = "";
    do {ch = nextChar();} while (ch <= ' ');
    while (ch > ' ') s += ch, ch = nextChar();
    return in;
}
