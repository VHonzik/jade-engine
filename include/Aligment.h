#pragma once

#include <functional>

namespace JadeEngine
{
  enum HorizontalAlignment
  {
    kHorizontalAlignment_Left,
    kHorizontalAlignment_Center,
    kHorizontalAlignment_Right
  };

  enum VerticalAlignment
  {
    kVerticalAlignment_Top,
    kVerticalAlignment_Center,
    kVerticalAlignment_Bottom
  };

  namespace detail
  {
    class ContainerElement
    {
    public:
      ContainerElement()
      {
        _getHeight = [](void* element) { return 0; };
        _getWidth = [](void* element) { return 0; };
        _getX = [](void* element) { return 0; };
        _getY = [](void* element) { return 0; };
        _getCenterX = [](void* element) { return 0; };
        _getCenterY = [](void* element) { return 0; };
        _setPosition = [](void*, int32_t x, int32_t y) { return; };
        _setCenterPosition = [](void*, int32_t x, int32_t y) { return; };
      }

      template<typename T>
      static ContainerElement Create(T* element)
      {
        ContainerElement result;
        result._getHeight = [](void* element) { return static_cast<T*>(element)->GetHeight(); };
        result._getWidth = [](void* element) { return static_cast<T*>(element)->GetWidth(); };
        result._getX = [](void* element) { return static_cast<T*>(element)->GetX(); };
        result._getY = [](void* element) { return static_cast<T*>(element)->GetY(); };
        result._getCenterX = [](void* element) { return static_cast<T*>(element)->GetCenterX(); };
        result._getCenterY = [](void* element) { return static_cast<T*>(element)->GetCenterY(); };
        result._setPosition = [](void* element, int32_t x, int32_t y) { return static_cast<T*>(element)->SetPosition(x, y); };
        result._setCenterPosition = [](void* element, int32_t x, int32_t y) { return static_cast<T*>(element)->SetCenterPosition(x, y); };
        result._element = static_cast<void*>(element);
        return result;
      }

      int32_t GetHeight() const { return _getHeight(_element); };
      int32_t GetWidth() const { return _getWidth(_element); };
      int32_t GetX() const { return _getX(_element); };
      int32_t GetY() const { return _getY(_element); };
      int32_t GetCenterX() const { return _getCenterX(_element); };
      int32_t GetCenterY() const { return _getCenterY(_element); };
      void SetElementPosition(int32_t x, int32_t y) { _setPosition(_element, x, y); }
      void SetElementCenterPosition(int32_t x, int32_t y) { _setCenterPosition(_element, x, y); }

    private:
      std::function<int32_t(void*)> _getHeight;
      std::function<int32_t(void*)> _getWidth;
      std::function<int32_t(void*)> _getX;
      std::function<int32_t(void*)> _getY;
      std::function<int32_t(void*)> _getCenterX;
      std::function<int32_t(void*)> _getCenterY;
      std::function<void(void*, int32_t, int32_t)> _setPosition;
      std::function<void(void*, int32_t, int32_t)> _setCenterPosition;

      void* _element;
    };

    template<typename T>
    ContainerElement make_container_element(T* element)
    {
      return ContainerElement::Create(element);
    }
  }
}
