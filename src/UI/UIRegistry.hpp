#include <functional>
#include "UI/Elements/UIControl.hpp"

class UIRegistry
{
public:
    using Creator = std::function<std::shared_ptr<UIControl>()>;

    static void Register(const std::string &name, Creator fn) { creators[name] = fn; }
    static std::shared_ptr<UIControl> Create(const std::string &name)
    {
        return creators.count(name) ? creators[name]() : nullptr;
    }

private:
    static inline std::unordered_map<std::string, Creator> creators;
};

#define REGISTER_UI_CONTROL(Type) \
    static inline bool _reg_##Type = []() { \
        UIRegistry::Register(#Type, []() { return std::make_shared<Type>(); }); \
        return true; }()
