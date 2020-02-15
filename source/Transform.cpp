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

  void Transform::SetPositionAnchor(const int32_t x, const int32_t y, const Anchor& point)
  {
    SetPositionAnchor({ x, y }, point);
  }

  void Transform::SetPositionAnchor(const Vector2D_i32& position, const Anchor& point)
  {
    if (point == kAnchor_center)
    {
      SetCenterPosition(position);
    }
    else if (point == kAnchor_leftTop)
    {
      SetPosition(position);
    }

    Vector2D_i32 destination = position;
    switch (point)
    {
    case kAnchor_center:
    case kAnchor_centerBottom:
    case kAnchor_centerTop:
      destination.x -= GetWidth() / 2;
      break;
    case kAnchor_rightBottom:
    case kAnchor_rightCenter:
    case kAnchor_rightTop:
      destination.x -= GetWidth();
      break;
    }

    switch (point)
    {
    case kAnchor_center:
    case kAnchor_leftCenter:
    case kAnchor_rightCenter:
      destination.y -= GetHeight() / 2;
      break;
    case kAnchor_centerBottom:
    case kAnchor_leftBottom:
    case kAnchor_rightBottom:
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

      if (IsAttached() && _attachmentData.childAnchor != kAnchor_leftTop)
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
    Vector2D_i32 destination = _parent->GetPosition();

    switch (_attachmentData.parentAnchor)
    {
    case kAnchor_center:
    case kAnchor_centerBottom:
    case kAnchor_centerTop:
      destination.x = _parent->GetCenterX();
      break;
    case kAnchor_rightBottom:
    case kAnchor_rightCenter:
    case kAnchor_rightTop:
      destination.x = _parent->GetX() + _parent->GetWidth();
      break;
    }

    switch (_attachmentData.parentAnchor)
    {
    case kAnchor_center:
    case kAnchor_leftCenter:
    case kAnchor_rightCenter:
      destination.y = _parent->GetCenterY();
      break;
    case kAnchor_centerBottom:
    case kAnchor_leftBottom:
    case kAnchor_rightBottom:
      destination.y = _parent->GetY() + _parent->GetHeight();
      break;
    }

    // Special case where we want to use SetCenterPosition
    if (_attachmentData.childAnchor == kAnchor_center)
    {
      SetCenterPosition(destination + _attachmentData.localPosition);
    }
    else
    {
      Vector2D_i32 topLeftOffset = kZeroVector2D_i32;

      switch (_attachmentData.childAnchor)
      {
      case kAnchor_center:
      case kAnchor_centerBottom:
      case kAnchor_centerTop:
        topLeftOffset.x = -GetWidth() / 2;
        break;
      case kAnchor_rightBottom:
      case kAnchor_rightCenter:
      case kAnchor_rightTop:
        topLeftOffset.x = -GetWidth();
        break;
      }

      switch (_attachmentData.childAnchor)
      {
      case kAnchor_center:
      case kAnchor_leftCenter:
      case kAnchor_rightCenter:
        topLeftOffset.y = -GetHeight() / 2;
        break;
      case kAnchor_centerBottom:
      case kAnchor_leftBottom:
      case kAnchor_rightBottom:
        topLeftOffset.y = -GetHeight();
        break;
      }

      SetPosition(destination + topLeftOffset + _attachmentData.localPosition);
    }
  }

  void Transform::Attach(const std::shared_ptr<Transform>& other, const Vector2D_i32& localPosition, const Anchor& anchor, const Anchor& otherAnchor)
  {
    assert(other);
    _children.push_back(other);

    other->OnAttached(shared_from_this(), { localPosition, anchor, otherAnchor });
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