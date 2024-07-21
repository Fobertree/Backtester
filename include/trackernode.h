#include <unordered_map>
#include <string>

template <typename T>
class TrackerNode
{
public:
protected:
private:
    std::unordered_map<std::string, T> children;
};