#pragma once

#include "Vector2D.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <bitset>

namespace JadeEngine
{
  class Transform;

  /**
  Specifies type of transform's properties that were changed - dirtied - last frame.

  @see Transform::IsDirty
  */
  enum DirtyFlag
  {
    /**
    Flag for the transform's position - `Transform::GetPosition`.
    */
    kDirtyFlag_position,

    /**
    Flag for the transform's center position - `Transform::GetCenterPosition`.
    */
    kDirtyFlag_centerPosition,

    /**
    Flag for the transform's position - `Transform::GetSize`.
    */
    kDirtyFlag_size,

    /**
    Flag for the transform's position - `Transform::GetBoundingBox`.
    */
    kDirtyFlag_boundingBox,

    /**
    Enumeration count for DirtyFlag. It has no logical meaning and it is not a valid flag for Transform functions.
    */
    kDirtyFlag_count
  };

  enum TransformBorderCoordinate
  {
    kTransformBorderCoordinate_left_top,
    kTransformBorderCoordinate_center,
    kTransformBorderCoordinate_right_bottom,
  };

  /**
  Specified a point on transform outline or center that will be used to position transform relative to another.
  */
  struct AttachmentPoint
  {
    /**
    X coordinate of the point.
    */
    TransformBorderCoordinate x;

    /**
    Y coordinate of the point.
    */
    TransformBorderCoordinate y;
  };

  const AttachmentPoint kDefaultAttachmentPoint = { kTransformBorderCoordinate_left_top, kTransformBorderCoordinate_left_top };
  const AttachmentPoint kCenterAttachmentPoint = { kTransformBorderCoordinate_center, kTransformBorderCoordinate_center };

  struct TransformAttachmentData
  {
    Vector2D_i32 localPosition;
    AttachmentPoint parentAttachmentPoint;
    AttachmentPoint childAttachmentPoint;
  };

  /**
  Transform is a %game object's property that specify their position and size in a coordinate system.

  The transform itself does not have concept of the coordinate system, that is entirely driven by the owning %game object. The coordinate system will usually be either world or screen coordinate system.
  */
  class Transform : public std::enable_shared_from_this<Transform>
  {
  public:
    /**
    Default constructor for transform that initializes the position and size to 0s.

    The bounding box will have position and size 0,0. The transform will have no parent.
    */
    Transform();

    /**
    Initializes the transform with position and size vectors.

    @param position Desired position of the game object as vector in pixels.
    @param size Desired dimensions of the transform as vector in pixels. This might not always correspond to the size of the game object itself.

    @code
    SomeGameObject::SomeGameObject(const SomeObjectParams& params)
    {
      // Assuming params contains `Vector initialPosition` and `Vector size`
      transform.InitializeVectors(initialPosition, size);
    }
    @endcode
    */
    void Initialize(const Vector2D_i32& position, const Vector2D_i32& size);

    /**
    Initializes the transform with position and size vector elements. See the vector version for more information.
    */
    void Initialize(const int32_t x, const int32_t y, const int32_t w, const int32_t h);

    /**
    Returns the x coordinate of the top-left corner of the transform.

    Same as `GetPosition().x`.
    */
    int32_t GetX() const { return _position.x; }

    /**
    Returns the y coordinate of the top-left corner of the transform.

    Same as `GetPosition().y`.
    */
    int32_t GetY() const { return _position.y; }

    /**
    Returns the width of the transform.

    Same as `GetSize().w`.
    */
    int32_t GetWidth() const { return _size.w; }

    /**
    Returns the height of transform.

    Same as `GetSize().h`.
    */
    int32_t GetHeight() const { return _size.h; }

    /**
    Returns the x coordinate of the center of the transform.

    Same as `GetCenterPosition().x`.
    */
    int32_t GetCenterX() const { return _centerPosition.x; }

    /**
    Returns the y coordinate of the center of the transform.

    Same as `GetCenterPosition().y`.
    */
    int32_t GetCenterY() const { return _centerPosition.y; }

    /**
    Returns the x and y coordinates of the top-left corner of the transform as a vector.
    */
    Vector2D_i32 GetPosition() const { return _position; }

    /**
    Returns the x and y coordinates of the center of the transform as a vector.
    */
    Vector2D_i32 GetCenterPosition() const { return _centerPosition; }

    /**
    Returns the width and height of the transform as a vector.
    */
    Vector2D_i32 GetSize() const { return _size; }

    /**
    Returns the transform representation as Box.
    */
    Box_i32 GetBox() const { return _transformationBox; }

    /**
    Returns the transform's bounding box.

    A bounding box that represents non-empty content of the transform box and its position is relative to the transform x,y.
    */
    Box_i32 GetBoundingBox() const { return _boundingBox; }

    /**
    Returns the transform's testing box.

    A testing box is a bounding box transformed by the transform position. That is a bounding box whose position was added up with transform's position.
    */
    Box_i32 GetTestingBox() const;

    /**
    Returns whether the transform is attached to a different transform - the parent.
    */
    bool IsAttached() const;

    /**
    Returns local position of the transform. This value only has effect when `IsAttached()` is true.

    The meaning of the local position depends on `AttachmentPoint` value that was passed to `Attach` function.

    @see Transform::Attach, Transform::IsAttached, AttachmentPoint
    */
    Vector2D_i32 GetLocalPosition() const { return _attachmentData.localPosition; }

    /**
    Set the transform x and y position as a vector in pixels.

    This will `kDirtyFlag_position` and `kDirtyFlag_centerPosition` flags to be dirtied until the end of the next frame.
    This will cause all children to be immediately moved as well.

    @see Transform::Attach, Transform::SetCenterPosition, Transform::GetPosition
    */
    void SetPosition(const Vector2D_i32& position);

    /**
    Set the transform x and y position as vector's element in pixels. See the vector version for more information.
    */
    void SetPosition(const int32_t x, const int32_t y);

    /**
    Set the transform's center x and y position as a vector in pixels.

    This will dirty the `kDirtyFlag_position` and `kDirtyFlag_centerPosition` flags for the next frame.
    This will cause all children to be immediately moved as well.

    @see Transform::Attach, Transform::SetPosition, Transform::GetCenterPosition
    */
    void SetCenterPosition(const Vector2D_i32& centerPosition);

    /**
    Set the transform's center x and y position as a vector's element in pixels. See the vector version for more information.
    */
    void SetCenterPosition(const int32_t centerX, const int32_t centerY);

    void SetPointPosition(const int32_t x, const int32_t y, const AttachmentPoint& point);
    void SetPointPosition(const Vector2D_i32& position, const AttachmentPoint& point);

    /**
    Set the height of the transform in pixels. Internally calls Transform::SetSize, see that for more information.

    @see Transform::SetSize
    */
    void SetHeight(const int32_t height);

    /**
    Set the width of the transform in pixels. Internally calls Transform::SetSize, see that for more information.

    @see Transform::SetSize
    */
    void SetWidth(const int32_t width);

    /**
    Set the width and height of the transform in pixels.

    If bounding box was not previously explicitly set (via `Transform::SetBoundingBox`), the bounding box will be resized to make its size match transform's size.
    This will dirty the `kDirtyFlag_size` and `kDirtyFlag_centerPosition` flags for the next frame. In the case bounding box was modified, `kDirtyFlag_boundingBox` flag will be dirtied as well.
    This will cause all children that were attached with `kAttachmentPoint_Center` to be immediately moved as well.

    @see Transform::SetBoundingBox
    */
    void SetSize(const Vector2D_i32& size);

    /**
    Set the width and height of the transform in pixels. See the vector version for more information.
    */
    void SetSize(const int32_t width, const int32_t height);

    /**
    Set the bounding box of the transform.

    A bounding box that represents non-empty content of the transform box and its position is relative to the transform x,y.
    The bounding box's size is by default tied to the transform's size until this function is called.
    This will dirty the `kDirtyFlag_boundingBox` flag for the next frame.

    @see Transform::GetBoundingBox, Transform::SetSize
    */
    void SetBoundingBox(const Box_i32& box);

    /**
    Set the transform's x and y of the local position in pixels.

    The meaning of the local position depends on `AttachmentPoint` value that was passed to `Attach` function.

    @pre The transform must have been previously attached to another transform via Transform::Attach. You can check whether transform is attached via Transform::IsAttached.
    @see Transform::Attach, Transform::IsAttached, AttachmentPoint
    */
    void SetLocalPosition(const Vector2D_i32& position);

    /**
    Set the transform's x and y of the local position in pixels. See the vector version for more information.
    */
    void SetLocalPosition(const int32_t x, const int32_t y);

    /**
    Attach `other` transform to `this` transform, making the `other` transform a child of `this` - parent - transform.

    Attachment essentially means that when the parent transform moves the child transform moves with it.

    As part of this call the child transform will be moved to the same position as the parent respecting `localPosition` and `attachmentPoint` parameters.

    @param other The transform that will become a child of `this` transform.
    @param localPosition The initial local position in pixels. The meaning of the local position depends on `attachmentPoint` and `otherAttachmentPoint` parameters.
    @param attachmentPoint The point on `this` transform where the `other` transform `otherAttachmentPoint` will be placed.
    @param otherAttachmentPoint The point on `other` transform that will placed where `attachmentPoint` of `this` is.

    @see Transform::SetLocalPosition, Transform::GetLocalPosition, Transform::IsAttached, AttachmentPoint
    */
    void Attach(const std::shared_ptr<Transform>& other, const Vector2D_i32& localPosition = kZeroVector2D_i32, const AttachmentPoint& attachmentPoint = kDefaultAttachmentPoint, const AttachmentPoint& otherAttachmentPoint = kDefaultAttachmentPoint);

    /**
    Detach the transform from its the parent.

    The transform will remain in its current position.

    @pre The transform must have been previously attached to another transform via Transform::Attach. You can check whether transform is attached via Transform::IsAttached.
    @see Transform::IsAttached, Transform::Attach
    */
    void Detach();

    /**
    Update the transform's dirty flags.

    @warning Used internally by the Jade Engine and there is very little reason to call this as a user.
    */
    void Update();

    /**
    Returns whether a property of transform has changed the previous frame.

    @note As the order of %game objects updates is not guaranteed, the dirty flag won't be "dirtied" the same frame the change in the property has occurred.
    @param flag Which property to check for.
    @returns True if property has changed the previous frame, false otherwise.

    @see DirtyFlag
    */
    bool IsDirty(const DirtyFlag flag) const;

  private:
    void OnAttached(const std::shared_ptr<Transform>& parent, const TransformAttachmentData& data);
    void OnAttachedPositionChange();
    void OnChildDetached(const std::shared_ptr<Transform>& child);

    Vector2D_i32 _centerPosition;
    Vector2D_i32 _position;
    Vector2D_i32 _size;

    TransformAttachmentData _attachmentData;

    bool _boundingBoxSet;
    Box_i32 _boundingBox;
    Box_i32 _transformationBox;

    std::bitset<kDirtyFlag_count> _dirtyFlagsCurrentFrame;
    std::bitset<kDirtyFlag_count> _dirtyFlags;

    std::shared_ptr<Transform> _parent;
    std::vector<std::shared_ptr<Transform>> _children;
  };

}