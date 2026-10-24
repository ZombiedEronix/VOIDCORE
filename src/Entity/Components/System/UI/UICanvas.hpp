#include "Entity/Components/Base/Behaviour.hpp"
#include "UIDocument.hpp"
#include "UI/UI.hpp"

class UICanvas : public Behaviour
{
public:
    int sortingOrder = 0;

    UIDocument* document = nullptr;

    void SetDocument(UIDocument* document)
    {
        this->document = document;
    }

    Rect GetTargetRect(float screenW, float screenH)
    {
        //Screen Space implemented only
        return Rect{0, 0, screenW, screenH};
    }
};
