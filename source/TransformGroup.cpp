#include "TransformGroup.h"

#include <algorithm>
#include <cassert>
#include <numeric>

namespace JadeEngine
{
  TransformGroup::TransformGroup(const TransformGroupParams& params)
    : _alignment(params.alignment)
    , _direction(params.direction)
    , _defaultSpacing(params.spacing)
    , _parentAnchor(ParentAnchor(_direction, _alignment))
    , _childAnchor(ChildAnchor(_direction, _alignment))
  {
    transform->Initialize(kZeroVector2D_i32, kZeroVector2D_i32);
  }

  void TransformGroup::Add(IGameObject* element)
  {
    Add(element, _defaultSpacing);
  }

  void TransformGroup::Add(IGameObject* element, int32_t elementSpacing)
  {
    Add(element->transform, elementSpacing);
    Align();
  }

  void TransformGroup::Add(const std::vector<IGameObject*>& elements)
  {
    for (const auto element : elements)
    {
      Add(element->transform);
    }
    Align();
  }

  Anchor TransformGroup::ParentAnchor(const GroupDirection direction, const int32_t alignment)
  {
    switch (direction)
    {
    case kGroupDirection_Vertical:
    {
      switch (alignment)
      {
      case kHorizontalAlignment_Left:
        return kAnchor_LeftBottom;
      case kHorizontalAlignment_Center:
        return kAnchor_CenterBottom;
      default:
        assert(alignment == kHorizontalAlignment_Right);
        return kAnchor_RightBottom;
      }
    }
    default:
    {
      assert(direction == kGroupDirection_Horizontal);
      switch (alignment)
      {
      case kVerticalAlignment_Top:
        return kAnchor_RightTop;
      case kVerticalAlignment_Center:
        return kAnchor_RightCenter;
      default:
        assert(alignment == kVerticalAlignment_Bottom);
        return kAnchor_RightBottom;
      }
    }
    }
  }

  Anchor TransformGroup::ChildAnchor(const GroupDirection direction, const int32_t alignment)
  {
    switch (direction)
    {
    case kGroupDirection_Vertical:
    {
      switch (alignment)
      {
      case kHorizontalAlignment_Left:
        return kAnchor_LeftTop;
      case kHorizontalAlignment_Center:
        return kAnchor_CenterTop;
      default:
        assert(alignment == kHorizontalAlignment_Right);
        return kAnchor_RightTop;
      }
    }
    default:
    {
      assert(direction == kGroupDirection_Horizontal);
      switch (alignment)
      {
      case kVerticalAlignment_Top:
        return kAnchor_LeftTop;
      case kVerticalAlignment_Center:
        return kAnchor_LeftCenter;
      default:
        assert(alignment == kVerticalAlignment_Bottom);
        return kAnchor_LeftBottom;
      }
    }
    }
  }


  void TransformGroup::Add(const std::shared_ptr<Transform>& elementTransform)
  {
    Add(elementTransform, _elements.size() == 0 ? 0 : _defaultSpacing);
  }

  void TransformGroup::Add(const std::shared_ptr<Transform>& elementTransform, const int32_t spacing)
  {
    _elements.push_back(elementTransform);
    _spacing.push_back(spacing);
    if (_elements.size() == 1)
    {
      // The first element is not attached to any other but instead to the TransformGroup::transform so use _childAnchor for parentAnchor as well
      Vector2D_i32 offset = kZeroVector2D_i32;
      switch (_direction)
      {
      case kGroupDirection_Vertical:
        offset.y = spacing;
        break;
      default:
        assert(_direction == kGroupDirection_Horizontal);
        offset.x = spacing;
        break;
      }

      transform->Attach(elementTransform, offset, _childAnchor, _childAnchor);
    }
    else
    {
      Vector2D_i32 offset = kZeroVector2D_i32;
      switch (_direction)
      {
      case kGroupDirection_Vertical:
        offset.y = spacing;
        break;
      default:
        assert(_direction == kGroupDirection_Horizontal);
        offset.x = spacing;
        break;
      }
      _elements[_elements.size()-2]->Attach(elementTransform, offset, _parentAnchor, _childAnchor);
    }
  }

  void TransformGroup::Align()
  {
    assert(_elements.size() != 0);
    if (_elements.size() != 0)
    {
      switch (_direction)
      {
      case kGroupDirection_Vertical:
      {
        const auto width = std::accumulate(std::cbegin(_elements), std::cend(_elements), 0, [](const int32_t currentWidth, const std::shared_ptr<Transform>& element)
        {
          return std::max(currentWidth, element->GetWidth());
        });
        const auto& firstElement = _elements[0];
        const auto& lastElement = _elements[_elements.size() - 1];
        const auto height = lastElement->GetY() + lastElement->GetHeight() - firstElement->GetY();

        transform->SetSize(width, height);
      }
      break;
      default:
      {
        assert(_direction == kGroupDirection_Horizontal);

        const auto height = std::accumulate(std::cbegin(_elements), std::cend(_elements), 0, [](const int32_t currentHeight, const std::shared_ptr<Transform>& element)
        {
          return std::max(currentHeight, element->GetHeight());
        });
        const auto& firstElement = _elements[0];
        const auto& lastElement = _elements[_elements.size() - 1];
        const auto width = lastElement->GetX() + lastElement->GetWidth() - firstElement->GetX();

        transform->SetSize(width, height);
      }
      break;
      }
    }
  }

  bool TransformGroup::Contains(IGameObject* element) const
  {
    assert(element != nullptr);
    return std::find(std::cbegin(_elements), std::cend(_elements), element->transform) != std::end(_elements);
  }

  Vector2D_i32 TransformGroup::GetElementSize(const size_t index)
  {
    assert(index < Size());
    return _elements[index]->GetSize();
  }

  int32_t TransformGroup::GetElementSpacing(const size_t index)
  {
    assert(index < Size());
    assert(Size() == _spacing.size());
    return _spacing[index];
  }
}