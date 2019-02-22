template <typename T>
class MStack {
public:
    MStack() = default;
    void push(const T& x) {
        if (st.empty()) {
            st.emplace_back(x, x);
        } else {
            st.emplace_back(x, std::max(x, st.back().snd));
        }
    }
    void pop() { st.pop_back(); }
    T top() const { return st.back().fst; }
    T max() const { return st.back().snd; }
    size_t size() const { return st.size(); }
    bool empty() const { return st.empty(); }
private:
    vector<pair<T, T>> st;
};

template <typename T>
class MQueue {
public:
    MQueue() = default;
    void push(const T& x) { add.push(x); }
    void pop() {
        if (rem.empty()) {
            while (!add.empty()) {
                rem.push(add.top());
                add.pop();
            }
        }
        rem.pop();
    }
    T max() const {
        if (!add.empty() && !rem.empty())
            return std::max(add.max(), rem.max());
        if (!add.empty())
            return add.max();
        return rem.max();
    }
    size_t size() const { return add.size() + rem.size(); }
    bool empty() const { return add.empty() && rem.empty(); }

private:
    MStack<T> add;
    MStack<T> rem;
};
