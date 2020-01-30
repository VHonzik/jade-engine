#pragma once

#include "Vector.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <bitset>

namespace JadeEngine
{
  enum DirtyFlag
  {
    kDirtyFlag_position,
    kDirtyFlag_centerPosition,
    kDirtyFlag_size,
    kDirtyFlag_boundingBox,
    kDirtyFlag_count
  };

  /**
  Transform is %game objects' property that specify their position and size in world or screen coordinate system.
  */
  class Transform : public std::enable_shared_from_this<Transform>
  {
  public:
    Transform();
    void Initialize(const Vector& position, const Vector& size);
    void Initialize(const int32_t x, const int32_t y, const int32_t w, const int32_t h);

    int32_t GetX() const { return _position.x; }
    int32_t GetY() const { return _position.y; }
    int32_t GetWidth() const { return _size.w; }
    int32_t GetHeight() const { return _size.h; }
    int32_t GetCenterX() const { return _centerPosition.x; }
    int32_t GetCenterY() const { return _centerPosition.y; }

    Vector GetPosition() const { return _position; }
    Vector GetCenterPosition() const { return _centerPosition; }
    Vector GetSize() const { return _size; }
    Box GetBox() const { return _transformationBox; }
    Box GetBoundingBox() const { return _boundingBox; }
    Box GetTestingBox() const;

    void SetPosition(const Vector& position);
    void SetPosition(const int32_t x, const int32_t y);
    void SetCenterPosition(const Vector& centerPosition);
    void SetCenterPosition(const int32_t centerX, const int32_t centerY);
    void SetHeight(const int32_t height);
    void SetWidth(const int32_t width);
    void SetSize(const Vector& size);
    void SetSize(const int32_t width, const int32_t height);
    void SetBoundingBox(const Box& box);

    void SetLocalPosition(const Vector& position);
    void SetLocalPosition(const int32_t x, const int32_t y);

    void Attach(const std::shared_ptr<Transform>& transform);
    void Attach(const std::shared_ptr<Transform>& transform, const Vector& localPosition);
    void Detach();

    void Update();

    bool IsDirty(const DirtyFlag flag) const;
    void ResetDirty(const DirtyFlag flag);

  private:
    void OnAttached(const std::shared_ptr<Transform>& parent, const Vector& localPosition);
    void OnAttachedPositionChange();
    void OnChildDetached(const std::shared_ptr<Transform>& child);

    Vector _centerPosition;
    Vector _position;
    Vector _size;
    Vector _localPosition;

    Box _transformationBox;

    bool _boundingBoxSet;
    Box _boundingBox;

    std::bitset<kDirtyFlag_count> _dirtyFlags;

    std::shared_ptr<Transform> _parent;
    std::vector<std::shared_ptr<Transform>> _children;
  };

}