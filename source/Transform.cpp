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
    , _attachmentPoint(kAttachmentPoint_TopLeft)
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

      for (const auto& child : _childrenTopLeft)
      {
        child->OnAttachedPositionChange();
      }
      for (const auto& child : _childrenCenter)
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

      for (const auto& child : _childrenTopLeft)
      {
        child->OnAttachedPositionChange();
      }
      for (const auto& child : _childrenCenter)
      {
        child->OnAttachedPositionChange();
      }
    }
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

      for (const auto& child : _childrenCenter)
      {
        child->OnAttachedPositionChange();
      }

      if (!_boundingBoxSet)
      {
        _boundingBox.size = size;
        _dirtyFlagsCurrentFrame.set(kDirtyFlag_boundingBox);
      }

      if (IsAttached() && _attachmentPoint == kAttachmentPoint_Center)
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
    _localPosition = position;

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
    switch (_attachmentPoint)
    {
    case kAttachmentPoint_TopLeft:
      SetPosition(_parent->GetPosition() + _localPosition);
      break;
    default:
      assert(_attachmentPoint == kAttachmentPoint_Center);
      SetCenterPosition(_parent->GetCenterPosition() + _localPosition);
      break;
    }
  }

  void Transform::Attach(const std::shared_ptr<Transform>& other, const Vector2D_i32& localPosition, const AttachmentPoint attachmentPoint /*= kAttachmentPoint_TopLeft*/)
  {
    assert(other);
    switch (attachmentPoint)
    {
    case kAttachmentPoint_TopLeft:
      _childrenTopLeft.push_back(other);
      break;
    default:
      assert(attachmentPoint == kAttachmentPoint_Center);
      _childrenCenter.push_back(other);
      break;
    }

    other->OnAttached(shared_from_this(), localPosition, attachmentPoint);
  }

  void Transform::OnAttached(const std::shared_ptr<Transform>& parent, const Vector2D_i32& localPosition, const AttachmentPoint attachmentPoint)
  {
    assert(parent);
    _parent = parent;
    _localPosition = localPosition;
    _attachmentPoint = attachmentPoint;
    OnAttachedPositionChange();
  }

  void Transform::OnChildDetached(const std::shared_ptr<Transform>& child)
  {
    _childrenTopLeft.erase(std::remove(std::begin(_childrenTopLeft), std::end(_childrenTopLeft), child));
    _childrenCenter.erase(std::remove(std::begin(_childrenCenter), std::end(_childrenCenter), child));
  }

  void Transform::Detach()
  {
    assert(_parent);
    _localPosition = {};
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