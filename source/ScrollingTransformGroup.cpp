#include "ScrollingTransformGroup.h"

#include "EngineConstants.h"
#include "Game.h"
#include "Input.h"
#include "TransformGroup.h"

#include <cassert>

namespace JadeEngine
{
  const auto kScrollBarWidth = 20;

  ScrollingTransformGroup::ScrollingTransformGroup(const ScrollingTransformGroupParams& params)
    : _group(params.scrolledGroup)
    , _offset{ 0, 0 }
    , _groupEmpty{ true }
    , _nothingToScroll{ true }
  {
    assert(_group != nullptr);

    Anchor anchor;
    switch (params.groupAligment)
    {
    case kHorizontalAlignment_Left:
      anchor = kAnchor_LeftTop;
      break;
    case kHorizontalAlignment_Center:
      anchor = kAnchor_CenterTop;
      break;
    default:
      assert(params.groupAligment == kHorizontalAlignment_Right);
      anchor = kAnchor_RightTop;
      _offset.x = -kScrollBarWidth - 10;
      break;
    }

    _scrollBarBackground = GGame.CreateSolidColorSprite(kScrollBarWidth, params.size.h, kLighterDarkGreyColor, params.z - 1, params.layer);
    transform->Attach(_scrollBarBackground->transform, kZeroVector2D_i32, kAnchor_RightTop, kAnchor_RightTop);
    _nothingToScroll = _group->transform->GetHeight() < params.size.h;
    auto foregroundHeight = params.size.h;
    if (!_nothingToScroll)
    {
      const auto t = static_cast<float>(params.size.h) / _group->transform->GetHeight();
      foregroundHeight = static_cast<int32_t>(t * params.size.h);
    }
    _scrollBarForeground = GGame.CreateSolidColorSprite(kScrollBarWidth, foregroundHeight, kLightGreyColor, params.z, params.layer);
    transform->Attach(_scrollBarForeground->transform, kZeroVector2D_i32, kAnchor_RightTop, kAnchor_RightTop);

    if (_group->Size() > 0)
    {
      _scrollSpeed = _group->GetElementSize(0).h / 2;
      _groupEmpty = false;
    }

    transform->Attach(_group->transform, _offset, anchor, anchor);

    transform->Initialize(kZeroVector2D_i32, params.size);
    Show(IsShown());
  }

  void ScrollingTransformGroup::UpdateForegroundScrollBarHeight()
  {
    _nothingToScroll = _group->transform->GetHeight() < transform->GetHeight();
    auto foregroundHeight = transform->GetHeight();
    if (!_nothingToScroll)
    {
      const auto t = static_cast<float>(transform->GetHeight()) / _group->transform->GetHeight();
      foregroundHeight = static_cast<int32_t>(t * transform->GetHeight());
    }

    if (_scrollBarForeground->transform->GetHeight() != foregroundHeight)
    {
      _scrollBarForeground->transform->SetSize({ 20, foregroundHeight });
    }

    Show(IsShown());
  }

  void ScrollingTransformGroup::UpdateForegroundScrollPosition()
  {
    if (_nothingToScroll) return;

    const auto t = static_cast<float>(std::abs(_offset.y)) / _group->transform->GetHeight();
    _scrollBarForeground->transform->SetLocalPosition({ 0, static_cast<int32_t>(t * transform->GetHeight()) });
  }

  void ScrollingTransformGroup::Show(const bool show)
  {
    IGameObject::Show(show);
    const bool isShown = IsShown();
    _scrollBarBackground->Show(isShown && !_nothingToScroll);
    _scrollBarForeground->Show(isShown && !_nothingToScroll);
  }

  void ScrollingTransformGroup::Update()
  {
    if (!_nothingToScroll && IsShown())
    {
      if (GInput.KeyPressed(SDLK_DOWN) || GInput.GetMouseWheelY() > 0)
      {
        if (std::abs(_offset.y) + transform->GetHeight() <= _group->transform->GetHeight())
        {
          _offset.y -= _scrollSpeed;
          _group->transform->SetLocalPosition(_offset);
          UpdateForegroundScrollPosition();
        }
      }
      else if (GInput.KeyPressed(SDLK_UP) || GInput.GetMouseWheelY() < 0)
      {
        if ((_offset.y + _scrollSpeed) <= 0)
        {
          _offset.y += _scrollSpeed;
          _group->transform->SetLocalPosition(_offset);
          UpdateForegroundScrollPosition();
        }
      }
    }

    if (_groupEmpty && _group->Size() > 0)
    {
      _groupEmpty = false;
      _scrollSpeed = _group->GetElementSize(0).h / 2;
    }

    if (transform->IsDirty(kDirtyFlag_Size))
    {
      _scrollBarBackground->transform->SetSize({ 20, transform->GetSize().h });
      UpdateForegroundScrollBarHeight();
      UpdateForegroundScrollPosition();
    }

    if (_group->transform->IsDirty(kDirtyFlag_Size))
    {
      UpdateForegroundScrollBarHeight();
      UpdateForegroundScrollPosition();
    }
  }
}