#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <sstream>
#include <pugixml/pugixml.hpp>
#include "UI/Elements.hpp"

using UIElementCreator = std::function<std::shared_ptr<UIControl>(pugi::xml_node)>;

class UIFactory
{
private:
    std::unordered_map<std::string, UIElementCreator> creators;

    static Color ParseColor(const char *str, const Color &defaultColor = Color::White())
    {
        if (!str || str[0] == '\0')
            return defaultColor;

        std::string s(str);
        if (s[0] == '#')
            s = s.substr(1);

        unsigned int hex = 0;
        std::stringstream ss;
        ss << std::hex << s;
        ss >> hex;

        if (s.length() == 8)
        {
            float r = ((hex >> 24) & 0xFF) / 255.0f;
            float g = ((hex >> 16) & 0xFF) / 255.0f;
            float b = ((hex >> 8) & 0xFF) / 255.0f;
            float a = (hex & 0xFF) / 255.0f;
            return Color(r, g, b, a);
        }
        else if (s.length() == 6)
        {
            float r = ((hex >> 16) & 0xFF) / 255.0f;
            float g = ((hex >> 8) & 0xFF) / 255.0f;
            float b = (hex & 0xFF) / 255.0f;
            return Color(r, g, b, 1.0f);
        }
        return defaultColor;
    }

    static void ParseBaseAttributes(pugi::xml_node node, std::shared_ptr<UIControl> control)
    {
        if (!control)
            return;

        if (auto attr = node.attribute("Name"))
            control->Name = attr.as_string();

        if (auto attr = node.attribute("Width"))
            control->Width = attr.as_float(control->Width);

        if (auto attr = node.attribute("Height"))
            control->Height = attr.as_float(control->Height);

        if (auto attr = node.attribute("Background"))
            control->Background = ParseColor(attr.as_string(), control->Background);

        if (auto attr = node.attribute("Margin"))
            control->Margin = attr.as_int();

        if (auto attr = node.attribute("Padding"))
            control->Padding = attr.as_int();
    }

    UIFactory()
    {
        Register("Grid", [](pugi::xml_node node)
                 {
            auto grid = std::make_shared<Grid>();
            ParseBaseAttributes(node, grid);
            return grid; });

        Register("StackPanel", [](pugi::xml_node node)
                 {
            auto sp = std::make_shared<StackPanel>();
            ParseBaseAttributes(node, sp);
            sp->Spacing = node.attribute("Spacing").as_float(0.0f);
            std::string orient = node.attribute("Orientation").as_string("Vertical");
            if (orient == "Horizontal")
                sp->orientation = Orientation::Horizontal;
            return sp; });

        Register("Decorator", [](pugi::xml_node node)
                 {
            auto dec = std::make_shared<Decorator>();
            ParseBaseAttributes(node, dec);
            dec->CornerRadius = node.attribute("CornerRadius").as_float(0.0f);
            return dec; });

        Register("TextBlock", [](pugi::xml_node node)
                 {
            auto tb = std::make_shared<TextBlock>();
            ParseBaseAttributes(node, tb);
            if (auto attr = node.attribute("Text"))
                tb->Text = attr.as_string();
            if (auto attr = node.attribute("Foreground"))
                tb->Foreground = ParseColor(attr.as_string(), tb->Foreground);
            return tb; });

        Register("ProgressBar", [](pugi::xml_node node)
                 {
            auto bar = std::make_shared<ProgressBar>();
            ParseBaseAttributes(node, bar);
            bar->Value = node.attribute("Value").as_float(0.0f);
            return bar; });
    }

public:
    static UIFactory &Get()
    {
        static UIFactory instance;
        return instance;
    }

    void Register(const std::string &tagName, UIElementCreator creator)
    {
        creators[tagName] = creator;
    }

    std::shared_ptr<UIControl> Create(pugi::xml_node node)
    {
        auto it = creators.find(node.name());
        if (it != creators.end())
        {
            return it->second(node);
        }
        return nullptr;
    }

    std::shared_ptr<UIControl> ParseNode(pugi::xml_node node)
    {
        auto control = Create(node);
        if (!control)
            return nullptr;

        for (pugi::xml_node child : node.children())
        {
            auto childControl = ParseNode(child);
            if (!childControl)
                continue;

            if (auto stack = std::dynamic_pointer_cast<StackPanel>(control))
            {
                stack->Add(childControl);
            }
            else if (auto dec = std::dynamic_pointer_cast<Decorator>(control))
            {
                dec->child = childControl;
            }
            else if (auto grid = std::dynamic_pointer_cast<Grid>(control))
            {
                grid->Add(childControl, 0, 0);
            }
        }

        return control;
    }

    std::shared_ptr<UIControl> LoadFromFile(const std::string &filepath)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filepath.c_str());
        if (!result)
        {
            Debug::LogError("Failed to load UI XML file: ", filepath, ", error: ", result.description());
            return nullptr;
        }

        pugi::xml_node rootNode = doc.first_child();
        if (!rootNode)
            return nullptr;

        return ParseNode(rootNode);
    }
};
