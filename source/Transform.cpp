#include "Transform.h"

#include <algorithm>
#include <cassert>

namespace JadeEngine
{
  Transform::Transform()
    : _position(0, 0)
    , _size(0, 0)
    , _centerPosition(0, 0)
    , _dirtyFlags(0)
    , _boundingBox(0, 0, 0, 0)
    , _transformationBox(0, 0, 0, 0)
    , _boundingBoxSet(false)
  {
  }

  void Transform::Initialize(const Vector& position, const Vector& size)
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

  void Transform::SetPosition(const Vector& position)
  {
    if (position != _position)
    {
      _dirtyFlags.set(kDirtyFlag_centerPosition);
      _dirtyFlags.set(kDirtyFlag_position);
      _position = position;
      _centerPosition = _position + _size / 2;
      _transformationBox = { _position, _size };

      for (const auto& child : _children)
      {
        child->OnAttachedPositionChange();
      }
    }
  }

  void Transform::SetPosition(const int32_t x, const int32_t y)
  {
    SetPosition({ x, y });
  }

  void Transform::SetCenterPosition(const int32_t centerX, const int32_t centerY)
  {
    SetCenterPosition({ centerX, centerY });
  }

  void Transform::SetCenterPosition(const Vector& centerPosition)
  {
    if (centerPosition != _centerPosition)
    {
      _dirtyFlags.set(kDirtyFlag_centerPosition);
      _dirtyFlags.set(kDirtyFlag_position);
      _centerPosition = centerPosition;
      _position = _centerPosition - _size / 2;
      _transformationBox = { _position, _size };

      for (const auto& child : _children)
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

  void Transform::SetSize(const Vector& size)
  {
    if (size != _size)
    {
      _dirtyFlags.set(kDirtyFlag_centerPosition);
      _dirtyFlags.set(kDirtyFlag_size);
      _size = size;
      _centerPosition = _position + _size / 2;
      _transformationBox = { _position, _size };

      if (!_boundingBoxSet)
      {
        _boundingBox.size = size;
      }
    }
  }

  void Transform::SetBoundingBox(const Box& box)
  {
    _boundingBoxSet = true;
    if (box.position != _boundingBox.position || box.size != _boundingBox.size)
    {
      _dirtyFlags.set(kDirtyFlag_boundingBox);
      _boundingBox.position = box.position;
      _boundingBox.size = box.size;
    }
  }

  Box Transform::GetTestingBox() const
  {
    return { _boundingBox.position + _position, _boundingBox.size };
  }

  void Transform::Update()
  {
    _dirtyFlags.reset();
  }

  void Transform::SetLocalPosition(const Vector& position)
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
    SetPosition(_parent->GetPosition() + _localPosition);
  }

  void Transform::Attach(const std::shared_ptr<Transform>& transform)
  {
    Attach(transform, {});
  }

  void Transform::Attach(const std::shared_ptr<Transform>& transform, const Vector& localPosition)
  {
    assert(transform);
    _children.push_back(transform);
    transform->OnAttached(shared_from_this(), localPosition);
  }

  void Transform::OnAttached(const std::shared_ptr<Transform>& parent, const Vector& localPosition)
  {
    assert(parent);
    _parent = parent;
    _localPosition = localPosition;
    OnAttachedPositionChange();
  }

  void Transform::OnChildDetached(const std::shared_ptr<Transform>& child)
  {
    _children.erase(std::remove(std::begin(_children), std::end(_children), child));
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

  void Transform::ResetDirty(const DirtyFlag flag)
  {
    assert(flag != kDirtyFlag_count);
    _dirtyFlags[flag] = false;
  }
}