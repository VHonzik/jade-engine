#include "LineBox.h"

#include "Game.h"
#include "LineStrip.h"
#include "Transform.h"

namespace JadeEngine
{
  LineBox::LineBox(const LineBoxParams& params)
  {
    LineStripParams stripParams;
    stripParams.layer = params.layer;
    stripParams.initialPoints = {};
    stripParams.color = params.color;
    stripParams.z = params.z;

    _strip = GGame.Create<LineStrip>(stripParams);
    transform->Initialize(kZeroVector2D_i32, params.size);
    UpdatePoints();
    transform->Attach(_strip->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    Show(true);
  }

  void LineBox::Update()
  {
    if (transform->IsDirty(kDirtyFlag_Size))
    {
      UpdatePoints();
    }
  }

  void LineBox::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _strip->Show(isShown && transform->GetSize().w > 2 && transform->GetSize().h > 2);
  }

  void LineBox::UpdatePoints()
  {
    if (transform->GetSize().w > 2 && transform->GetSize().h > 2)
    {
      const auto sizeHalf = transform->GetSize() / 2;
      _strip->SetPoints({ sizeHalf * Vector2D_i32{-1, -1}, sizeHalf * Vector2D_i32{1, -1}, sizeHalf * Vector2D_i32{1, 1}, sizeHalf * Vector2D_i32{-1, 1}, sizeHalf * Vector2D_i32{-1, -1} });
    }
    else
    {
      _strip->SetPoints({});
    }
  }
}