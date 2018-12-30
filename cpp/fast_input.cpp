#define cin _input_stream

#ifdef _WIN32
	#define nextchar getchar
#else
	#define nextchar getchar_unlocked
#endif

class _istream_t {} _input_stream;

template <typename T>
inline  _istream_t& operator >>(_istream_t& in, T& x) {
    char ch;
    bool negate = false;
    x = 0;
    do {ch = nextchar();} while (ch <= ' ');
    if (ch == '-') negate = true, ch = nextchar();
    while ('0' <= ch && ch <= '9') {
        x = (x<<3) + (x<<1) + ch - '0';
        ch = nextchar();
    }
    if (negate) x = -x;
    return in;
}

inline  _istream_t& operator >>(_istream_t& in, char& ch) {
    do {ch = nextchar();} while (ch <= ' ');
    return in;
}

inline  _istream_t& operator >>(_istream_t& in, std::string& s) {
    char ch; s = "";
    do {ch = nextchar();} while (ch <= ' ');
    while (ch > ' ') s += ch, ch = nextchar();
    return in;
}
