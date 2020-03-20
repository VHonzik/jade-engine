#pragma once

#include "Alignment.h"
#include "IGameObject.h"
#include "ObjectLayer.h"

namespace JadeEngine
{
  class Sprite;
  class TransformGroup;

  struct ScrollingTransformGroupParams
  {
    ObjectLayer         layer;
    TransformGroup*     scrolledGroup;
    HorizontalAlignment groupAligment;
    Vector2D_i32        size;
    int32_t             z;
  };

  class ScrollingTransformGroup : public IGameObject
  {
  public:
    ScrollingTransformGroup(const ScrollingTransformGroupParams& params);
    void Update() override;
    void Show(const bool show) override;
  private:
    void UpdateForegroundScrollBarHeight();
    void UpdateForegroundScrollPosition();

    TransformGroup* _group;
    Vector2D_i32 _offset;
    Sprite* _scrollBarBackground;
    Sprite* _scrollBarForeground;
    bool _groupEmpty;
    bool _nothingToScroll;
    int32_t _scrollSpeed;
  };
}