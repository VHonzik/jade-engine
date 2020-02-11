#include "Transform.h"

#include <algorithm>
#include <cassert>

namespace JadeEngine
{
  Transform::Transform()
    : _boundingBox(0, 0, 0, 0)
    , _boundingBoxSet(false)
    , _centerPosition(0, 0)
    , _dirtyFlags(0)
    , _dirtyFlagsCurrentFrame(0)
    , _parent(nullptr)
    , _position(0, 0)
    , _size(0, 0)
    , _transformationBox(0, 0, 0, 0)
  {
  }

  void Transform::Initialize(const Vector2D_i32& position, const Vector2D_i32& size)
  {
    _position = position;
    _size = size;
    _transformationBox = { position, size };
    _centerPosition = _position + _size / 2;
    _boundingBox = { {0, 0}, size };
    _boundingBoxSet = false;
  }

  void Transform::Initialize(const int32_t x, const int32_t y, const int32_t w, const int32_t h)
  {
    Initialize({ x, y }, { w, h });
  }

  void Transform::SetPosition(const int32_t x, const int32_t y)
  {
    SetPosition({ x ,y });
  }

  void Transform::SetPosition(const Vector2D_i32& position)
  {
    if (position != _position)
    {
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_centerPosition);
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_position);
      _position = position;
      _centerPosition = _position + _size / 2;
      _transformationBox = { _position, _size };

      for (const auto& child : _children)
      {
        child->OnAttachedPositionChange();
      }
    }
  }

  void Transform::SetCenterPosition(const int32_t centerX, const int32_t centerY)
  {
    SetCenterPosition({ centerX, centerY });
  }

  void Transform::SetCenterPosition(const Vector2D_i32& centerPosition)
  {
    if (centerPosition != _centerPosition)
    {
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_centerPosition);
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_position);
      _centerPosition = centerPosition;
      _position = _centerPosition - _size / 2;
      _transformationBox = { _position, _size };

      for (const auto& child : _children)
      {
        child->OnAttachedPositionChange();
      }
    }
  }

  void Transform::SetPointPosition(const int32_t x, const int32_t y, const AttachmentPoint& point)
  {
    SetPointPosition({ x, y }, point);
  }

  void Transform::SetPointPosition(const Vector2D_i32& position, const AttachmentPoint& point)
  {
    Vector2D_i32 destination = position;
    switch (point.x)
    {
    case kTransformBorderCoordinate_left_top:
      destination.x -= 0;
      break;
    case kTransformBorderCoordinate_center:
      destination.x -= GetWidth() / 2;
      break;
    default:
      assert(point.x == kTransformBorderCoordinate_right_bottom);
      destination.x -= GetWidth();
      break;
    }

    switch (point.y)
    {
    case kTransformBorderCoordinate_left_top:
      destination.y -= 0;
      break;
    case kTransformBorderCoordinate_center:
      destination.y -= GetHeight() / 2;
      break;
    default:
      assert(point.y == kTransformBorderCoordinate_right_bottom);
      destination.y -= GetHeight();
      break;
    }

    SetPosition(destination);
  }

  void Transform::SetHeight(const int32_t height)
  {
    SetSize({ GetWidth(), height });
  }

  void Transform::SetWidth(const int32_t width)
  {
    SetSize({ width, GetHeight() });
  }

  void Transform::SetSize(const int32_t width, const int32_t height)
  {
    SetSize({ width, height });
  }

  void Transform::SetSize(const Vector2D_i32& size)
  {
    if (size != _size)
    {
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_centerPosition);
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_size);
      _size = size;
      _centerPosition = _position + _size / 2;
      _transformationBox = { _position, _size };

      for (const auto& child : _children)
      {
        child->OnAttachedPositionChange();
      }

      if (!_boundingBoxSet)
      {
        _boundingBox.size = size;
        _dirtyFlagsCurrentFrame.set(kDirtyFlag_boundingBox);
      }

      if (IsAttached() && (_attachmentData.childAttachmentPoint.x == kTransformBorderCoordinate_center || _attachmentData.childAttachmentPoint.y == kTransformBorderCoordinate_center))
      {
        OnAttachedPositionChange();
      }
    }
  }

  void Transform::SetBoundingBox(const Box_i32& box)
  {
    _boundingBoxSet = true;
    if (box.position != _boundingBox.position || box.size != _boundingBox.size)
    {
      _dirtyFlagsCurrentFrame.set(kDirtyFlag_boundingBox);
      _boundingBox.position = box.position;
      _boundingBox.size = box.size;
    }
  }

  Box_i32 Transform::GetTestingBox() const
  {
    return { _boundingBox.position + _position, _boundingBox.size };
  }

  void Transform::Update()
  {
    _dirtyFlags.reset();
    _dirtyFlags = _dirtyFlagsCurrentFrame;
    _dirtyFlagsCurrentFrame.reset();
  }

  void Transform::SetLocalPosition(const Vector2D_i32& position)
  {
    _attachmentData.localPosition = position;

    assert(_parent);
    if (_parent)
    {
      OnAttachedPositionChange();
    }
  }

  void Transform::SetLocalPosition(const int32_t x, const int32_t y)
  {
    SetLocalPosition({ x, y });
  }

  void Transform::OnAttachedPositionChange()
  {
    assert(_parent);
    Vector2D_i32 destination = kZeroVector2D_i32;
    switch (_attachmentData.parentAttachmentPoint.x)
    {
    case kTransformBorderCoordinate_left_top:
      destination.x = _parent->GetX();
      break;
    case kTransformBorderCoordinate_center:
      destination.x = _parent->GetCenterX();
      break;
    default:
      assert(_attachmentData.parentAttachmentPoint.x == kTransformBorderCoordinate_right_bottom);
      destination.x = _parent->GetX() + _parent->GetWidth();
      break;
    }

    switch (_attachmentData.parentAttachmentPoint.y)
    {
    case kTransformBorderCoordinate_left_top:
      destination.y = _parent->GetY();
      break;
    case kTransformBorderCoordinate_center:
      destination.y = _parent->GetCenterY();
      break;
    default:
      assert(_attachmentData.parentAttachmentPoint.y == kTransformBorderCoordinate_right_bottom);
      destination.y = _parent->GetY() + _parent->GetHeight();
      break;
    }

    // Special case where we want to use SetCenterPosition
    if (_attachmentData.childAttachmentPoint.x == kTransformBorderCoordinate_center && _attachmentData.childAttachmentPoint.y == kTransformBorderCoordinate_center)
    {
      SetCenterPosition(destination + _attachmentData.localPosition);
    }
    else
    {
      Vector2D_i32 topLeftOffset = kZeroVector2D_i32;

      switch (_attachmentData.childAttachmentPoint.x)
      {
      case kTransformBorderCoordinate_left_top:
        topLeftOffset.x = 0;
        break;
      case kTransformBorderCoordinate_center:
        topLeftOffset.x = -GetWidth() / 2;
        break;
      default:
        assert(_attachmentData.parentAttachmentPoint.x == kTransformBorderCoordinate_right_bottom);
        topLeftOffset.x = -GetWidth();
        break;
      }

      switch (_attachmentData.childAttachmentPoint.y)
      {
      case kTransformBorderCoordinate_left_top:
        topLeftOffset.y = 0;
        break;
      case kTransformBorderCoordinate_center:
        topLeftOffset.y = -GetHeight() / 2;
        break;
      default:
        assert(_attachmentData.parentAttachmentPoint.y == kTransformBorderCoordinate_right_bottom);
        topLeftOffset.y = -GetHeight();
        break;
      }

      SetPosition(destination + topLeftOffset + _attachmentData.localPosition);
    }
  }

  void Transform::Attach(const std::shared_ptr<Transform>& other, const Vector2D_i32& localPosition, const AttachmentPoint& attachmentPoint /*= kDefaultAttachmentPoint*/, const AttachmentPoint& otherAttachmentPoint /*= kDefaultAttachmentPoint*/)
  {
    assert(other);
    _children.push_back(other);

    other->OnAttached(shared_from_this(), {localPosition, attachmentPoint, otherAttachmentPoint});
  }

  void Transform::OnAttached(const std::shared_ptr<Transform>& parent, const TransformAttachmentData& data)
  {
    assert(parent);
    _parent = parent;
    _attachmentData = data;
    OnAttachedPositionChange();
  }

  void Transform::OnChildDetached(const std::shared_ptr<Transform>& child)
  {
    _children.erase(std::remove(std::begin(_children), std::end(_children), child));
  }

  void Transform::Detach()
  {
    assert(_parent);
    _parent->OnChildDetached(shared_from_this());
    _parent = nullptr;
  }

  bool Transform::IsDirty(const DirtyFlag flag) const
  {
    assert(flag != kDirtyFlag_count);
    return _dirtyFlags[flag];
  }

  bool Transform::IsAttached() const
  {
    return _parent != nullptr;
  }
}