template<size_t SIZE>
class Bitset {
public:
    typedef uint64_t TData;
    typedef size_t TSize;

    class TReference {
        friend class Bitset;
        public:
            TReference(TData* dataPtr, TSize bitId):
                dataPtr(dataPtr), bitId(bitId) {}

            operator bool() const {
                return (*dataPtr >> bitId) & 1;
            }
            TReference& operator =(const TReference& other) {
                this->operator=(static_cast<bool>(other));
                return *this;
            }
            TReference& operator =(bool newBit) {
                if (!newBit) {
                    *dataPtr = *dataPtr &
                        (Bitset::FULL ^ (Bitset::ONE << bitId));
                } else {
                    *dataPtr = *dataPtr | (Bitset::ONE << bitId);
                }
                return *this;
            }
            bool operator ~() const {
                return ((*dataPtr >> bitId) & 1) ^ 1;
            }
            TReference& flip() {
                *this = !static_cast<bool>(*this);
                return *this;
            }
        private:
            TSize bitId;
            TData* dataPtr;
    };

    Bitset() {
        n = div64(SIZE+63);
        fill(data, data+n, ZERO);
    }

    Bitset(const string& s) {
        n = div64(SIZE+63);
        fill(data, data+n, ZERO);
        TSize N = (s.length() > SIZE ? SIZE : s.length());
        for(TSize i = 0; i < N; ++i) {
            if (s[i] == '1') {
                this->set(i);
            }
        }
    }

    Bitset(const Bitset<SIZE>& other) {
        this->n = other.n;
        for(TSize i = 0; i < n; ++i) {
            this->data[i] = other.data[i];
        }
    }

    inline void set() {
        fill(data, data+n, FULL);
    }
    inline void set(TSize x) {
        data[div64(x)] |= (ONE << mod64(x));
    }
    inline void reset() {
        fill(data, data+n, ZERO);
    }
    inline void reset(TSize x) {
        data[div64(x)] &= ~(ONE << mod64(x));
    }

    inline void flip() {
        *this = this->operator~();
    }
    inline void flip(TSize x) {
        data[div64(x)] ^= (ONE << mod64(x));
    }
    bool get(TSize x) const {
        return (data[div64(x)] >> mod64(x)) & 1;
    }

    bool any() const {
        for(TSize i = 0; i < n; ++i) {
            if (data[i] != 0) return true;
        }
        return false;
    }
    bool all() const {
        for(TSize i = 0; i < n; ++i) {
            if (data[i] != FULL) return false;
        }
        return true;
    }
    bool none() const { return !any(); }

    TSize size() const { return SIZE; }
    TSize count() const {
        TSize ret = 0;
        for(TSize i = 0; i < n; ++i) {
            ret += __builtin_popcountll(data[i]);
        }
        return ret;
    }
    TSize findNext(TSize pos) {
        TSize p = div64(pos);
        TData mask = lastNBits(data[p], BITS-mod64(pos)-1);
        if (mask != 0) {
            return mul64(p) + __builtin_ctzll(mask);
        }
        for(p = p + 1; p < n; ++p) {
            if (data[p] != 0) {
                return mul64(p) + __builtin_ctzll(data[p]);
            }
        }
        return SIZE;
    }
    TSize findFirst() {
        return (this->operator[](0) == 1 ? 0 : findNext(0));
    }

    TReference operator [](TSize x) {
        return TReference(&data[div64(x)], mod64(x));
    }
    bool operator [](TSize x) const {
        return (data[div64(x)] >> mod64(x)) & ONE;
    }

    bool operator <(const Bitset<SIZE>& other) {
        for(TSize i = 0; i < n; ++i) {
            if (other.data[i] != data[i]) {
                return data[i] < other.data[i];
            }
        }
        return false;
    }
    bool operator ==(const Bitset<SIZE>& other) {
        for(TSize i = 0; i < n; ++i) {
            if (other.data[i] != data[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator !=(const Bitset<SIZE>& other) {
        return !this->operator==(other);
    }

    Bitset<SIZE> operator ~() const {
        Bitset<SIZE> ret;
        for(TSize i = 0; i < n; ++i) {
            ret.data[i] = ~data[i];
            if (i == n-1) {
                ret.data[i] ^= lastNBits(ret.data[i], BITS - mod64(SIZE));
            }
        }
        return ret;
    }
    Bitset<SIZE>& operator <<=(TSize shift) {
        if (shift > SIZE) {
            this->reset();
        }
        TSize fullShift = div64(shift);
        TSize insideShift = mod64(shift);
        TSize outsideShift = BITS - insideShift;
        TData carry = 0;
        Bitset<SIZE> temp;
        for(TSize i = 0; i < n - fullShift; ++i) {
            temp.data[i + fullShift] = (data[i] << insideShift) | carry;
            carry = (data[i] & (FULL ^ ((ONE << outsideShift) - 1)))
                    >> outsideShift;
        }
        for (TSize i = 0; i < n; ++i) {
            data[i] = temp.data[i];
        }
        return *this;
    }
    Bitset<SIZE>& operator >>=(TSize shift) {
        if (shift > SIZE) {
            this->reset();
        }
        TSize fullShift = div64(shift);
        TSize insideShift = mod64(shift);
        TSize outsideShift = BITS - insideShift;
        TData carry = 0;
        Bitset<SIZE> temp;
        for(TSize i = n-1; i >= fullShift; ++i) {
            temp.data[i - fullShift] = (data[i] >> insideShift) | carry;
            carry = firstNBits(data[i], insideShift);
            if (i == fullShift) break;
        }
        for (TSize i = 0; i < n; ++i) {
            data[i] = temp.data[i];
        }
        return *this;
    }
    Bitset<SIZE>& operator &=(const Bitset<SIZE>& other) {
        for(TSize i = 0; i < n; ++i) {
            this->data[i] &= other.data[i];
        }
        return *this;
    }
    Bitset<SIZE>& operator |=(const Bitset<SIZE>& other) {
        for(TSize i = 0; i < n; ++i) {
            this->data[i] |= other.data[i];
        }
        return *this;
    }
    Bitset<SIZE>& operator ^=(const Bitset<SIZE>& other) {
        for(TSize i = 0; i < n; ++i) {
            this->data[i] ^= other.data[i];
        }
        return *this;
    }

    Bitset<SIZE> operator <<(TSize shift) {
        return Bitset<SIZE>(*this) <<= shift;
    }
    Bitset<SIZE> operator >>(TSize shift) {
        return Bitset<SIZE>(*this) >>= shift;
    }
    Bitset<SIZE> operator &(const Bitset<SIZE>& other) {
        return Bitset<SIZE>(*this) &= other;
    }
    Bitset<SIZE> operator |(const Bitset<SIZE>& other) {
        return Bitset<SIZE>(*this) |= other;
    }
    Bitset<SIZE> operator ^(const Bitset<SIZE>& other) {
        return Bitset<SIZE>(*this) ^= other;
    }

    string to_string() const {
        string ret;
        for(TSize i = 0; i < SIZE; ++i) {
            ret += '0' + this->operator[](i);
        }
        return ret;
    }
    friend istream& operator <<(istream& is, Bitset<SIZE>& x) {
        string s;
        is >> s;
        x = Bitset<SIZE>(s);
        return is;
    }
    friend ostream& operator <<(ostream& os, const Bitset<SIZE>& x) {
        string s = x.to_string();
        os << s;
        return os;
    }

private:
    static constexpr TSize BITS = 64;

    static constexpr TData ZERO = static_cast<TData>(0);
    static constexpr TData ONE = static_cast<TData>(1);
    static constexpr TData FULL = ~ZERO;

    TSize n;
    TData data[(SIZE >> 6) + 2];

    static TData div64(TData x) {return (x >> 6);}
    static TData mod64(TData x) {return (x & 63);}
    static TData mul64(TData x) {return (x << 6);}
    static TData firstNBits(TData x, TSize y) {
        return (x & (~(FULL << y)));
    }
    static TData lastNBits(TData x, TSize y) {
        return x & (FULL << (BITS - y));
    }
};
