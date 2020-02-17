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
    kDirtyFlag_Position,

    /**
    Flag for the transform's center position - `Transform::GetCenterPosition`.
    */
    kDirtyFlag_CenterPosition,

    /**
    Flag for the transform's position - `Transform::GetSize`.
    */
    kDirtyFlag_Size,

    /**
    Flag for the transform's position - `Transform::GetBoundingBox`.
    */
    kDirtyFlag_BoundingBox,

    /**
    Enumeration count for DirtyFlag. It has no logical meaning and it is not a valid flag for Transform functions.
    */
    kDirtyFlag_Count
  };

  /**
  Anchor specifies a point within an object's bounding box.

  In the process of attaching one transform to another one can specify parent and child anchors for convenient positioning.
  */
  enum Anchor
  {
    /**
    Left-top corner of the bounding box.

    *═══╗
    ║   ║
    ╚═══╝
    */
    kAnchor_LeftTop,

    /**
    Center-top of the bounding box.
    ╔═*═╗
    ║   ║
    ╚═══╝
    */
    kAnchor_CenterTop,

    /**
    Right-top corner of the bounding box.
    ╔═══*
    ║   ║
    ╚═══╝
    */
    kAnchor_RightTop,

    /**
    Right-center of the bounding box.
    ╔═══╗
    ║   *
    ╚═══╝
    */
    kAnchor_RightCenter,

    /**
    Right-bottom corner of the bounding box.
    ╔═══╗
    ║   ║
    ╚═══*
    */
    kAnchor_RightBottom,

    /**
    Center-bottom of the bounding box.
    ╔═══╗
    ║   ║
    ╚═*═╝
    */
    kAnchor_CenterBottom,

    /**
    Left-bottom corner of the bounding box.
    ╔═══╗
    ║   ║
    *═══╝
    */
    kAnchor_LeftBottom,

    /**
    Left-center of the bounding box.
    ╔═══╗
    *   ║
    ╚═══╝
    */
    kAnchor_LeftCenter,

    /**
    Center of the bounding box.
    ╔═══╗
    ║ * ║
    ╚═══╝
    */
    kAnchor_Center,
  };

  const Anchor kDefaultAnchor = kAnchor_LeftTop;

  namespace detail
  {
    struct TransformAttachmentData
    {
      Vector2D_i32 localPosition;
      Anchor parentAnchor;
      Anchor childAnchor;
    };
  }

  using detail::TransformAttachmentData;

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

    Local position is essentially a vector from the position of parent's anchor to the position of this (child) anchor as they were specified in `Attach` call.

    @see Transform::Attach, Transform::IsAttached, Anchor
    */
    Vector2D_i32 GetLocalPosition() const { return _attachmentData.localPosition; }

    /**
    Set the transform x and y position as a vector in pixels.

    This will `kDirtyFlag_Position` and `kDirtyFlag_CenterPosition` flags to be dirtied until the end of the next frame.
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

    This will dirty the `kDirtyFlag_Position` and `kDirtyFlag_CenterPosition` flags for the next frame.
    This will cause all children to be immediately moved as well.

    @see Transform::Attach, Transform::SetPosition, Transform::GetCenterPosition
    */
    void SetCenterPosition(const Vector2D_i32& centerPosition);

    /**
    Set the transform's center x and y position as a vector's element in pixels. See the vector version for more information.
    */
    void SetCenterPosition(const int32_t centerX, const int32_t centerY);

    /**
    Set transforms x and y position so that the passed anchor point will be positioned exactly at the passed position as vector in pixels.

    If the `kAnchor_Center` is passed this is the same as calling `SetCenterPosition` including dirty flag effects.
    If the `kAnchor_LeftTop` is passed this is the same as calling `SetPosition` including dirty flag effects.
    Otherwise this will internally call `SetPosition`, including dirty flag effects, with appropriate offset.

    @see Anchor, Transform::SetPosition, Transform::SetCenterPosition
    */
    void SetPositionAnchor(const Vector2D_i32& position, const Anchor& point);

    /**
    Set transforms x and y position so that the passed anchor point will be positioned exactly at the passed position as vector's elements in pixels. See the vector version for more information.
    */
    void SetPositionAnchor(const int32_t x, const int32_t y, const Anchor& point);

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
    This will dirty the `kDirtyFlag_Size` and `kDirtyFlag_CenterPosition` flags for the next frame. In the case bounding box was modified, `kDirtyFlag_BoundingBox` flag will be dirtied as well.
    This will cause all children to be immediately moved as well unless they were attached with `kAnchor_LeftTop` as parent anchor.

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
    This will dirty the `kDirtyFlag_BoundingBox` flag for the next frame.

    @see Transform::GetBoundingBox, Transform::SetSize
    */
    void SetBoundingBox(const Box_i32& box);

    /**
    Set the transform's x and y of the local position in pixels.

    Local position is essentially a vector from the position of parent's anchor to the position of this (child) anchor as they were specified in `Attach` call.

    @pre The transform must have been previously attached to another transform via Transform::Attach. You can check whether transform is attached via Transform::IsAttached.
    @see Transform::Attach, Transform::IsAttached, Anchor
    */
    void SetLocalPosition(const Vector2D_i32& position);

    /**
    Set the transform's x and y of the local position in pixels. See the vector version for more information.
    */
    void SetLocalPosition(const int32_t x, const int32_t y);

    /**
    Attach `other` transform to `this` transform, making the `other` transform a child of `this` - parent - transform.

    Attachment essentially means that when the parent transform moves the child transform moves with it.

    As part of this call the child transform will be moved to have correct relative position to the parent respecting `localPosition`, `parentAnchor` and `childAnchor` parameters.

    @param other The transform that will become a child of `this` transform.
    @param localPosition The initial local position in pixels. Local position is essentially a vector from the position of parent's anchor to the position of the child anchor.
    @param anchor The point on `this` transform where the `other` transform `otherAttachmentPoint` will be placed.
    @param otherAnchor The point on `other` transform that will placed where `anchor` of `this` is.

    @see Transform::SetLocalPosition, Transform::GetLocalPosition, Transform::IsAttached, Anchor
    */
    void Attach(const std::shared_ptr<Transform>& other, const Vector2D_i32& localPosition = kZeroVector2D_i32, const Anchor& anchor = kDefaultAnchor, const Anchor& otherAnchor = kDefaultAnchor);

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
    void OnAttached(const std::shared_ptr<Transform>& parent, const detail::TransformAttachmentData& data);
    void OnAttachedPositionChange();
    void OnChildDetached(const std::shared_ptr<Transform>& child);

    Vector2D_i32 _centerPosition;
    Vector2D_i32 _position;
    Vector2D_i32 _size;

    TransformAttachmentData _attachmentData;

    bool _boundingBoxSet;
    Box_i32 _boundingBox;
    Box_i32 _transformationBox;

    std::bitset<kDirtyFlag_Count> _dirtyFlagsCurrentFrame;
    std::bitset<kDirtyFlag_Count> _dirtyFlags;

    std::shared_ptr<Transform> _parent;
    std::vector<std::shared_ptr<Transform>> _children;
  };

}