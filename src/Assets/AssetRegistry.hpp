#pragma once
#include <functional>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "Assets/File.hpp"
#include "Entity/Object.hpp"
#include "Core/GarbageCollector/Ref.hpp"

#define ASSET_CONCAT_IMPL(x, y) x##y
#define ASSET_CONCAT(x, y) ASSET_CONCAT_IMPL(x, y)

class AssetRegistry
{
public:
    using LoaderFn = std::function<Ref<Object>(const VoidCore::Assets::File &file)>;

    template <typename T>
    static void Register(const std::string &ext, std::function<Ref<T>(const VoidCore::Assets::File &)> loader)
    {
        static_assert(std::is_base_of_v<Object, T>, "Asset type must derive from Object");
        std::type_index typeIdx(typeid(T));

        loaders[typeIdx][ext] = [loader = std::move(loader)](const VoidCore::Assets::File &file) -> Ref<Object>
        {
            return loader(file).Get();
        };
    }

    template <typename T>
    static Ref<T> Load(const VoidCore::Assets::File &file)
    {
        std::type_index typeIdx(typeid(T));
        auto typeIt = loaders.find(typeIdx);
        if (typeIt == loaders.end())
            return nullptr;

        const std::string &path = file.GetPath();
        auto dot = path.find_last_of('.');
        if (dot == std::string::npos)
            return nullptr;

        std::string ext = path.substr(dot);
        auto extIt = typeIt->second.find(ext);
        if (extIt == typeIt->second.end())
            return nullptr;

        Ref<Object> obj = extIt->second(file);
        return static_cast<T *>(obj.Get());
    }

private:
    static inline std::unordered_map<std::type_index, std::unordered_map<std::string, LoaderFn>> loaders;
};

#define REGISTER_ASSET_LOADER(AssetType, Extension, LoaderFunc) \
    static inline bool ASSET_CONCAT(_reg_asset_, __LINE__) = []() { \
        AssetRegistry::Register<AssetType>(Extension, LoaderFunc); \
        return true; }()
