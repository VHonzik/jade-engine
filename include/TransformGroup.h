#pragma once

#include "Alignment.h"
#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Transform.h"

#include <memory>
#include <vector>

namespace JadeEngine
{
  struct TransformGroupParams
  {
    ObjectLayer     layer;
    GroupDirection  direction;
    int32_t         spacing;
    int32_t         alignment;
  };

  class TransformGroup : public IGameObject
  {
  public:
    TransformGroup(const TransformGroupParams& params);

    void Add(IGameObject* element);
    void Add(IGameObject* element, int32_t elementSpacing);
    void Add(const std::vector<IGameObject*>& elements);
    template<typename InputIt>
    void Add(InputIt first, InputIt last)
    {
      for (; first != last; ++first)
      {
        Add(*first);
      }
      if (_elements.size() > 0)
      {
        Align();
      }
    }

    bool Contains(IGameObject* element) const;

  private:
    void Add(const std::shared_ptr<Transform>& elementTransform);
    void Add(const std::shared_ptr<Transform>& elementTransform, const int32_t spacing);
    void Align();

    static Anchor ParentAnchor(const GroupDirection direction, const int32_t alignment);
    static Anchor ChildAnchor(const GroupDirection direction, const int32_t alignment);

    int32_t _alignment;
    GroupDirection _direction;
    std::vector<std::shared_ptr<Transform>> _elements;
    std::vector<int32_t> _spacing;
    int32_t _defaultSpacing;
    Anchor _parentAnchor;
    Anchor _childAnchor;
  };
}
